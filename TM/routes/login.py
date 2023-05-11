from urllib.parse import parse_qs

from user import *
from utils.auth import *
from utils.html_reader import html_reader

from .base import BaseRoute


class Login(BaseRoute):
    def executor(req, path, qs, *args, **kwargs):
        return html_reader(path)
