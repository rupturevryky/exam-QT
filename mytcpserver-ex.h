#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

public slots:
    void slotNewConnection();         // Обработчик нового подключения клиента
    void slotClientDisconnected();    // Обработчик отключения клиента
    void slotServerRead();            // Обработчик чтения данных от клиента

private:
    QTcpServer *mTcpServer;           // Указатель на TCP сервер
    QTcpSocket *mTcpSocket;           // Указатель на подключённый сокет клиента
    int server_status;                // Статус сервера

    void heapify(QList<int> &arr, int n, int i); // Функция для поддержания максимальной кучи
    QString heapSortUntilStep(QList<int> &arr, int step); // Функция сортировки до указанного шага
    QString arrayToString(const QList<int> &arr); // Вспомогательная функция для преобразования массива в строку
};

#endif // MYSERVER_H
