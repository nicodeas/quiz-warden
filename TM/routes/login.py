from utils.html_reader import html_reader
from urllib.parse import parse_qs

from .base import BaseRoute
from .auth import *


class Login(BaseRoute):
    def executor(req, path, qs, *args, **kwargs):
        if req.command == 'GET':
            return html_reader(path)
        elif req.command == 'POST':
            # Read request
            content_length = int(req.headers['Content-Length'])
            post_data = req.rfile.read(content_length)
            data = parse_qs(post_data.decode('utf-8'))
            # Extract username and password from request
            # TODO: account for empty username/password field
            username = data['username'][0]
            password = data['password'][0]

            if login_user(username, password):
                # Add session_id to header and redirect to index
                session_id = new_session(username)
                req.send_response(302)
                req.send_header('Location', '/')
                # No set expiration; cookie will last for duration of browser
                req.send_header('Set-Cookie', 'session_token={}'.format(session_id))
                req.end_headers()
            else:
                # Login failed!
                req.send_response(302)
                req.send_header('Location', '/login')
                req.end_headers()
        return html_reader(path)