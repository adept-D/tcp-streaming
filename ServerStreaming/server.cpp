#include "server.h"



Server::Server(QObject *parent)
    : QTcpServer(parent), frameSize(0), totalBytes(0),bytesReceived(0)
{
}

void Server::startServer(){
    try{
        connect(this,&QTcpServer::destroyed, this, &QTcpServer::deleteLater);

        if(this->listen(QHostAddress::Any,8000)){
              qDebug() << "Server is listening..." << Qt::endl;
        }else{
             throw -1;
        }
    }
    catch(int){
        std::cerr << "Can't connect to server" << std::endl;
    }
}


void Server::incomingConnection(qintptr handle){
    _socket = new QTcpSocket;
     qDebug() << "Client Connected " << Qt::endl;
     _socket->setSocketDescriptor(handle);


     connect(_socket,&QTcpSocket::readyRead, this, &Server::slotReadyRead);
     connect(_socket,&QTcpSocket::disconnected, _socket, &QTcpSocket::deleteLater);

}


void Server::slotReadyRead(){
    QDataStream in(_socket);

        in.setVersion(QDataStream::Qt_5_6);
        _socket->waitForBytesWritten();
        if (bytesReceived <= sizeof(quint32) * 5) {
            if((_socket->bytesAvailable() >= static_cast<qint64>(sizeof(quint32) * 5))
                    && (frameSize == 0)) {
                in >> totalBytes  >> frameSize >> rows >> cols >> audioLen;
                bytesReceived += sizeof(quint32) * 5;
            }
            if((_socket->bytesAvailable() >= frameSize)
                    && (frameSize != 0)) {

                in >> imgData;
                in >> audioData;
                in >> type;

                bytesReceived += frameSize;

                if(bytesReceived == totalBytes){

                   if( type == "cam" ){
                       int format = 3; //CV_8UC3
                        emit sendFrame(imgData, rows, cols, format);
                   }
                   else if(type == "screen"){
                       int format = 4; //CV_8UC4
                       emit sendFrame(imgData, rows, cols, format);
                   }
                   else if(type == "audio"){
                       emit sendAudio(audioData, audioLen);
                   }
                     totalBytes = 0;
                     bytesReceived = 0;
                     frameSize = 0;
                     audioData = 0;
                     type = "";
                }
             }
         }

}



