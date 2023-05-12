import os
from classes.user import *
from utils.auth import *

from ..base import BaseRoute


class Login(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        status = 401
        message = "Unauthorized"
        headers = {}

        # Extract username and password from request
        post_data = kwargs.get("post_data")
        username = post_data.get("username", "")
        password = post_data.get("password", "")

        if login_user(username, password):
            session_id = generate_session_id(username)
            # Generate new session
            if session_id not in users:
                user = User()
                user.username = username
                user.session_id = session_id
                users[session_id] = user

                data = {}
                if os.path.exists("data/session.json"):
                    with open("data/session.json", "r") as f:
                        data = json.load(f)

                if session_id not in data:
                    data[session_id] = []

                with open("data/session.json", "w") as f:
                    json.dump(data, f, indent=2)
            # Add session_id to header and redirect to index

            cookie = f"session_token={session_id}; Path=/; Max-Age=99999999;"
            headers = {"Set-Cookie": cookie}
            status = 200
            message = "Success"

        return status, {"message": message}, headers
