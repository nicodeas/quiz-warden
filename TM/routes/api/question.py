from ..base import BaseRoute
from test_manager import test_manager


class Question(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        print(qs)
        if("answer" not in qs):
            # Get a new question without checking it
            try:
                test_manager.check_question_number(int(qs['number'][0]))
                question_info = test_manager.get_question_info()
                return 200, question_info
            except:
                return 500, {"message": "Error getting question"}
        else:
            
            is_correct = test_manager.check_answer(qs['answer'])
            return 200, {"correct": is_correct}
            
        
        
    