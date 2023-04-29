from .base import BaseRoute
from utils.html_reader import html_reader


class Quiz(BaseRoute):
    def executor(req, path, qs, *args, **kwargs):
        return html_reader(path)