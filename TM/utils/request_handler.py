import json
import os
from http.server import BaseHTTPRequestHandler

from routes import *


class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        routes = BaseRoute.routes

        # set path to index if path is "/"
        path = self.path[1:] if self.path[1:] != "" else "index"
        status = 200
        content_type = "text/html"
        response = None

        if path.startswith("static/"):
            if os.path.exists(path):
                with open(path, "rb") as file:
                    response = file.read()

        elif path.startswith("api/") and path in routes.keys():
            content_type = "application/json"
            status = routes[path](path)[0]
            response = json.dumps(routes[path](path)[1]).encode()

        elif path in routes.keys():
            response = routes[path](path).encode()

        if response is None:
            status = 404
            response = b"404"

        self.send_response(status)
        self.send_header("Content-type", content_type)
        self.end_headers()

        self.wfile.write(response)
