from utils.QB.request_processors import process_question


def get_question(self, addr, q_id):
    req = f"GET_QUESTION_BY_ID|{q_id}"

    res = self.send_request(addr, req)
    if res is None:
        return False

    res = process_question(res)
    return res
