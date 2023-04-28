from .base import BaseRoute
from utils.html_reader import html_reader


class Index(BaseRoute):
    def executor(path, *args, **kwargs):
        return html_reader(path)
