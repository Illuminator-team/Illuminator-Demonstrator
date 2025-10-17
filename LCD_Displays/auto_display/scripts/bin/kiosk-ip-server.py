#!/usr/bin/python3
from http.server import BaseHTTPRequestHandler, HTTPServer
import socket, fcntl, struct, os, json

# Return IPv4 for an interface or None (works on Linux)
def iface_ipv4(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        return socket.inet_ntoa(
            fcntl.ioctl(
                s.fileno(),
                0x8915,  # SIOCGIFADDR
                struct.pack('256s', ifname[:15].encode('utf-8'))
            )[20:24]
        )
    except OSError:
        return None
    finally:
        s.close()

def list_ifaces():
    try:
        return [n for n in os.listdir('/sys/class/net') if os.path.isdir('/sys/class/net/' + n)]
    except Exception:
        return ['wlan0', 'eth0', 'usb0', 'enx0']

def get_ip():
    # Prefer Wi-Fi/eth first
    prefs = ['wlan0', 'eth0', 'enx', 'usb0']
    ifaces = list_ifaces()

    def score(name):
        for i, p in enumerate(prefs):
            if name == p or name.startswith(p):
                return i
        return 100
    ifaces.sort(key=score)

    for name in ifaces:
        if name == 'lo':
            continue
        ip = iface_ipv4(name)
        if ip and not ip.startswith('127.'):
            return ip

    # Fallback: routing trick
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip
    except Exception:
        return "unknown"

class Handler(BaseHTTPRequestHandler):
    def _cors(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.send_header("Cache-Control", "no-store")

    def do_OPTIONS(self):
        self.send_response(204); self._cors(); self.end_headers()

    def do_GET(self):
        if self.path == "/ip":
            body = json.dumps({"ip": get_ip()}).encode("utf-8")
            self.send_response(200); self._cors()
            self.send_header("Content-Type", "application/json")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            try:
                self.wfile.write(body)
            except BrokenPipeError:
                pass
        else:
            self.send_response(404); self._cors(); self.end_headers()

    def log_message(self, *args):
        return

if __name__ == "__main__":
    print("IP server listening on http://localhost:8080")
    HTTPServer(("localhost", 8080), Handler).serve_forever()
