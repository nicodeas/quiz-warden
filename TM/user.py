import random


# Global dictionary to store users
users = {}

class User:
    def __init__(self):
        self.username = None
        self.session_id = None
        self.question_order = []

    def get_username(self):
        return self.username
    
    def get_session_id(self):
        return self.session_id
    
    def get_session_order(self):
        return self.question_order
    
    def seed_question_order(self):
        random.seed(self.get_session_id())
        questions = set()
        order = []
        
        # TODO: Change this to suit question generation
        # i.e. select from different banks of questions
        while len(order) < 10:
            question = random.randint(0,9)
            if question not in questions:
                questions.add(question)
                order.append(question)
        self.question_order = order