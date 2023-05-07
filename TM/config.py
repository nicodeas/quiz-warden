import os

PORT = 8000
HOST = "0.0.0.0"

# TODO: remove when submitting project
if os.environ.get("ENV") == "docker":
    QB_HOSTS = [("python_question_bank", 8080), ("c_question_bank", 8081)]
else:
    QB_HOSTS = [("localhost", 8080), ("localhost", 8081)]

# In seconds
HEALTHCHECK_INTERVAL = 5
