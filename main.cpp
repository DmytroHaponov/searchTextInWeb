#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "searchengine.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    SearchEngine search_engine;
    QQmlContext* ctx = engine.rootContext();
    ctx->setContextProperty("search_engine", &search_engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
