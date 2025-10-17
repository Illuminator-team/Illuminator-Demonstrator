# LCD auto display

### Install chromium-browser and lxterminal

```bash
sudo apt update
sudo apt install -y chromium-browser lxterminal
```

### To check IP

```bash
hostname -I | awk '{print $1}'
```

# Move Scripts

Paste the kiosk and bin folders on the Raspinator dir. 
Paste the kiosk.desktop script under .config/autostart. If the autostart folder does not exist, create it.

Store the image you wish to be displayed in your html under Pictures or any dir of your preference.

Make sure to update the img source in the index.html file to match the location of the image you wish to display.

# After Moving Scripts

### Fix script characters in case they got messed up by windows

```bash
sed -i 's/\r$//' /home/Raspinator/bin/kiosk-ip-server.py
sed -i 's/\r$//' /home/Raspinator/bin/start-kiosk.sh
```

### Make scripts executable

```bash
chmod +x /home/Raspinator/bin/start-kiosk.sh
chmod +x /home/Raspinator/bin/kiosk-ip-server.py
```

### Check manually

```bash
/home/Raspinator/bin/start-kiosk.sh
```

### Then reboot