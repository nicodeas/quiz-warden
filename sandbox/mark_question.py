import io
import socket

HOST = "localhost"  # replace with server address
PORT = 8080  # replace with server port
filename = "received.txt"

# create a socket and connect to the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

attempt = ""
with open("./fix_png_header.py") as f:
    attempt = f.read()
# request needs to contain a valid keyword else there will be an error

#attempt = "def fix_png_header(input_filename, output_filename):\n\twith open(input_filename, 'rb') as f:\n\t\tdata = f.read()\n\t\tdata = bytearray(data)\n\t\tpng_header = [137, 80, 78, 71, 13, 10, 26, 10]\n\t\tfor i in range(8):\n\t\t\tdata[i] = png_header[i]\n\twith open(output_filename, 'wb') as f:\n\t\tf.write(data)\nfix_png_header('./static/img/corrupt-chris.png', './static/img/user_output.png')"

req = "MARK_QUESTION_BY_ID|3|" + attempt

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
