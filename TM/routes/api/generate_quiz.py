from utils.QB.qb_handler import QbHandler
from classes.user import *
from utils.auth import *
import random
from ..base import BaseRoute


class GenerateQuiz(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        status = 200
        message = "Success"
        headers = {}

        NUM_QUESTIONS = 3

        qb_handler = QbHandler()

        session_id = req.headers["Cookie"].split("=")[1]
        qbs = qb_handler.qbs.items()
        qb_list = [(qb[0], qb[1]) for qb in qbs]

        if len(qbs) == 0:
            status = 500
            message = "No question banks loaded"
            return status, {"message": message}, headers

        num_questions = random.randint(1, NUM_QUESTIONS)
        questions = qb_handler.generate_quiz(qb_list[0][0], num_questions)

        if len(qbs) == 2:
            num_questions = NUM_QUESTIONS - num_questions
            questions += qb_handler.generate_quiz(qb_list[1][0], num_questions)

        session = {}
        with open("data/session.json", "r") as f:
            session = json.load(f)

        question_data = {
            q: {"attempts": 0, "type": None, "correct": None} for q in questions
        }

        session[session_id] = {}
        session[session_id]["questions"] = question_data
        session[session_id]["current_question"] = 0

        with open("data/session.json", "w") as f:
            json.dump(session, f, indent=2)

        return status, {"message": message}, headers
