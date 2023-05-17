from utils.QB.qb_handler import QbHandler


class TestManager:
    def __init__(self, questions, curr_question, completed):
        self.questions = questions
        self.current_question = curr_question
        self.max_questions = len(questions)
        self.completed = completed

    def check_question_number(self, current_number):
        if 0 < current_number < self.max_questions:
            self.current_question = current_number
            self.change_question()

    def get_question_info(self, q_id):
        question = self.questions[q_id - 1]
        qb_handler = QbHandler()
        qbs = qb_handler.qbs.items()
        qb_list = [(qb[0], qb[1]) for qb in qbs]

        qb_addr = None
        for qb in qb_list:
            if qb[1] == question["language"]:
                qb_addr = qb[0]
                break

        if qb is None:
            return {}

        question_info = qb_handler.get_question(qb_addr, question["q_id"])

        return question_info

    def check_answer(self, answer_index, qid):
        for q in self.questions:
            if q["qid"] == qid:
                if answer_index == self.answer and q["attempt"] > 0:
                    q["attempt"] -= 1
                    q["mark"] = q["attempt"] + 1
                    return {
                        "correct": True,
                        "attempts": q["attempt"],
                        "mark": q["mark"],
                    }
                else:
                    if q["attempt"] > 0:
                        q["attempt"] -= 1
                    return {"correct": False, "attempts": q["attempt"]}
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
