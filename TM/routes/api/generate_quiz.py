import os
from utils.QB.qb_handler import QbHandler
from user import *
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

        print(questions)
        return status, {"message": message}, headers
