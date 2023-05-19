from utils.QB.request_processors import return_mark


def mark_question(self, addr, q_id, answer):
    """
    Marks question by id. Returns a tuple of (is_correct, error)
    """
    req = f"MARK_QUESTION_BY_ID|{q_id}|{answer}"
    res = self.send_request(addr, req)

    # incorrect,error and timeout has same behaviour as error
    correct_prefix = "CORRECT|"
    error_prefix = "ERROR|"
    incorrect_prefix = "INCORRECT|"
    timeout_prefix = "TIMEOUT|"

    res = res.decode()
    if not res:
        return False, None
    elif res.startswith(correct_prefix):
        return True, None

    elif res.startswith(error_prefix):
        return False, res.removeprefix(error_prefix)

    elif res.startswith(incorrect_prefix):
        return False, res.removeprefix(incorrect_prefix)

    elif res.startswith(timeout_prefix):
        return False, "TIMEOUT"

    else:
        return False, "Malformed Response from QB"
