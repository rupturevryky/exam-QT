#include "mytcpserver-ex.h"
#include <QDebug>
#include <QCoreApplication>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Server is not started";
    } else {
        server_status = 1;
        qDebug() << "Server is started";
    }
}

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
    server_status = 0;
}

void MyTcpServer::slotNewConnection()
{
    if (server_status == 1) {
        mTcpSocket = mTcpServer->nextPendingConnection();
        mTcpSocket->write("Hello, I am an echo server with heap sort functionality!\r\n");
        connect(mTcpSocket, &QTcpSocket::readyRead,
                this, &MyTcpServer::slotServerRead);
        connect(mTcpSocket, &QTcpSocket::disconnected,
                this, &MyTcpServer::slotClientDisconnected);
    }
}

void MyTcpServer::slotServerRead()
{
    while (mTcpSocket->bytesAvailable() > 0) {
        QByteArray data = mTcpSocket->readAll();
        QString message = QString::fromUtf8(data);

        // Проверяем, что команда начинается с "sort&"
        if (message.startsWith("sort&")) {
            QStringList parts = message.split('&');
            if (parts.size() != 3) {
                mTcpSocket->write("Error: Invalid format.\r\n");
                return;
            }

            int step = parts[1].toInt();  // Извлекаем шаг сортировки
            QStringList numberStrings = parts[2].split(' ');  // Получаем числа в виде строк

            // Преобразуем числа в список int
            QList<int> numbers;
            for (const QString &numStr : numberStrings) {
                numbers.append(numStr.toInt());
            }

            // Выполняем пирамидальную сортировку до указанного шага
            QString result = heapSortUntilStep(numbers, step);

            // Отправляем результат клиенту
            mTcpSocket->write(result.toUtf8() + "\r\n");
        } else {
            mTcpSocket->write("Unknown command.\r\n");
        }
    }
}

void MyTcpServer::slotClientDisconnected()
{
    mTcpSocket->close();
}

// Функция для перестройки кучи
void MyTcpServer::heapify(QList<int> &arr, int n, int i)
{
    int largest = i;  // Изначально считаем текущий элемент (i) наибольшим
    int left = 2 * i + 1;  // Индекс левого потомка
    int right = 2 * i + 2;  // Индекс правого потомка

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        qSwap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Основная функция сортировки с возвратом состояния после указанного шага
QString MyTcpServer::heapSortUntilStep(QList<int> &arr, int step)
{
    int n = arr.size();
    int currentStep = 0;

    // Строим кучу (перестраиваем массив)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
        currentStep++;
        if (currentStep == step) {
            return arrayToString(arr);  // Возвращаем массив после текущего шага
        }
    }

    // Извлекаем элементы из кучи один за другим
    for (int i = n - 1; i > 0; i--) {
        qSwap(arr[0], arr[i]);
        heapify(arr, i, 0);
        currentStep++;
        if (currentStep == step) {
            return arrayToString(arr);  // Возвращаем массив после текущего шага
        }
    }

    // Если шаг превышает количество шагов в сортировке, возвращаем полностью отсортированный массив
    return arrayToString(arr);
}

// Вспомогательная функция для преобразования массива в строку
QString MyTcpServer::arrayToString(const QList<int> &arr)
{
    QString result;
    for (int num : arr) {
        // Добавляем только те числа, которые не равны нулю
        if (num != 0 || result.isEmpty()) {
            result += QString::number(num) + " ";
        }
    }
    return result.trimmed(); // Убираем пробел в конце
}
