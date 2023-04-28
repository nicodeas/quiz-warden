from http.server import HTTPServer
from TM.utils.request_handler import RequestHandler
from .config import *

def main():
    server = HTTPServer((HOST, PORT), RequestHandler)
    print(f'Started server on {HOST}:{PORT}')
    server.serve_forever()
    
if __name__ == '__main__':
    main()