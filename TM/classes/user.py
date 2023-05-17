# Global dictionary to store users
from classes import TestManager

users = {}


class User(TestManager):
    # User class constructor
    def __init__(self, username, session_id):
        self.username = username
        self.session_id = session_id
        super().__init__([], -1, True)
    # Initialise TestManager instance variables
    def init_tm(self, questions, curr_question, completed):
        super().__init__(questions, curr_question, completed)
    # Accessor methods
    def get_username(self):
        return self.username

    def get_session_id(self):
        return self.session_id
