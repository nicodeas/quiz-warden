FROM alpine:latest

RUN apk add build-base

WORKDIR /QB

COPY ./ ./

RUN make -B

CMD ["make", "run"]
