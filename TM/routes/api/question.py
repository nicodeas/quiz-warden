from classes.test_manager import TestManager

from ..base import BaseRoute


class Question(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        headers = {}
        if "answer" not in qs:
            # Get a new question without checking it
            try:
                current_question = int(qs["number"][0])
                if 0 < current_question < test_manager.max_questions:
                    



                    return 200, question_info, headers
                return 400, {"message": "Question out of range"}, headers
            except:
                return 500, {"message": "Error getting question"}, headers

        payload = test_manager.check_answer(qs["answer"], qs["number"])
        return 200, payload, headers
