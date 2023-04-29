from utils.html_reader import html_reader

from .base import BaseRoute


class Index(BaseRoute):
    def executor(req, path, *args, **kwargs):
        return html_reader(path)