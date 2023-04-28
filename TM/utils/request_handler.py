from http.server import BaseHTTPRequestHandler
from TM.routes import *


class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

        routes = BaseRoute.routes

        path = self.path[1:] if self.path[1:] != "" else "index"

        if path in routes.keys():
            self.wfile.write(routes[path](path).encode())
