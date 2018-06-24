#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "searchengine.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine application_engine;

    SearchEngine search_engine;
    QQmlContext* ctx = application_engine.rootContext();
    ctx->setContextProperty("search_engine", &search_engine);

    application_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
//    qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionNumber();
//    qDebug()<<QCoreApplication::libraryPaths();

    return app.exec();
}
