#!/usr/bin/env bash
set -e

# 0) Give the desktop a moment
sleep 1

# 1) Wake the LCD by opening a terminal
lxterminal --title="LCD init" -e bash -lc 'echo "Initializing LCD..."; sleep 2; exec bash' &

# 2)  Start IP server (stop any previous one first)
pkill -f kiosk-ip-server.py 2>/dev/null || true
/home/Raspinator/bin/kiosk-ip-server.py &


sleep 2

# 3) Pick Chromium binary
if command -v chromium-browser >/dev/null 2>&1; then
  BROWSER="chromium-browser"
else
  BROWSER="chromium"
fi



# 4) Light cleanup so we always get a fresh window

pkill -9 chromium chromium-browser 2>/dev/null || true
rm -f /home/Raspinator/.config/chromium/Singleton* 2>/dev/null || true

# 5) Launch EXACTLY like before (no extra display/GPU flags)
URL="file:///home/Raspinator/kiosk/index.html"
"$BROWSER" --kiosk --incognito --noerrdialogs --disable-infobars \
  --password-store=basic \
  --check-for-update-interval=31536000 "$URL" >/dev/null 2>&1 &
#set -e
#
## Allow desktop to initialize
#sleep 1
#
## Wake the LCD by opening a terminal
#lxterminal --title="LCD init" -e bash -lc 'echo "Initializing LCD..."; sleep 2; exec bash' &
#
## Start the exit server
#/home/Raspinator/bin/kiosk-exit-server.py &
#
#sleep 2
#
## Clean up singleton files for some reason
#rm -f /home/Raspinator/.config/chromium/Singleton* 2>/dev/null
## Launch Chromium in kiosk mode with our page
#URL="file:///home/Raspinator/kiosk/index.html"
#if command -v chromium-browser >/dev/null 2>&1; then
#  BROWSER="chromium-browser"
#else
#  BROWSER="chromium"
#fi
#
#$BROWSER --kiosk --incognito --noerrdialogs --disable-infobars \
#  --check-for-update-interval=31536000 "$URL" >/dev/null 2>&1 &
#