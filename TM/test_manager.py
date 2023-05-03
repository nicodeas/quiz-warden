import json


class TestManager:
    def __init__(self):
        self.max_questions = 0
        self.question = ""
        self.choices = []
        self.question_number = 1
        self.questions = []

        # dummy variables until QB exists
        self.dummy_questions = []
        self.answer = -1

        with open("data/questions.json", "r") as f:
            self.dummy_questions = json.load(f)["questions"]
            self.max_questions = len(self.dummy_questions)
        self.construct_attempts()

    def check_question_number(self, current_number):
        if 0 < current_number < self.max_questions:
            self.question_number = current_number
            self.change_question()

    def get_question_info(self):
        return {"question": self.question, "choices": self.choices}
    
    # Dummy function 
    # self.questions needs to be constructed when we get the list of question IDs from the QB
    # this code is used to generate ids (since i cannot get them until QB)
    def construct_attempts(self):
        for i in range(0,self.max_questions+1):
            self.questions.append({"qid": i+1, "attempt": 3, "mark": 0})



    def check_answer(self, answer_index, qid):
        for q in self.questions:
            if q['qid'] == qid:
                if answer_index == self.answer:
                    if q['attempt'] == 3:
                        q['mark'] = q['attempt']
                        q['attempt'] -= 1
                    elif q['attempt'] == 2:
                        q['mark'] = q['attempt']
                        q['attempt'] -= 1
                    elif q['attempt'] == 1:
                        q['mark'] = q['attempt']
                        q['attempt'] -= 1
                    else:
                        return {'correct': False, 'attempts': q['attempt'] }
                else:
                    if q['attempt'] > 0:
                        q['attempt'] -= 1
                    return {'correct': False, 'attempts': q['attempt'] }
                break
        return {'correct': True, 'attempts': q['attempt'], 'mark': q['mark'] }

    # dummy function until QB exists
    def change_question(self):
        question_index = self.question_number - 1
        self.question = self.dummy_questions[question_index]["question"]
        self.choices = self.dummy_questions[question_index]["choices"]
        self.answer = int(self.dummy_questions[question_index]["answer"])


test_manager = TestManager()
