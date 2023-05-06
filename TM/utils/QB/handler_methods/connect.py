import socket


def connect(self, addr):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect(addr)
    except socket.error:
        return None

    return s
