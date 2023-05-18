import io
import os


def receive_data(s):
    # flag denoting when stream of image data begins
    IMAGE_DATA_FLAG = b"$"
    receiving_image_data = False
    # store question fields
    qb_response = b""
    # store image data
    image_data = b""

    data = io.BytesIO()
    while True:
        res = s.recv(io.DEFAULT_BUFFER_SIZE)
        if not res:
            break
        data.write(res)

    data = data.getvalue()

    # check for image data flag
    if not receiving_image_data:
        image_index = data.find(IMAGE_DATA_FLAG)
        # reading image data!
        if image_index != -1:
            qb_response += data[:image_index]
            image_data += data[image_index + len(IMAGE_DATA_FLAG) :]
            receiving_image_data = True
        else:
            # no image flag; read as regular response
            qb_response += data
    else:
        image_data += data
    return qb_response, image_data


# stores question fields in a dict; writes image_data to file if it exists
def process_question(qb_response, image_data):
    question_parts = qb_response.decode().split("&")
    # only receives fields to display question to user; marking handled by QB
    question = {
        "id": int(question_parts[0]),
        "language": question_parts[1],
        "type": question_parts[2],
        "text": question_parts[3],
    }
    if question["type"] == "CHOICE":
        question["choices"] = question_parts[4].split("^")
    elif question["type"] == "IMAGE":
        image_info = question_parts[4].split("$")
        question["image"] = image_info[0]
        process_image_data(image_data, question["image"])
    return question


def process_image_data(image_data, filename):
    # TODO: naming convention and directory for images
    # possibly just name after question?
    # could images just be stored on TM?

    # check file hasn't already been requested
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
