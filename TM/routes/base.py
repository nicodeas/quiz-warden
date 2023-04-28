class BaseRoute:
    routes = {}

    def __init_subclass__(self):
        self.routes[self.__name__.lower()] = self.executor
