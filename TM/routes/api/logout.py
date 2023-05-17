from utils.auth import logout_user

from ..base import BaseRoute


class Logout(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        # Get session_token from request headers
        cookies = req.headers["Cookie"]
        session_token = cookies.split("=")[1]

        # Default vlues for status, message, and headers
        status = 500
        message = "Failed to logout"
        headers = {}

        if logout_user(session_token):
            status = 200
            message = "Success"
            # Remove session_id cookie from header
            headers = {
                "Set-Cookie": "session_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT"
            }

        return status, {"message": message}, headers
