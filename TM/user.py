# Global dictionary to store logged in users
users = {}

class User:
    def __init__(self):
        self.username = None
        self.session_id = None

    def get_username(self):
        return self.username
    
    def get_session_id(self):
        return self.session_id
