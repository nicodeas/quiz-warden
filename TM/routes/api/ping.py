from ..base import BaseRoute


class Ping(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        return 200, {"message": "pong"}, {}
