from http.server import BaseHTTPRequestHandler
import os
from routes import *


class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        routes = BaseRoute.routes

        path = self.path[1:] if self.path[1:] != "" else "index"

        response = None

        if path.startswith("static/"):
            if not os.path.exists(path):
                response = None
            else:
                with open(path, "rb") as file:
                    response = file.read()

        if path in routes.keys():
            response = routes[path](path).encode()

        if response is None:
            self.send_response(404)
            response = b"404"
        else:
            self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

        self.wfile.write(response)
