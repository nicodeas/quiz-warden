import random


# Global dictionary to store users
users = {}

class User:
    def __init__(self):
        self.username = None
        self.session_id = None
        self.questions = []

    def get_username(self):
        return self.username
    
    def get_session_id(self):
        return self.session_id