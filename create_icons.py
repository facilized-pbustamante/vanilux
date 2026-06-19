import os

os.makedirs('src/icons', exist_ok=True)

# Helper function to generate SVG content
def make_svg(path, color, svg_content):
    full_content = f'''<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 48 48" width="48" height="48">
  <defs>
    <filter id="glow" x="-40%" y="-40%" width="180%" height="180%">
      <feGaussianBlur stdDeviation="1.5" result="coloredBlur"/>
      <feMerge>
        <feMergeNode in="coloredBlur"/>
        <feMergeNode in="SourceGraphic"/>
      </feMerge>
    </filter>
  </defs>
  {svg_content.replace('COLOR', color)}
</svg>'''
    with open(os.path.join('src/icons', path), 'w') as f:
        f.write(full_content)

# Define path/shapes
icons = {
    # Amber Icons for headers and category sidebar
    'star_amber.svg': ('#e09924', '<path d="M24 6l5.5 11.2L42 19l-9 8.8 2.1 12.2-11.1-5.8-11.1 5.8 2.1-12.2-9-8.8 12.5-1.8z" fill="none" stroke="COLOR" stroke-width="2" stroke-linejoin="round" filter="url(#glow)"/>'),
    'clock_amber.svg': ('#e09924', '<circle cx="24" cy="24" r="18" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><path d="M24 12v12h10" fill="none" stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" filter="url(#glow)"/>'),
    'grid_amber.svg': ('#e09924', '<rect x="8" y="8" width="12" height="12" rx="2" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><rect x="28" y="8" width="12" height="12" rx="2" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><rect x="8" y="28" width="12" height="12" rx="2" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><rect x="28" y="28" width="12" height="12" rx="2" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/>'),
    'list_amber.svg': ('#e09924', '<path d="M14 12h26M14 24h26M14 36h26" fill="none" stroke="COLOR" stroke-width="2.5" stroke-linecap="round" filter="url(#glow)"/><circle cx="8" cy="12" r="1.5" fill="COLOR" filter="url(#glow)"/><circle cx="8" cy="24" r="1.5" fill="COLOR" filter="url(#glow)"/><circle cx="8" cy="36" r="1.5" fill="COLOR" filter="url(#glow)"/>'),
    'globe_amber.svg': ('#e09924', '<circle cx="24" cy="24" r="18" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><path d="M6 24h36M24 6a22 22 0 000 36" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><ellipse cx="24" cy="24" rx="8" ry="18" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/>'),
    'code_amber.svg': ('#e09924', '<path d="M14 16l-8 8 8 8M34 16l8 8-8 8M27 10l-6 28" fill="none" stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" filter="url(#glow)"/>'),
    'music_amber.svg': ('#e09924', '<circle cx="14" cy="36" r="6" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><circle cx="34" cy="32" r="6" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><path d="M20 36V10l20-3v25" fill="none" stroke="COLOR" stroke-width="2" stroke-linejoin="round" filter="url(#glow)"/>'),
    'settings_amber.svg': ('#e09924', '<circle cx="24" cy="24" r="6" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><path d="M24 6l2 4 4.3-.8.8 4.3 4-2-1 4.2 3.1 3.1 4.2-1-2 4.3 4.3.8-.8 4.3 4-2-1 4.2-3.1 3.1-4.2-1-2 4.3-4.3-.8-.8-4.3-4 2 1-4.2-3.1-3.1-4.2 1 2-4.3-4.3-.8.8-4.3-4 2 1-4.2 3.1-3.1 4.2 1 2-4.3z" fill="none" stroke="COLOR" stroke-width="2" stroke-linejoin="round" filter="url(#glow)"/>'),
    'android_amber.svg': ('#e09924', '<path d="M12 24v12a4 4 0 004 4h16a4 4 0 004-4V24M16 20h16a10 10 0 00-20 0z" fill="none" stroke="COLOR" stroke-width="2" stroke-linejoin="round" filter="url(#glow)"/><circle cx="18" cy="16" r="1.5" fill="COLOR" filter="url(#glow)"/><circle cx="30" cy="16" r="1.5" fill="COLOR" filter="url(#glow)"/><path d="M15 10l-3-4M33 10l3-4" fill="none" stroke="COLOR" stroke-width="2" stroke-linecap="round" filter="url(#glow)"/>'),
    'document_amber.svg': ('#e09924', '<path d="M10 6h20l8 8v28a2 2 0 01-2 2H10a2 2 0 01-2-2V8a2 2 0 012-2z" fill="none" stroke="COLOR" stroke-width="2" stroke-linejoin="round" filter="url(#glow)"/><path d="M30 6v8h8M14 18h20M14 26h20M14 34h12" fill="none" stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" filter="url(#glow)"/>'),
    'gamepad_amber.svg': ('#e09924', '<rect x="6" y="14" width="36" height="20" rx="10" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><path d="M16 20v8M12 24h8" stroke="COLOR" stroke-width="2" stroke-linecap="round" filter="url(#glow)"/><circle cx="28" cy="24" r="2" fill="COLOR" filter="url(#glow)"/><circle cx="34" cy="24" r="2" fill="COLOR" filter="url(#glow)"/>'),

    # White App Icons for the main grid
    'terminal.svg': ('#f2e9e1', '<rect x="6" y="8" width="36" height="32" rx="4" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><path d="M14 16l6 6-6 6M22 28h10" fill="none" stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" filter="url(#glow)"/>'),
    'folder.svg': ('#f2e9e1', '<path d="M6 12a2 2 0 012-2h10l4 4h18a2 2 0 012 2v20a2 2 0 01-2 2H8a2 2 0 01-2-2z" fill="none" stroke="COLOR" stroke-width="2" stroke-linejoin="round" filter="url(#glow)"/>'),
    'firefox.svg': ('#f2e9e1', '<circle cx="24" cy="24" r="18" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><path d="M6 24h36M24 6a22 22 0 000 36" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><ellipse cx="24" cy="24" rx="8" ry="18" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/>'),
    'settings.svg': ('#f2e9e1', '<circle cx="24" cy="24" r="6" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><path d="M24 6l2 4 4.3-.8.8 4.3 4-2-1 4.2 3.1 3.1 4.2-1-2 4.3 4.3.8-.8 4.3 4-2-1 4.2-3.1 3.1-4.2-1-2 4.3-4.3-.8-.8-4.3-4 2 1-4.2-3.1-3.1-4.2 1 2-4.3-4.3-.8.8-4.3-4 2 1-4.2 3.1-3.1 4.2 1 2-4.3z" fill="none" stroke="COLOR" stroke-width="2" stroke-linejoin="round" filter="url(#glow)"/>'),
    'code.svg': ('#f2e9e1', '<path d="M14 16l-8 8 8 8M34 16l8 8-8 8M27 10l-6 28" fill="none" stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" filter="url(#glow)"/>'),
    'gamepad.svg': ('#f2e9e1', '<rect x="6" y="14" width="36" height="20" rx="10" fill="none" stroke="COLOR" stroke-width="2" filter="url(#glow)"/><path d="M16 20v8M12 24h8" stroke="COLOR" stroke-width="2" stroke-linecap="round" filter="url(#glow)"/><circle cx="28" cy="24" r="2" fill="COLOR" filter="url(#glow)"/><circle cx="34" cy="24" r="2" fill="COLOR" filter="url(#glow)"/>')
}

for filename, (color, content) in icons.items():
    make_svg(filename, color, content)

print("Icons generated successfully!")
