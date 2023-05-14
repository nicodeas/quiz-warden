import threading
from http.server import HTTPServer

from config import HOST, PORT
from utils.QB import QbHandler, qb_health_check
from utils.request_handler import RequestHandler


def main():
    print("Connecting to QBs...")
    qb_handler = QbHandler()
    print("Finished connecting to QBs")

    server = HTTPServer((HOST, PORT), RequestHandler)
    print(f"Started server on {HOST}:{PORT}")

    # Start health check thread
    threading.Thread(target=qb_health_check, args=[qb_handler], daemon=True).start()

    # Start server
    server.serve_forever()


if __name__ == "__main__":
    main()
