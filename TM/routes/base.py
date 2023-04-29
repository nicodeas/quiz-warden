class BaseRoute:
    routes = {}

    def __init_subclass__(self, is_api=False):
        if not is_api:
            self.routes[self.__name__.lower()] = self.executor
        else:
            self.routes[f"api/{self.__name__.lower()}"] = self.executor
