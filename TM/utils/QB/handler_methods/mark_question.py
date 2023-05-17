from utils.QB.request_processors import process_question


def mark_question(self, addr, q_id, answer):
    req = f"MARK_QUESTION_BY_ID|{q_id}|{answer}"

    res = self.send_request(addr, req)
    if res is None:
        return False
    res = process_question(res[0], res[1])
    return res
