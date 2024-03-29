# quiz-warden

## Overview

## Project Structure

## Test Manager

### Setup

- To start the `TM` server, change into the `TM` directory and run

```py
python -m main
```

### Development

#### Adding page routes

- Add a new file to the `routes` directory. The file should be named after the route. For example, the route `/quiz` should be named `quiz.py`.
- In the file, update the class name to match the route in PascalCase. For example, the route `/quiz` should be named `Quiz`.
- Register the route by importing it into `routes/__init__.py`.
- Add the relevant html file to the `views` directory. The file should be named after the route. For example, the route `/quiz` should be named `quiz.html`.
- Add the relevant static files to the `static` directory. Added static files can be accessed at `/static/<filename>`.

#### Adding API routes

- Add a new file to the `api` directory. The file should be named after the route. For example, the route `/api/quiz` should be named `quiz.py`.
- In the file, update the class name to match the route in PascalCase. For example, the route `/api/quiz` should be named `Quiz`. Change the argument `route` to be the path of the directory.
- Register the route by importing it into `routes/api/__init__.py`.

## Question Bank

See make file for options

To run in DEBUG mode, use `DEBUG=1 ./main -p 8080 PYTHON`
