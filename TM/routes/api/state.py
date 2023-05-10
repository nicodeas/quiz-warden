from utils.QB.qb_handler import QbHandler

from ..base import BaseRoute


class State(BaseRoute, route="api"):
    """
    Returns various information about the state of the application.

    Currently, this includes the following:
    - activeQBs: A list of all healthy question banks that are loaded.
    """

    def executor(req, path, qs, *args, **kwargs):
        payload = {}
        qb_instance = QbHandler()
        payload["activeQBs"] = list(qb_instance.qbs.values())
        return 200, payload
