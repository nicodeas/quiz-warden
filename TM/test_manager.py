import json

class TestManager():
    def __init__(self):
        self.max_questions = 0
        self.question = ""
        self.choices = []
        self.question_number = 1
        
        # dummy variables until QB exists
        self.questions = []
        self.answer = -1

        with open('data/questions.json', 'r') as f:
            self.questions = json.load(f)['questions']
            self.max_questions = len(self.questions)

    def check_question_number(self, current_number):
        if 0 < current_number < test_manager.max_questions:
            self.question_number = current_number
            self.change_question()
    
    def get_question_info(self):
        return {"question": self.question, "choices": self.choices}
    
    def check_answer(self, answer_index):
        return answer_index == self.answer
    
    # dummy function until QB exists
    def change_question(self):
        question_index = self.question_number - 1
        self.question = self.questions[question_index]['question']
        self.choices = self.questions[question_index]['choices']
        self.answer = int(self.questions[question_index]['answer'])


test_manager = TestManager()