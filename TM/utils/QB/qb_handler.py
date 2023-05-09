from config import QB_HOSTS

from .qb_health_check import do_health_check


class QbHandler:
    from . import connect, generate_test, health_check

    # Singleton
    def __new__(cls, *args, **kwargs):
        instance = cls.__dict__.get("instance")
        if instance is not None:
            return instance
        cls.instance = instance = object.__new__(cls)
        instance.init(*args, **kwargs)
        return instance

    def init(self, *args, **kwargs):
        self.qbs = {}
        do_health_check(self)