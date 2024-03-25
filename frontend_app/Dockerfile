FROM node:lts-alpine

RUN npm install -g http-server

WORKDIR /frontend_app

COPY ./package_json .
COPY ./package-lock.json .
RUN npm install

COPY . .