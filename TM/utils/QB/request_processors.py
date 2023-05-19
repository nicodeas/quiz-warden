import io
import os


def receive_data(s):
    data = io.BytesIO()
    while True:
        res = s.recv(io.DEFAULT_BUFFER_SIZE)
        if not res:
            break
        data.write(res)

    data = data.getvalue()

    return data


# stores question fields in a dict; writes image_data to file if it exists
def process_question(qb_response):
    question_parts = qb_response.decode().split("&")
    print(question_parts)
    # only receives fields to display question to user; marking handled by QB
    question = {
        "id": int(question_parts[0]),
        "language": question_parts[1],
        "type": question_parts[2],
        "text": question_parts[3],
    }
    if question["type"] == "CHOICE":
        question["choices"] = question_parts[4].split("^")
    return question


def process_image_data(image_data, filename):
    if not os.path.exists(filename):
        with open(filename, "wb") as f:
            f.write(image_data)
    else:
        print("file already exists on TM!")


# reads stream of q_ids and stores in list
def process_generated_questions(qb_response):
    questions = [question for question in qb_response.decode().split("&") if question]
    return questions


# checks if answer sent to QB is correct
def return_mark(qb_response):
    return qb_response == b"correct"
