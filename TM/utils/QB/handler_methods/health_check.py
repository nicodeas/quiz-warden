import io
import socket


def health_check(self, addr):
    req = "HEALTH_CHECK|"

    s = self.connect(addr)
    if s is None:
        return False

    s.send(req.encode())

    while True:
        data = s.recv(io.DEFAULT_BUFFER_SIZE)
        if not data:
            break
        print(data.decode())
    s.close()

    return True
