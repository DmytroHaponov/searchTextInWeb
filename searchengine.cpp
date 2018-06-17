#include "searchengine.h"
#include <QDebug>

SearchEngine::SearchEngine(QObject *parent) : QObject(parent)
{

}

void SearchEngine::on_main_URL_received(QString url)
{
    qDebug()<<"starting url is "<<url;
}
