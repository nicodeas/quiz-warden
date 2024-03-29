import socket

from request_processors import *

# debug: req variables for q_id and num_to_generate
Q_TO_GET = 3
NUM_TO_GENERATE = 4
Q_TO_MARK = 3
USER_ANSWER = "./cool-chris.png"

HOST = "localhost"  # replace with server address
PORT = 8080  # replace with server port

# create a socket and connect to the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

# request needs to contain a valid keyword else there will be an error
#req = f"GET_QUESTION_BY_ID|{Q_TO_GET}"
#req = f"GENERATE_QUESTIONS|{NUM_TO_GENERATE}"
#req = f"MARK_QUESTION_BY_ID|{Q_TO_MARK}|{USER_ANSWER}"
req = f"GET_ANSWER_BY_ID|{Q_TO_GET}"
s.send(req.encode())

qb_response, image_data = receive_data(s)
print(qb_response)

if "GENERATE_QUESTIONS" in req:
    # array holding order of q_ids from a server
    questions = process_generated_questions(qb_response)
    print(questions)
elif "MARK_QUESTION_BY_ID" in req:
    mark = return_mark(qb_response)
    print(mark)
elif "GET_QUESTION_BY_ID" in req:
    # dict storing question fields; image is written to directory
    question = process_question(qb_response, image_data)
    print(question)
elif "GET_ANSWER_BY_ID" in req:
    answer = return_answer(qb_response)
    print(answer)
elif "HEALTH_CHECK" in req:
    pass

# close the socket
s.close()