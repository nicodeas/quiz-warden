import socket
import io

HOST = "localhost"  # replace with server address
PORT = 8080  # replace with server port
filename = "received_file.txt"

# create a socket and connect to the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

# receive data and write it to a file
with open(filename, "wb") as f:
    while True:
        data = s.recv(io.DEFAULT_BUFFER_SIZE)
        if not data:
            break
        f.write(data)

# close the file and the socket
s.close()
