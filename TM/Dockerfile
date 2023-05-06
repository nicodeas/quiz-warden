FROM python:latest

COPY --from=ghcr.io/ufoscout/docker-compose-wait:latest /wait /wait

WORKDIR /TM

COPY ./ ./

RUN pip install -r requirements.txt

CMD /wait && python -u -m main