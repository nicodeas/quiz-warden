from config import QB_HOSTS


class QbHandler:
    from . import connect, health_check

    # Singleton
    def __new__(cls):
        if not hasattr(cls, "instance"):
            cls.instance = super(QbHandler, cls).__new__(cls)
            return cls.instance

    def __init__(self):
        self.qbs = []

    def initialise(self):
        for host in QB_HOSTS:
            if self.connect(host):
                self.qbs.append(host)
                print(f"  Connected to QB at {host[0]}:{host[1]}")
            else:
                print(f"  QB at {host[0]}:{host[1]} is down")
