#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QObject>

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QObject *parent = nullptr);

signals:

public slots:
    void on_main_URL_received(QString url);
};

#endif // SEARCHENGINE_H
