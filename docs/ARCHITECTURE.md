# Arquitectura y decisiones técnicas

Documento de referencia de **por qué** vanilux está hecho como está.
Para instrucciones de uso/instalación ver el [README](../README.md).

## Índice

1. [Vista general del flujo](#1-vista-general-del-flujo)
2. [Descubrimiento de aplicaciones](#2-descubrimiento-de-aplicaciones)
3. [El widget `AppIconButton`](#3-el-widget-appiconbutton)
4. [El glow: por qué Cairo y no CSS](#4-el-glow-por-qué-cairo-y-no-css)
5. [Blur uniforme (box blur separable)](#5-blur-uniforme-box-blur-separable)
6. [Modelo de selección única (focus-follows-mouse)](#6-modelo-de-selección-única-focus-follows-mouse)
7. [Navegación por teclado](#7-navegación-por-teclado)
8. [Filtros, tags, favoritos y recientes](#8-filtros-tags-favoritos-y-recientes)
9. [Rendimiento de arranque](#9-rendimiento-de-arranque)
10. [Proceso residente y señales](#10-proceso-residente-y-señales)
11. [Estilado (CSS) y resolución de assets](#11-estilado-css-y-resolución-de-assets)

---

## 1. Vista general del flujo

```
main()                                  (src/main.cpp)
  ├─ ¿instancia viva? → SIGUSR1 y exit   (toggle del residente)
  └─ si no:
       app = Gtk::Application::create(NON_UNIQUE)
       app->hold()                        (sigue vivo con la ventana oculta)
       LauncherWindow window              (constructor monta toda la UI)
       g_unix_signal_add(SIGUSR1/SIGTERM)
       app->run(window)                   (muestra la ventana y corre el loop)

LauncherWindow()                         (src/launcher_window.cpp)
  ├─ load_css()                          (lee /usr/local/share/.../style.css)
  ├─ load_persisted_data()               (favorites.txt / recents.txt)
  └─ setup_ui()
       ├─ scan_applications()            (~7 ms)
       ├─ arma search / sidebar / grilla / status
       └─ difiere rebuild_all() a un idle (la ventana pinta primero)
```

`LauncherWindow` deriva de `Gtk::Window`. Cada ítem es un `AppIconButton`
(deriva de `Gtk::EventBox`).

## 2. Descubrimiento de aplicaciones

`AppDiscovery::scan_applications()` recorre los directorios estándar de
`.desktop` y para cada archivo llama a `parse_desktop_file()`, que:

- Lee solo el grupo `[Desktop Entry]`.
- Extrae `Name`, `Exec`, `Icon`, `Categories`, `TryExec`.
- **Descarta** entradas con `NoDisplay=true`, `Hidden=true`, sin `Name` o sin
  `Exec`, o cuyo `TryExec` no es ejecutable en el `PATH`.
- Limpia los *field codes* del `Exec` (`%f`, `%U`, `%i`, …) con una regex.
- Deduplica por nombre (con *case folding* Unicode vía `Glib::ustring`).

El resultado se ordena alfabéticamente. La categoría visible de cada app se
deriva heurísticamente de `Categories` en `LauncherWindow::detect_category()`
(p. ej. `WebBrowser/Email/Network → internet`, `X-WayDroid-App → android`).

> Costo medido: ~7 ms para ~130 apps. **No** es el cuello de botella del
> arranque (ver §9).

## 3. El widget `AppIconButton`

Un `Gtk::EventBox` (necesita ventana propia para recibir enter/leave y foco)
que contiene:

```
EventBox (.app-button, can_focus)
└─ Box vertical
   ├─ Overlay (88×72)
   │   ├─ Box "overlay_main"     → icon_wrapper (54×54, margin-top 17) → Image 48px
   │   └─ overlay: botón ★ (fav-toggle), arriba-derecha, fondo transparente
   └─ Label (nombre, wrap, 2 líneas)
```

Detalles no obvios:

- `margin-top: 17` en el `icon_wrapper` reserva espacio **arriba** del ícono
  para que el glow no se corte (en un `Box` vertical, el `valign` de un hijo
  `PACK_SHRINK` no tiene efecto: el hueco queda abajo y empujaría la etiqueta).
- Las **estrellas** (`star_empty_rounded.svg` / `star_filled_rounded.svg`) se
  cargan **una sola vez** en `static` y se reusan en todos los botones.
- El ícono de la app se resuelve así:
  - ruta de archivo (`/...` o `src/icons/...`) → `Gdk::Pixbuf::create_from_file`
    con cache estático por clave;
  - nombre de tema → `m_image.set_from_icon_name(...)` para que **GTK lo
    rasterice perezosamente al dibujar** (solo los visibles), con su cache
    interna.

## 4. El glow: por qué Cairo y no CSS

**Problema.** La idea original era un `box-shadow` ámbar en `.icon-wrapper`
activado por `:hover`/clase de estado. En **GTK 3 esto es poco fiable**: la
sombra se renderiza la primera vez, pero al volver a entrar el puntero GTK no
re-invalida la región de la sombra (que se dibuja *fuera* de la asignación del
widget) y queda el bloque de fondo sin el halo.

**Solución.** El borde y el glow se dibujan a mano en
`AppIconButton::on_draw()` con Cairo, y en los handlers de `enter`/`leave`/
`focus` se llama `queue_draw()` para forzar un repintado completo. Así se
repinta correctamente **siempre**.

Claves del dibujo:

- Se dibuja **después** de `Gtk::EventBox::on_draw(cr)` (encima del fondo y los
  hijos, pero el halo queda fuera/al borde del ícono, sin tapar la imagen).
- El marco se ancla al **ícono real** (`m_image`), con tamaño fijo (56×56
  centrado en el ícono), **no** al `icon_wrapper` — porque el wrapper se estira
  en la grilla y haría el marco más alto que el ícono.
- Condición de activación: `has_focus()` (ver §6).

```cpp
bool AppIconButton::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    bool ret = Gtk::EventBox::on_draw(cr);
    if (!has_focus()) return ret;          // sólo el ícono seleccionado
    // ... calcula el rect (56×56) centrado en m_image ...
    // 1) glow difuso (superficie aparte + blur) ; 2) borde nítido 2px ámbar
    return ret;
}
```

## 5. Blur uniforme (box blur separable)

Apilar muchos trazos concéntricos para simular el halo se **nota como capas**.
En su lugar:

1. Se estampa el borde redondeado en una `Cairo::ImageSurface` ARGB32 con un
   margen `M` alrededor.
2. Se le aplica un **box blur separable** (`blur_argb_surface`): pasada
   horizontal + pasada vertical, repetido 3 veces ⇒ aproxima una gaussiana y da
   un degradé liso, sin bandas.
3. Se pinta esa superficie debajo del borde nítido.

El blur opera sobre los 4 canales premultiplicados (BGRA en little-endian) con
ventana acotada en los bordes. Como **solo el ícono enfocado** ejecuta este
camino (el resto sale temprano en `on_draw`), el costo por frame es
despreciable. La intensidad del halo se mantiene **muy tenue** a pedido (el
borde nítido sí es bien visible).

## 6. Modelo de selección única (focus-follows-mouse)

Requisito: que haya **una sola** selección, sea por mouse o por teclado.

- El glow se activa exclusivamente con `has_focus()`.
- Al **entrar** el mouse a un `AppIconButton` se hace `grab_focus()`
  (*focus-follows-mouse*): el hover y la selección de teclado son lo mismo.
- El foco inicial se **estaciona en el buscador**, así ningún ícono arranca
  "seleccionado".
- Los handlers `focus-in`/`focus-out` hacen `queue_draw()` (y muestran/ocultan
  la estrella) para que el glow se repinte al moverse.

Esto evita el bug previo de "dos íconos brillando" (uno por foco de arranque y
otro por hover).

## 7. Navegación por teclado

`LauncherWindow::on_key_press_event()` intercepta:

- **Esc** → ocultar.
- **Enter / KP_Enter** → si el foco es un `AppIconButton`, lanzar su app.
- **`f` / `F`** → si el foco es un `AppIconButton`, alternar favorito. (Sólo
  actúa con un ícono enfocado, así escribir "f" en la búsqueda funciona normal.)
- **Flechas** → `handle_custom_navigation()`.

`handle_custom_navigation()` modela una grilla: lee la posición
`(left_attach, top_attach)` de cada celda y mueve el foco a la celda vecina en
la dirección pedida, con transiciones entre secciones (Buscador ↔ Favoritos ↔
Recientes ↔ Todas ↔ barra de tags). Para que las flechas lleguen a los íconos,
cada `AppIconButton` se crea con `set_can_focus(true)`.

## 8. Filtros, tags, favoritos y recientes

- La barra lateral se arma con un *helper* `make_tag_btn(id, nombre, svg,
  activa)`. Siempre se agregan **Todas**, **Favoritos** y **Recientes** arriba;
  luego las categorías reales que tengan apps.
- `set_category(id)` arma `m_apps` según el caso (`all` = todas;
  `favorites`/`recents` = entradas resueltas desde las listas persistidas;
  cualquier otra = filtro por `detect_category`) y llama `rebuild_all()`.
- `rebuild_all()` reconstruye 3 grillas: Favoritos, Recientes y la principal.
  Las dos primeras **solo se muestran** cuando la tag es *Todas* y no hay
  búsqueda activa (`show_sections`); en cualquier otra tag se ve solo la lista
  filtrada en la grilla principal.
- Persistencia: `favorites.txt` y `recents.txt` en `~/.config/vanilux/`
  (texto plano, un nombre por línea; recientes acotado a 6).

## 9. Rendimiento de arranque

Medición (instrumentando con `std::chrono`, ~130 apps):

| Etapa | Costo |
|---|---|
| `scan_applications()` | ~7 ms |
| construir 131 botones (`populate_grid`) | ~65 ms |
| `show_all()` de las grillas | ~50 ms |
| **carga del índice del tema Papirus** (1ª vez) | **~310 ms** |

Conclusión: el cuello de botella **no** es decodificar íconos ni crear widgets,
sino la **carga única del índice del tema Papirus** la primera vez que se
consulta. Mitigaciones aplicadas:

1. **Poblado inicial diferido** — `rebuild_all()` se difiere a un
   `Glib::signal_idle` de prioridad por debajo del redibujo, así la ventana
   **pinta el marco al instante** y la grilla entra a continuación.
2. **Rasterizado perezoso de íconos de tema** — `set_from_icon_name` deja que
   GTK dibuje cada ícono al hacerse visible (solo los del viewport), con cache
   interna.
3. **Caches estáticas** — estrellas (2 SVG) e íconos de archivo, reusados entre
   las grillas Favoritos/Recientes/Todas y entre reconstrucciones.
4. **Proceso residente** (§10) — el costo de Papirus se paga **una sola vez por
   sesión**; las aperturas siguientes son inmediatas.

## 10. Proceso residente y señales

`main.cpp` implementa un patrón *single-instance con toggle*:

- Al lanzarse, lee `/tmp/vanilux.pid`. Si ese PID está vivo
  (`kill(pid, 0) == 0`), le manda **`SIGUSR1`** y termina (exit 0). La instancia
  viva alterna mostrar/ocultar.
- Si no hay instancia, se convierte en la **residente**: escribe su PID, hace
  `app->hold()` (para no morir cuando la ventana se oculta) y corre el loop.
- Las señales se integran al loop de GLib con `g_unix_signal_add()`
  (seguro para tocar GTK desde el callback):
  - `SIGUSR1` → `LauncherWindow::toggle_visibility()`.
  - `SIGTERM` → `app->quit()` (cierra limpio y borra el PID file).

`hide_launcher()` oculta y **resetea la vista mientras está oculta** (limpia
búsqueda, vuelve a *Todas*, scroll arriba) para que `show_launcher()` sea un
simple `show()` + `present()` + `grab_focus()` instantáneo.

> Un PID *stale* (proceso muerto) se detecta con `kill(pid,0) != 0` y se trata
> como "no hay instancia", arrancando normal.

## 11. Estilado (CSS) y resolución de assets

- `load_css()` busca el CSS en este orden:
  `/usr/local/share/vanilux/style.css` → `src/style.css` → un **CSS
  embebido** en el binario (último recurso). Mantener `src/style.css` y el
  embebido razonablemente en sync.
- `resolve_icon_path()` traduce rutas `src/icons/<x>` a
  `/usr/local/share/vanilux/icons/<x>` si existe la versión instalada,
  para que funcione tanto instalado como ejecutando desde el repo.
- El CSS define los paneles tipo TUI, las tags, el buscador, la barra de estado
  y los *keycaps*. El **glow del ícono no está en el CSS** (es Cairo, §4); el
  CSS solo aplica color al label en hover/focus.
