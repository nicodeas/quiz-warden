import io


def health_check(self, addr):
    req = "HEALTH_CHECK|"

    s = self.connect(addr)
    if s is None:
        return False

    s.send(req.encode())

    res = io.BytesIO()
    while True:
        data = s.recv(io.DEFAULT_BUFFER_SIZE)
        if not data:
            break
        res.write(data)

    return res.getvalue().decode()
