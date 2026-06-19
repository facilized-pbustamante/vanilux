CXX = g++
VERSION = $(shell cat VERSION 2>/dev/null || echo 0.1)
CXXFLAGS = -O2 -g -std=c++17 -Wall -Wextra -DVLX_VERSION='"$(VERSION)"'

# Separate compile & link flags for GTKMM
GTK_CFLAGS = $(shell pkg-config --cflags gtkmm-3.0)
GTK_LIBS = $(shell pkg-config --libs gtkmm-3.0)

SRC_FILES = src/main.cpp src/app_discovery.cpp src/launcher_window.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)
TARGET = vanilux
TEST_TARGET = test-discovery

# Install prefix. Override with: sudo make install PREFIX=/usr
PREFIX ?= /usr/local
BINDIR  = $(DESTDIR)$(PREFIX)/bin
SHAREDIR = $(DESTDIR)$(PREFIX)/share/vanilux

.PHONY: all test clean install uninstall dist

all: $(TARGET)

# Link step
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(TARGET) $(GTK_LIBS)

# Compile step
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(GTK_CFLAGS) -c $< -o $@

test: tests/test_discovery.cpp src/app_discovery.cpp
	$(CXX) $(CXXFLAGS) $(GTK_CFLAGS) tests/test_discovery.cpp src/app_discovery.cpp -o $(TEST_TARGET) $(GTK_LIBS)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(OBJ_FILES) tests/*.o

# Install binary + CSS + icons, then kill any resident instance so the next
# F4 launches the new binary. Needs root for /usr/local (use: sudo make install).
install: all
	install -Dm755 $(TARGET) $(BINDIR)/$(TARGET)
	install -Dm644 src/style.css $(SHAREDIR)/style.css
	mkdir -p $(SHAREDIR)/icons
	install -m644 src/icons/*.svg $(SHAREDIR)/icons/
	-killall $(TARGET) 2>/dev/null

uninstall:
	rm -f $(BINDIR)/$(TARGET)
	rm -rf $(SHAREDIR)
	-killall $(TARGET) 2>/dev/null

# Build a portable release tarball: vanilux-<VERSION>.tar.gz
DISTNAME = $(TARGET)-$(VERSION)
dist: clean
	rm -rf /tmp/$(DISTNAME) $(DISTNAME).tar.gz
	mkdir -p /tmp/$(DISTNAME)
	cp -r Makefile README.md VERSION install.sh uninstall.sh \
	      vanilux.desktop src tests docs /tmp/$(DISTNAME)/ 2>/dev/null || true
	tar -C /tmp -czf $(DISTNAME).tar.gz $(DISTNAME)
	rm -rf /tmp/$(DISTNAME)
	@echo "Release listo: $(DISTNAME).tar.gz"
