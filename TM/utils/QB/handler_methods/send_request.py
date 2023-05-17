import socket

from utils.QB.request_processors import receive_data


def send_request(self, addr, req):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect(addr)
    except socket.error:
        return None

    s.send(req.encode())

    res = receive_data(s)
    return res
