# Global dictionary to store users
import os
from classes import TestManager
import json

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
    
    @staticmethod
    def dump_sessions():
        data = {}
        for session_id, user in users.items():
            data[session_id] = {"questions": {}, "current_question": user.current_question, "completed": user.completed}
            data[session_id] = {
                "questions": user.questions,
                "current_question": user.current_question,
                "completed": user.completed,
            }
        with open("data/session.json", "w") as f:
            json.dump(data, f, indent=2
        )

    def load_session(self):
        data = {}
        if os.path.exists("data/session.json"):
            with open("data/session.json", "r") as f:
                data = json.load(f)

        if self.session_id in data:
            self.init_tm(
                data[self.session_id]["questions"],
                data[self.session_id]["current_question"],
                data[self.session_id]["completed"],
            )