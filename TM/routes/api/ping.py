from ..base import BaseRoute


class Ping(BaseRoute, route="api"):
    def executor(req, path, *args, **kwargs):
        return 200, {"message": "pong"}
