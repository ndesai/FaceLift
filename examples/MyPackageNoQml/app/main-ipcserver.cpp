#include <QCoreApplication>
#include <QDebug>

#include "facelift/example/mypackagenoqml/MyInterfaceIPCAdapter.h"
#include "facelift/example/mypackagenoqml/MyInterfaceIPCProxy.h"

#include "../models/cpp/MyInterfaceCppImplementation.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Starting IPC Server...";

    // Server
    auto& server = MyInterfaceImplementation::instance();
    QObject::connect(&server, &MyInterface::counterChanged, [&server]() 
    { 
        qDebug() << "counterChanged to" << server.counter();
    });


    // Create an IPC Adapter for the server created above
    auto ipcAdapter = new MyInterfaceIPCAdapter();
    ipcAdapter->setService(&server);
    ipcAdapter->registerService();
    
    return a.exec();
}
