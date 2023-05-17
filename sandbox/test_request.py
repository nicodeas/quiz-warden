import socket

from request_processors import *

# debug: req variables for q_id and num_to_generate
Q_TO_GET = 3
NUM_TO_GENERATE = 4

HOST = "localhost"  # replace with server address
PORT = 8080  # replace with server port

# create a socket and connect to the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

# request needs to contain a valid keyword else there will be an error
req = f"GET_QUESTION_BY_ID|{Q_TO_GET}"
# req = f"GENERATE_QUESTIONS|{NUM_TO_GENERATE}"
s.send(req.encode())

qb_response, image_data = receive_data(s)
print(qb_response)

if "GENERATE_QUESTIONS" in req:
    # array holding order of q_ids from a server
    questions = process_generated_questions(qb_response)
    print(questions)
elif "MARK_QUESTION_BY_ID" in req:
    pass
elif "GET_QUESTION_BY_ID" in req:
    # dict storing question fields; image is written to directory
    question = process_question(qb_response, image_data)
    print("TM end after sorting question into dict:")
    print(question)
elif "HEALTH_CHECK" in req:
    pass

# close the socket
s.close()
