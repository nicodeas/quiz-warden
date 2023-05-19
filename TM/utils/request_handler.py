import json
import os
from http.server import BaseHTTPRequestHandler
from urllib.parse import parse_qs, urlparse

from routes import *
from utils.auth import is_valid_session


class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        routes = BaseRoute.routes

        url = urlparse(self.path)
        qs = parse_qs(url.query)
        path = url.path[1:]
        status = 404
        content_type = "text/html"
        response = "404 Page not found"
        headers = {}

        if path == "login" and "Cookie" in self.headers:
            cookies = self.headers["Cookie"]
            session_token = cookies.split("=")[1]

            if is_valid_session(session_token):
                # User is authenticated, redirect to index page
                self.send_response(302)
                self.send_header("Location", "/")
                self.end_headers()
                return

        # Check if user is authenticated
        if path != "login" and not path.startswith("static"):
            session_token = None
            if "Cookie" in self.headers:
                cookies = self.headers["Cookie"]
                session_token = cookies.split("=")[1]

            if not is_valid_session(session_token):
                # User is not authenticated, redirect to login page
                self.send_response(302)
                self.send_header("Location", "/login")
                self.end_headers()
                return

        if path.startswith("static/") and os.path.exists(path):
            if path.endswith(".css"):
                content_type = "text/css"
            elif path.endswith(".js"):
                content_type = "text/javascript"
            elif path.endswith(".png"):
                content_type = "image/png"

            with open(path, "rb") as file:
                status = 200
                response = file.read()

        elif path in routes.keys():
            status, response, headers = routes[path](self, path, qs)

        if path.startswith("api/"):
            content_type = "application/json"
            if status == 404:
                response = {"message": "Not found"}
            response = json.dumps(response)

        self.send_response(status)
        self.send_header("Content-type", content_type)

        for header, value in headers.items():
            self.send_header(header, value)

        self.end_headers()

        if isinstance(response, str):
            response = response.encode()

        self.wfile.write(response)

    def do_POST(self):
        routes = BaseRoute.routes

        url = urlparse(self.path)
        qs = parse_qs(url.query)
        path = url.path[1:]
        status = 404
        content_type = "text/html"
        response = "404 Page not found"
        headers = {}

        if path in routes.keys():
            content_type = "application/json"
            content_length = int(self.headers["Content-Length"])
            post_data = self.rfile.read(content_length)
            post_data = json.loads(post_data)
            status, response, headers = routes[path](
                self, path, qs, post_data=post_data
            )

        if path.startswith("api/"):
            content_type = "application/json"
            if status == 404:
                response = {"message": "Not found"}
            response = json.dumps(response)

        self.send_response(status)
        self.send_header("Content-type", content_type)

        for header, value in headers.items():
            self.send_header(header, value)

        self.end_headers()
        self.wfile.write(response.encode())
