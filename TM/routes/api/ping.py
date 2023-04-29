from ..base import BaseRoute


class Ping(BaseRoute, is_api=True):
    def executor(path, *args, **kwargs):
        return 200, {"message": "pong"}
