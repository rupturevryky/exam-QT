# Указываем базовый образ Ubuntu
FROM ubuntu:latest

# Указываем информацию об авторе
LABEL maintainer="Mishin Vladislav 231-352 vladislav.m.vlad@gmail.com"

# Устанавливаем часовой пояс
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Обновляем пакеты и устанавливаем необходимые зависимости для компиляции Qt
RUN apt-get update && \
    apt-get install -y qtbase5-dev qtchooser qt5-qmake build-essential

# Создаем директорию для проекта и копируем файлы проекта в контейнер
WORKDIR /Mishin_231-352
COPY echoServer.pro mytcpserver-ex.h main_exam.cpp mytcpserver-ex.cpp ./

# Компилируем проект
RUN qmake echoServer.pro && \
    make && \
    mv echoServer hallServer

# Указываем точку входа по умолчанию
CMD ["./hallServer"]

