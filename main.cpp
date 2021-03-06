#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "searchengine.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine application_engine;

    search::SearchEngine search_engine;
    QQmlContext* ctx = application_engine.rootContext();
    ctx->setContextProperty("search_engine", &search_engine);
    application_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
