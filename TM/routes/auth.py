import hashlib
from user import User, users

# Verifies user credentials exist among pre-registered users
def login_user(username, password):
    # TODO: update this to traverse and verify against pre-registered users
    if (username == 'admin' and password == 'admin') or (username == 'jack' and password == 'jack'):
        return True
    return False

# Hashes username to serve as session_id for user
def generate_session_id(username):
    session_id = hashlib.sha256(username.encode()).hexdigest()
    return session_id

# Checks that session_token in header is same as session_id in User class
def is_valid_session(session_token):
    return session_token in users
