#include "server.h"
#include "serverworker.h"

Server::Server(QObject *parent):
    QTcpServer(parent)
{}

void Server::incomingConnection(qintptr socketDesriptor) {
    qDebug("New Player");
    ServerWorker *worker = new ServerWorker(this);
    if (!worker->setSocketDescriptor(socketDesriptor)) {
        worker->deleteLater();
        return;
    }

    clients.append(worker);
}
