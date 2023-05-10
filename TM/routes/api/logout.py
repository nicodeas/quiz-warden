from ..base import BaseRoute


class Logout(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        # Remove session_id cookie from header
        headers ={"Set-Cookie": "session_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT"}
        return 200, {"message": "Success"}, headers
