import json


class TestManager:
    def __init__(self):
        self.max_questions = 0
        self.question = ""
        self.choices = []
        self.question_number = 1
        self.questions = []
        self.type = None
        self.language = None
        self.question_info = {}

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
        if(self.choices):
            return {"question": self.question, "choices": self.choices, "type": self.type}
        return {"question": self.question,  "type": self.type, "language": self.language}
    
    # Dummy function 
    # self.questions needs to be constructed when we get the list of question IDs from the QB
    # this code is used to generate ids (since i cannot get them until QB)
    def construct_attempts(self):
        for i in range(0,self.max_questions+1):
            self.questions.append({"qid": i+1, "attempt": 3, "mark": 0})


    def check_answer(self, answer_index, qid):
        for q in self.questions:
            if q['qid'] == qid:
                if answer_index == self.answer and q['attempt'] > 0:
                    q['attempt'] -= 1
                    q['mark'] = q['attempt'] + 1
                    return {'correct': True, 'attempts': q['attempt'], 'mark': q['mark']}
                else:
                    if q['attempt'] > 0:
                        q['attempt'] -= 1
                    return {'correct': False, 'attempts': q['attempt']}
        return {'correct': False, 'attempts': 0}

    # dummy function until QB exists
    def change_question(self):
        new_question = self.dummy_questions[self.question_number - 1]
        self.question = new_question["question"]
        if 'choices' in new_question:
            self.choices = new_question["choices"]
        elif 'language' in new_question:
            self.choices = []
            self.language = new_question['language']
        self.type = new_question['type']
        self.answer = new_question["answer"]
        print(self.language)


test_manager = TestManager()
