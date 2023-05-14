# Global dictionary to store users
import hashlib
from classes import TestManager

users = {}


class User(TestManager):
    def __init__(self, username, session_id):
        self.username = username
        self.session_id =  session_id
        super().__init__([], -1, True)
        
    def init_tm(self, questions, curr_question, completed):
        super().__init__(questions, curr_question, completed)

    def get_username(self):
        return self.username

    def get_session_id(self):
        return self.session_id
    