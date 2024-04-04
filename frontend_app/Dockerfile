FROM node:lts-alpine

RUN npm install -g http-server

WORKDIR /frontend_app

COPY ./package.json .
COPY ./package-lock.json .
RUN npm install

COPY . .

EXPOSE 8080