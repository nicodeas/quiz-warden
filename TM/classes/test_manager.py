from utils.QB.qb_handler import QbHandler


class TestManager:
    # TestManager class constructor
    def __init__(self, questions, curr_question, completed):
        self.questions = questions
        self.current_question = curr_question
        self.max_questions = len(questions)
        self.completed = completed

    def get_question_info(self, q_number):
        question = self.questions[q_number - 1]
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
        question_info["attempts"] = self.questions[self.current_question - 1][
            "attempts"
        ]
        question_info["correct"] = self.questions[self.current_question - 1]["correct"]
        return question_info

    def mark_question(self, q_number, answer):
        question = self.questions[q_number - 1]
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

        is_correct = qb_handler.mark_question(qb_addr, question["q_id"], answer)
        return is_correct
