from utils.html_reader import html_reader
from urllib.parse import parse_qs

from user import *
from .base import BaseRoute
from utils.auth import *


class Login(BaseRoute):
    def executor(req, path, qs, *args, **kwargs):
        if req.command == 'GET':
            return html_reader(path)
        elif req.command == 'POST':
            # Extract username and password from request
            content_length = int(req.headers['Content-Length'])
            post_data = req.rfile.read(content_length)
            data = parse_qs(post_data.decode('utf-8'))
            
            username = data.get('username', [''])[0]
            password = data.get('password', [''])[0]

            if login_user(username, password):
                session_id = generate_session_id(username)
                # Generate new session
                if session_id not in users:
                    user = User()
                    user.username = username
                    user.session_id = session_id
                    users[session_id] = user
                # Add session_id to header and redirect to index
                req.send_response(302)
                req.send_header('Location', '/')
                req.send_header('Set-Cookie', f'session_token={session_id}')
                req.end_headers()
            else:
                # Login failed!
                req.send_response(302)
                req.send_header('Location', '/login')
                req.end_headers()
        return html_reader(path)