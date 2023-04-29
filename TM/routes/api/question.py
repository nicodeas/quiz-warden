from ..base import BaseRoute
from test_manager import test_manager


class Question(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        if "answer" not in qs:
            # Get a new question without checking it
            try:
                current_question = int(qs['number'][0])
                print(current_question, test_manager.max_questions)
                if 0 < current_question < test_manager.max_questions:
                    test_manager.check_question_number(current_question)
                    question_info = test_manager.get_question_info()
                    return 200, question_info
                return 400, {"message": "Question out of range"}
            except:
                return 500, {"message": "Error getting question"}
            
        is_correct = test_manager.check_answer(qs['answer'])
        return 200, {"correct": is_correct}
            
        
        
    