from utils.html_reader import html_reader

from .base import BaseRoute


class Quiz(BaseRoute):
    def executor(req, path, qs, *args, **kwargs):
        return html_reader(path)
