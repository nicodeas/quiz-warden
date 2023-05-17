import random

from classes.user import User, users
from utils.QB.qb_handler import QbHandler

from ..base import BaseRoute


class GenerateQuiz(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        status = 200
        message = "Success"
        headers = {}

        NUM_QUESTIONS = 3

        session_id = req.headers["Cookie"].split("=")[1]
        user: User = users[session_id]
        if not user.completed:
            status = 400
            message = "A quiz has been generated already."
            return status, {"message": message}, headers

        qb_handler = QbHandler()
        qbs = qb_handler.qbs.items()
        qb_list = [(qb[0], qb[1]) for qb in qbs]

        if len(qbs) == 0:
            status = 500
            message = "No question banks loaded"
            return status, {"message": message}, headers

        elif len(qbs) == 1:
            # tuple (q_id, language)
            questions = [
                (q, qb_list[0][1])
                for q in qb_handler.generate_quiz(qb_list[0][0], NUM_QUESTIONS)
            ]

        elif len(qbs) == 2:
            num_questions = random.randint(1, NUM_QUESTIONS)
            # tuple (q_id, language)
            questions = [
                (q, qb_list[0][1])
                for q in qb_handler.generate_quiz(qb_list[0][0], num_questions)
            ]

            num_questions = NUM_QUESTIONS - num_questions
            # tuple (q_id, language)
            questions += [
                (q, qb_list[1][1])
                for q in qb_handler.generate_quiz(qb_list[1][0], num_questions)
            ]

        question_data = [
            {"q_id": q, "attempts": 1, "correct": None, "language": l}
            for q, l in questions
        ]
        user.init_tm(question_data, 1, False)
        
        user.dump_sessions()

        return status, {"message": message}, headers
