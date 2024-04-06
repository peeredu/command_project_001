FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y make && \
    apt-get install -y default-libmysqlclient-dev && \
    apt-get install -y gcc &&\
    apt-get install -y clang-format && \
    apt-get install -y cppcheck

WORKDIR /app

COPY ./backend ./backend
COPY ./data/src ./data/src

RUN make -C ./backend/src/

EXPOSE 2000

CMD ["./backend/bin/socket_server"]