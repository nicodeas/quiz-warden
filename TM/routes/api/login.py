import json
import os

from classes.user import User, users
from utils.auth import generate_session_id, login_user

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
            if session_id not in users:
                user = User(username, session_id)
                users[session_id] = user

                data = {}
                if os.path.exists("data/session.json"):
                    with open("data/session.json", "r") as f:
                        data = json.load(f)
                if session_id not in data:
                    data[session_id] = {}
                else:
                    user.init_tm(
                        data[session_id]["questions"],
                        data[session_id]["current_question"],
                        data[session_id]["completed"],
                    )
                with open("data/session.json", "w") as f:
                    json.dump(data, f, indent=2)

            cookie = f"session_token={session_id}; Path=/; Max-Age=99999999;"
            headers = {"Set-Cookie": cookie}
            status = 200
            message = "Success"

        return status, {"message": message}, headers
