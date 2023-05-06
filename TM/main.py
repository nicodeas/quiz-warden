import threading
import time
from http.server import HTTPServer

from config import HOST, PORT
from utils.QB import QbHandler
from utils.request_handler import RequestHandler


def qb_healthcheck(qb_handler):
    while True:
        time.sleep(2)
        for qb in qb_handler.qbs:
            qb_handler.health_check(qb)


def main():
    print("Connecting to QBs...")
    qb_handler = QbHandler()
    qb_handler.initialise()
    print("Finished connecting to QBs")

    server = HTTPServer((HOST, PORT), RequestHandler)
    print(f"Started server on {HOST}:{PORT}")

    threading.Thread(target=qb_healthcheck, args=[qb_handler]).start()

    server.serve_forever()


if __name__ == "__main__":
    main()
