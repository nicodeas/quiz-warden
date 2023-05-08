import socket

HOST = "localhost"  # replace with server address
PORT = 8080  # replace with server port

# create a socket and connect to the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

Q_TO_GET = 0

# request needs to contain a valid keyword else there will be an error
req = f"GET_QUESTION_BY_ID|{Q_TO_GET}"
#req = "HEALTH_CHECK|"
s.send(req.encode())

qb_response =""
# receive and print the response from the server
while True:
    data = s.recv(1024)
    if not data:
        break
    qb_response+=data.decode()

    #qb_response = data.decode()
print(qb_response)

for line in qb_response.split('\n'):
    if not line:
        continue
    question_parts = line.split('&')
    if not question_parts[0]:
        continue
    question = {
        'id': int(question_parts[0]),
        'language': question_parts[1],
        'type': question_parts[2],
        'text': question_parts[3]
    }
    if question['type'] == 'CHOICE':
        question['choices'] = question_parts[4].split('^')
    if question['type'] == 'IMAGE':
        question['image'] = question_parts[4]
print("TM end after sorting question into dict:")
print(question)


# close the socket
s.close()
