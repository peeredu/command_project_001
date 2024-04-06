FROM node:lts-alpine

RUN npm install -g http-server

WORKDIR /frontend_app

COPY ./package.json .
COPY ./package-lock.json .
RUN npm install

COPY . .

RUN npm run build

EXPOSE 8080

CMD ["npm", "run", "serve"]