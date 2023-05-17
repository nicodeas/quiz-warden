from classes.user import User, users

from ..base import BaseRoute


class Question(BaseRoute, route="api"):
    """
    GET: mark question by id
    """

    def executor(req, path, qs, *args, **kwargs):
        headers = {}
        
        post_data = kwargs.get("post_data")
        answer = post_data.get("answer", "")

        session_id = req.headers["Cookie"].split("=")[1]
        if session_id not in users or users.get(session_id) == {}:
            return 400, {"message": "No quiz"}, headers

        user: User = users[session_id]
        current_question = int(qs["number"][0])
        if current_question < 1 or current_question > user.max_questions:
            return 400, {"message": "Question out of range"}, headers

        is_correct = user.mark_question(current_question, answer)
        print(is_correct)

        user.dump_sessions()

        return 200, {}, headers
