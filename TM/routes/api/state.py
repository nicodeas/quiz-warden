from ..base import BaseRoute
from utils.QB.qb_handler import QbHandler

class State(BaseRoute, route="api"):
    def executor(req, path, qs, *args, **kwargs):
        payload = {}
        qb_instance =QbHandler()
        payload["activeQBs"] = list(qb_instance.qbs.values())
        return 200, payload
