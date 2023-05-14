def health_check(self, addr):
    req = "HEALTH_CHECK|"

    res = self.send_request(addr, req)
    if res is None:
        return False

    return res[0].decode()
