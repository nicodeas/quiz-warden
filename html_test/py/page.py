from http.server import BaseHTTPRequestHandler, HTTPServer
import requests

class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            with open('index.html', 'r') as f:
                self.wfile.write(f.read().encode())
        elif self.path == '/file.txt':
            # Send request to C server for file contents
            c_server_response = requests.get('http://localhost:8080/file.txt')
            if c_server_response.status_code == 200:
                self.send_response(200)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write(c_server_response.text.encode())
            else:
                self.send_error(404, 'File not found')
        else:
            self.send_error(404, 'File not found')

def run(server_class=HTTPServer, handler_class=MyHandler):
    server_address = ('', 8000)
    httpd = server_class(server_address, handler_class)
    print('Starting httpd...')
    httpd.serve_forever()

if __name__ == '__main__':
    run()
