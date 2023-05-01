import os


def html_reader(route):
    if route == "":
        route = "index"
    html_path = f"views/{route}.html"

    if os.path.ex
