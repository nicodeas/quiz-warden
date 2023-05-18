import io
import socket

HOST = "localhost"  # replace with server address
PORT = 8080  # replace with server port
filename = "received.txt"

# create a socket and connect to the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

attempt = ""
with open("try.py") as f:
    attempt = f.read()
# request needs to contain a valid keyword else there will be an error
req = "MARK_QUESTION_BY_ID|0|" + attempt

print(attempt)
s.send(req.encode())

# receive data and write it to a file
data = b""
# NOTE: use w and decode for text, use wb and im not sure if you need to decode for images
with open(filename, "w") as f:
    while True:
        data = s.recv(io.DEFAULT_BUFFER_SIZE)
        print(data.decode())
        if not data:
            break
        f.write(data.decode())


# close the file and the socket
s.close()
