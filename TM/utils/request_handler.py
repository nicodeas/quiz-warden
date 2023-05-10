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

        # Check if user is authenticated
        if path != 'login':
            if 'Cookie' in self.headers:
                cookies = self.headers['Cookie']
                session_token = cookies.split('=')[1]

                if is_valid_session(session_token):
                    # User is authenticated, continue with routing
                    pass
                else:
                    # User is not authenticated, redirect to login page
                    self.send_response(302)
                    self.send_header('Location', '/login')
                    self.end_headers()
                    return
            else:
                # No session token found, redirect to login page
                self.send_response(302)
                self.send_header('Location', '/login')
                self.end_headers()
                return

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