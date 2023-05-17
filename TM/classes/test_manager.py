from utils.QB.qb_handler import QbHandler


class TestManager:
    # TestManager class constructor
    def __init__(self, questions, curr_question, completed):
        self.questions = questions
        self.current_question = curr_question
        self.max_questions = len(questions)
        self.completed = completed

    # Method to check if the current question number is within bounds
    # of questions - change current question if it is
    def check_question_number(self, current_number):
        if 0 < current_number < self.max_questions:
            self.current_question = current_number
            self.change_question()

    # Method to retrieve information about a specific question using q_id
    def get_question_info(self, q_id):
        question = self.questions[q_id - 1]
        # Initialise new instance of QbHandler
        qb_handler = QbHandler()
        qbs = qb_handler.qbs.items()
        qb_list = [(qb[0], qb[1]) for qb in qbs]
        # Use language field of question to address correct QB
        qb_addr = None
        for qb in qb_list:
            if qb[1] == question["language"]:
                qb_addr = qb[0]
                break

        if qb is None:
            return {}
        # Request question information from QB using handler
        question_info = qb_handler.get_question(qb_addr, question["q_id"])

        return question_info

    def check_answer(self, answer_index, qid):
        for q in self.questions:
            if q["qid"] == qid:
                # Answer is correct - update mark based on attempts left
                if answer_index == self.answer and q["attempt"] > 0:
                    q["attempt"] -= 1
                    q["mark"] = q["attempt"] + 1
                    return {
                        "correct": True,
                        "attempts": q["attempt"],
                        "mark": q["mark"],
                    }
                else:
                    # Answer is incorrect, but there are still attempts left
                    if q["attempt"] > 0:
                        # Decrement attempts by 1
                        q["attempt"] -= 1
                    return {"correct": False, "attempts": q["attempt"]}
        # Answer is incorrect - no attempts left!
        return {"correct": False, "attempts": 0}

    # dummy function until QB exists
    def change_question(self):
        new_question = self.dummy_questions[self.question_number - 1]
        self.question = new_question["question"]
        if "choices" in new_question:
            self.choices = new_question["choices"]
        elif "language" in new_question:
            self.choices = []
            self.language = new_question["language"]
        self.type = new_question["type"]
        self.answer = new_question["answer"]
        print(self.language)
