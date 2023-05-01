import hashlib

def login_user(username, password):
    # TODO: update this to traverse and verify against pre-registered users
    if username == 'admin' and password == 'admin':
        return True
    return False

def new_session(username):
    session_id = hashlib.sha256(username.encode()).hexdigest()
    return session_id

def is_valid_session(session_token):
    # TODO: this should check against a list of valid cookies
    return True