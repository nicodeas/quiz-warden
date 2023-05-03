from utils.html_reader import html_reader
from .base import BaseRoute
from user import users


class Logout(BaseRoute):
    def executor(req, path, qs, *args, **kwargs):
        # Remove session_id cookie from header
        req.send_response(302)
        req.send_header('Location', '/login')
        req.send_header('Set-Cookie',
                        'session_token=; expires=Thu, 01 Jan 1970 00:00:00 GMT')
        req.end_headers()
        return html_reader(path)