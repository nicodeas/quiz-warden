from classes.user import User, users

from ..base import BaseRoute


class Mark(BaseRoute, route="api"):
    """
    POST: mark question by id
    """

    def executor(req, path, qs, *args, **kwargs):
        headers = {}

        post_data = kwargs.get("post_data")
        answer = post_data.get("answer", "")
        current_question = int(post_data.get("number", ""))

        session_id = req.headers["Cookie"].split("=")[1]
        if session_id not in users or users.get(session_id) == {}:
            return 400, {"message": "No quiz"}, headers

        user: User = users[session_id]

        if current_question < 1 or current_question > user.max_questions:
            return 400, {"message": "Question out of range"}, headers

        is_correct, error_message = user.mark_question(current_question, answer)

        if is_correct:
            user.questions[current_question - 1]["correct"] = True
        else:
            if user.questions[current_question - 1]["attempts"] == 3:
                user.questions[current_question - 1]["correct"] = False
            else:
                user.questions[current_question - 1]["attempts"] += 1
        if not any(q["correct"] is None for q in user.questions):
            user.completed = True

        print(error_message)
        user.questions[current_question - 1]["errors"] = (
            error_message if error_message else ""
        )

        user.dump_sessions()

        return (
            200,
            {},
            headers,
        )
