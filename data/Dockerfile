# делаем контейнер из официального образа
FROM mysql

# указываем пароль для БД
ENV MYSQL_ROOT_PASSWORD root

# копируем в контейнер Docker файл инициализации базы данных в директорию /docker-entrypoint-initdb.d 
# файл docker-entrypoint.sh будет запускать любые файлы в этой директории заканчивающиеся на «.sql»
ADD ./docker/init/init.sql /docker-entrypoint-initdb.d

# указываем,что порт 3306 необходимо открыть
EXPOSE 3306

# команды для docker
# Сборка образа из Dockerfile, который находится в этой директории
# docker build -t db_mysql .

# Запуск котнейнера
# docker run --name=db_mysql -d -p 3306:3306 -v db_data:/var/lib/mysql db_mysql

# Остановка котнейнера
# docker stop db_mysql

# Удаление всех остановленных контейнеров
# docker container prune

# Удаление всех томов (кроме именованных вручную)
# docker volume prune

# Удаление тома db_data
# docker volume rm db_data