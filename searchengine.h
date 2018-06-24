#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H


#include <QEventLoop>

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
    /*!
     * \brief error_msg - signal for QML to display error in C++ processing data passed from QML
     * \param msg - message describing error
     */
    void error_msg(QString msg);
    void download_progress_changed(qint64 part, qint64 max, QString url);

public slots:
    void on_main_URL_received(const QString& url);
    void set_max_threads_count(const QString& count);
    void set_target_text(const QString& text);
    void set_max_URL_quantity(const QString& count);

    void page_downloaded(const QString& url);

private:
    /*!
     * \brief convert QString to int, emit errors if need
     * \param count - QString to convert
     * \param msg - custom part of message, describes source of request
     * \return result of convertion. Can return "0", but in that case engine waits new data input,
     *           there's error message on UI
     */
    int qstring_to_int(const QString& count, const QString& msg);

private:
    //! string - parent url, where link was found
    std::map<QString, std::list<QNetworkReply*>> m_downloaded;

    //! thread pool for search in downloaded URLs
    QThreadPool m_local_search_thread_pool;

    //! search starts on this URL
    QString m_starting_URL;

    //! text to search
    QString m_target_text;

    //! allowed maximum of URLs to be scanned
    int m_max_URL_count = -1;

    //! whether input data allows to implement search
    //! it's set to false in cases of fault inputs
    bool m_can_start_scan = true;







    QEventLoop event;
};

#endif // SEARCHENGINE_H
