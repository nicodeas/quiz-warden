import random

from classes.user import *
from classes.user import User, users
from utils.auth import *
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

        num_questions = random.randint(1, NUM_QUESTIONS)
        # tuple (q_id, language)
        questions = [
            (q, qb_list[0][1])
            for q in qb_handler.generate_quiz(qb_list[0][0], num_questions)
        ]

        if len(qbs) == 2:
            num_questions = NUM_QUESTIONS - num_questions
            # tuple (q_id, language)
            questions += [
                (q, qb_list[1][1])
                for q in qb_handler.generate_quiz(qb_list[1][0], num_questions)
            ]

        session = {}
        with open("data/session.json", "r") as f:
            session = json.load(f)

        question_data = [
            {"q_id": q, "attempts": 0, "correct": None, "language": l}
            for q, l in questions
        ]

        session[session_id] = {}
        session[session_id]["questions"] = question_data
        session[session_id]["current_question"] = 1
        session[session_id]["completed"] = False

        with open("data/session.json", "w") as f:
            json.dump(session, f, indent=2)

        user.init_tm(question_data, 1, False)
        return status, {"message": message}, headers
