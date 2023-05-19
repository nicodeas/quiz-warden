from utils.QB.request_processors import process_generated_questions


def generate_quiz(self, addr, num_questions):
    req = f"GENERATE_QUESTIONS|{num_questions}"

    res = self.send_request(addr, req)
    if res is None:
        return False
    res = process_generated_questions(res)
    return res
