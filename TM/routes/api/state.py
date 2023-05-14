from utils.QB.qb_handler import QbHandler

from ..base import BaseRoute

from classes.user import users
class State(BaseRoute, route="api"):
    """
    Returns various information about the state of the application.

    Currently, this includes the following:
    - activeQBs: A list of all healthy question banks that are loaded.
    """

    def executor(req, path, qs, *args, **kwargs):
        payload = {}
        qb_instance = QbHandler()
        
        session_id = req.headers["Cookie"].split("=")[1]
        if session_id in users and users.get(session_id) != {}:
            user = users[session_id]
            payload["maxQuestions"] = user.max_questions
            payload["currentQuestion"] = user.current_question
            payload["completed"] = user.completed

        payload["activeQBs"] = list(qb_instance.qbs.values())
        return 200, payload, {}
