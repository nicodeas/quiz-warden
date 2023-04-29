class BaseRoute:
    routes = {}

    def __init_subclass__(self, route=""):
        route = f"{route}/" if route else route
        path = "" if self.__name__.lower() == "index" else self.__name__.lower()
        self.routes[route + path] = self.executor
