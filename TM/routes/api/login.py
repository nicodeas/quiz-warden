import json
import os

from classes.user import User, users
from utils.auth import generate_session_id, login_user

from ..base import BaseRoute


class Login(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        # Default values for status, message, and headers
        status = 401
        message = "Unauthorized"
        headers = {}

        # Extract username and password from request
        post_data = kwargs.get("post_data")
        username = post_data.get("username", "")
        password = post_data.get("password", "")

        # Check if login is successful
        if not login_user(username, password):
            return status, {"message": message}, headers

        # Success! Generate session_id and create new User instance
        # (if it doesn't already exist)
        session_id = generate_session_id(username)
        if session_id not in users:
            user = User(username, session_id)
            users[session_id] = user

            # Load session data from JSON file if it exists
            data = {}
            if os.path.exists("data/session.json"):
                with open("data/session.json", "r") as f:
                    data = json.load(f)

            if session_id not in data or data.get(session_id) == {}:
                # Initialise session data as an empty dict
                data[session_id] = {}
            else:
                # Session data exists for this user - retrive data for their session
                user.init_tm(
                    data[session_id]["questions"],
                    data[session_id]["current_question"],
                    data[session_id]["completed"],
                )
            # Save updated session data to JSON file
            with open("data/session.json", "w") as f:
                json.dump(data, f, indent=2)

        # Set cookie header with generated session_id
        cookie = f"session_token={session_id}; Path=/; Max-Age=99999999;"
        headers = {"Set-Cookie": cookie}
        # Set status and message to indicate successful login
        status = 200
        message = "Success"

        return status, {"message": message}, headers
