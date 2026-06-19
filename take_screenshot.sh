#!/bin/bash
export DISPLAY=:0.0

# Clean up any existing instances forcefully
killall -9 vanilux 2>/dev/null
rm -f /tmp/vanilux.pid
sleep 1.0

# Start launchpad in background
/usr/local/bin/vanilux &
LP_PID=$!

# Wait for it to map on the screen
sleep 2.5

# Raise the window to ensure it is visible and focused
wmctrl -a vanilux || true
sleep 0.5

# Capture the screenshot of the root window
import -window root /home/koe/.gemini/antigravity-ide/brain/585c5938-3732-408d-a339-d8983dfe4490/screenshot_launchpad.png

# Clean up launchpad process
kill -9 $LP_PID 2>/dev/null
killall -9 vanilux 2>/dev/null
rm -f /tmp/vanilux.pid
echo "Screenshot captured successfully!"
