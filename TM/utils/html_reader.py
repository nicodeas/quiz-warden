import os


def html_reader(route):
    html_files = os.listdir("views")
    html_routes = [file.rstrip(".html") for file in html_files]

    if route in html_routes:
        with open(f"views/{route}.html", "r") as html:
            return html.read()
    return None
