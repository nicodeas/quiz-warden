import os


def html_reader(route):
    headers = {}
    if route == "":
        route = "index"
    html_path = f"views/{route}.html"

    if os.path.exists(html_path):
        with open(html_path, "r") as html:
            return 200, html.read(), headers
    return 404, "Page not found", headers
