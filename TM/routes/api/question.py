from classes.user import User, users

from ..base import BaseRoute


class Question(BaseRoute, route="api"):
    """
    GET: get question by id
    """

    def executor(req, path, qs, *args, **kwargs):
        headers = {}

        # Get session_id from request headers
        session_id = req.headers["Cookie"].split("=")[1]
        if session_id not in users or users.get(session_id) == {}:
            return 400, {"message": "No quiz"}, headers

        # Get user object using session_id as key
        user: User = users[session_id]
        # Get current question number from query
        current_question = int(qs["number"][0])
        # Check if current question number is out of range
        if current_question < 1 or current_question > user.max_questions:
            return 400, {"message": "Question out of range"}, headers

        # Get question information for current question
        question_info = user.get_question_info(current_question)
        # Update user's current question
        user.current_question = current_question

        return 200, question_info, headers
