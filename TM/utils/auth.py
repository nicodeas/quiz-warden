import hashlib
import json

from classes.user import users


# Verifies user credentials exist against pre-registered users
def login_user(username, password):
    with open("data/users.json", "r") as f:
        data = json.load(f)
        for user in data["users"]:
            if user["username"] == username and user["password"] == password:
                return True
        return False


# Hashes username to serve as session_id for user
def generate_session_id(username):
    session_id = hashlib.sha256(username.encode()).hexdigest()
    return session_id


# Checks that session_token in header is same as session_id in User class
def is_valid_session(session_token):
    if session_token in users:
        print("accessing page as " + users[session_token].username)
    return session_token in users


# Logs out user by removing session_token from users dict
def logout_user(session_token):
    if session_token in users:
        users.pop(session_token)
        return True
    return False
