FROM mysql:8.3.0

ARG MYSQL_DATABASE
ARG MYSQL_USER
ARG MYSQL_PASSWORD
ARG MYSQL_ROOT_PASSWORD
ARG MYSQL_ROOT_HOST
ARG MYSQL_PORT
ARG MYSQL_INITDB_SKIP_TZINFO

# указываем переменные окружения
ENV MYSQL_DATABASE=$MYSQL_DATABASE
ENV MYSQL_USER=$MYSQL_USER
ENV MYSQL_PASSWORD=$MYSQL_PASSWORD
ENV MYSQL_ROOT_PASSWORD=$MYSQL_ROOT_PASSWORD
ENV MYSQL_ROOT_HOST=$MYSQL_ROOT_HOST
ENV MYSQL_PORT=$MYSQL_PORT
ENV MYSQL_INITDB_SKIP_TZINFO=$MYSQL_INITDB_SKIP_TZINFO

# копируем в контейнер Docker файл инициализации базы данных в директорию /docker-entrypoint-initdb.d 
# файл docker-entrypoint.sh будет запускать любые файлы в этой директории заканчивающиеся на «.sql»
ADD ./docker/init/init.sql /docker-entrypoint-initdb.d

# указываем,что порт 3306 необходимо открыть
EXPOSE 3306 33060

# команды для docker
# Сборка образа из Dockerfile, который находится в этой директории
# docker build -t db_mysql .

# Запуск котнейнера
# docker run --name=db_mysql -d -p 3306:3306 -v db_data:/var/lib/mysql --env-file ../.env --rm db_mysql

# docker exec -it db_mysql bash

# Остановка котнейнера
# docker stop db_mysql

# Удаление всех остановленных контейнеров
# docker container prune

# Удаление всех томов (кроме именованных вручную)
# docker volume prune

# Удаление тома db_data
# docker volume rm db_data

#Удаление всех используемых сетей
# docker network prune -f