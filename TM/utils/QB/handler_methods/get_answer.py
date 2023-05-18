def get_answer(self, addr, q_id):
    req = f"GET_ANSWER_BY_ID|{q_id}"

    res = self.send_request(addr, req)
    if res is None:
        return False

    return res
