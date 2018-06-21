#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QObject>
#include <map>
#include <list>
#include <QNetworkReply>
#include <QThreadPool>

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QObject *parent = nullptr);

signals:
    void error_msg(QString msg);

public slots:
    void on_main_URL_received(QString url);
    void set_max_threads_count(QString count);
    void set_target_text(QString text);
    void set_max_URL_quantity(QString count);

private:
    int qstring_to_int(QString count, QString msg);

private:
    /// string - parent url, where link was found
   std::map<QString, std::list<QNetworkReply*>> m_downloaded;
   QThreadPool m_thread_pool;
   QString m_starting_URL;
   QString m_target_text;
   int m_max_URL_count = -1;
};

#endif // SEARCHENGINE_H
