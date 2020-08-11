#include <QCoreApplication>
#include <QDebug>

#include "facelift/example/mypackagenoqml/MyInterfaceIPCAdapter.h"
#include "facelift/example/mypackagenoqml/MyInterfaceIPCProxy.h"
#include "facelift/example/mypackagenoqml/MyInterface.h"

using namespace facelift::example::mypackagenoqml;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Starting IPC Client...";

    // Client
    MyInterfaceIPCProxy proxy;
    QObject::connect(&proxy, &MyInterface::counterChanged, [&proxy]() 
    { 
        qDebug() << "counterChanged to" << proxy.counter();
    });
    proxy.connectToServer();

    return a.exec();
}
