import json
import os
from http.server import BaseHTTPRequestHandler
from urllib.parse import parse_qs, urlparse

from routes import *


class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        routes = BaseRoute.routes

        url = urlparse(self.path)
        qs = parse_qs(url.query)
        path = url.path[1:]
        status = 404
        content_type = "text/html"
        response = "404 Page not found"

        if path.startswith("static/") and os.path.exists(path):
            if path.endswith(".css"):
                content_type = "text/css"
            elif path.endswith(".js"):
                content_type = "text/javascript"

            with open(path, "r") as file:
                status = 200
                response = file.read()

        elif path in routes.keys():
            status, response = routes[path](self, path, qs)

        if path.startswith("api/"):
            content_type = "application/json"
            if status == 404:
                response = {"message": "Not found"}
            response = json.dumps(response)

        self.send_response(status)
        self.send_header("Content-type", content_type)
        self.end_headers()
        self.wfile.write(response.encode())

    def do_POST(self):
        routes = BaseRoute.routes

        url = urlparse(self.path)
        path = url.path[1:]
        status = 404
        content_type = "text/html"
        response = "404 Page not found"

        if path in routes.keys():
            content_type = "application/json"
            content_length = int(self.headers["Content-Length"])
            post_data = self.rfile.read(content_length)
            post_data = json.loads(post_data)
            status, response = routes[path](self, path, post_data)

        if path.startswith("api/"):
            content_type = "application/json"
            if status == 404:
                response = {"message": "Not found"}
            response = json.dumps(response)

        self.send_response(status)
        self.send_header("Content-type", content_type)
        self.end_headers()
        self.wfile.write(response.encode())
