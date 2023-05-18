from classes.user import users
from utils.QB.qb_handler import QbHandler

from ..base import BaseRoute


class State(BaseRoute, route="api"):
    """
    Returns various information about the state of the application.

    Currently, this includes the following:
    - activeQBs: A list of all healthy question banks that are loaded.

    Additionally, if a user is logged in, the following information is also returned:
    - username: The username of the user.
    - maxQuestions: The maximum number of questions in the quiz.
    - currentQuestion: The current question number.
    - completed: Whether or not the user has completed the quiz.
    - totalMarks: The total marks of the user for the quiz.
    """

    def executor(req, path, qs, *args, **kwargs):
        payload = {}
        qb_instance = QbHandler()

        session_id = req.headers["Cookie"].split("=")[1]
        if session_id in users and users.get(session_id) != {}:
            user = users[session_id]
            payload["username"] = user.username
            payload["maxQuestions"] = user.max_questions
            payload["currentQuestion"] = user.current_question
            payload["completed"] = user.completed

            total_marks = 0

            for question in user.questions:
                if question["correct"]:
                    total_marks += (
                        4 - question["attempts"] if question["attempts"] else 0
                    )
            payload["totalMarks"] = total_marks

        payload["activeQBs"] = list(qb_instance.qbs.values())
        return 200, payload, {}
