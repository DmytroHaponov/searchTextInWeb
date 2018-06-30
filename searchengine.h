#pragma once

#include <QObject>
#include <map>
#include <list>
#include <queue>
#include <QNetworkReply>
#include <QThreadPool>

namespace search
{

class SearchEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList results READ results NOTIFY resultsChanged)
public:
    explicit SearchEngine(QObject *parent = nullptr);

    /*!
     * \brief download page to local file
     * \param page_name - url to download
     */
    void download_page(const QString& page_name);

    const QVariantList& results() const
    {
        return m_results;
    }

signals:
    /*!
     * \brief error_msg - signal for QML to display error in C++ processing data passed from QML
     * \param msg - message describing error
     */
    void error_msg(QString msg);

    /*!
     * \brief update progress bar for downloading url
     * \param part - how much has been downloaded
     * \param max - what is 100%
     * \param url - what page
     */
    void download_progress_changed(qint64 part, qint64 max, QString url);

    void resultsChanged();

public slots:
    /*!
     * \brief user input starting URL
     * \param url - where search starts
     */
    void on_main_URL_received(const QString& url);

    /*!
     * \brief set maximum number of simultaneously downloading threads
     * \param count - number to set
     */
    void set_max_threads_count(const QString& count);

    /*!
     * \brief set what text to search
     * \param text - what to search
     */
    void set_target_text(const QString& text);

    /*!
     * \brief set when to stop in case urls don't stop being found
     * \param count - maximum number of urls
     */
    void set_max_URL_quantity(const QString& count);

    /*!
     * \brief once url is downloaded - start new scan &/or download
     * \param url - what page has been downloaded
     */
    void on_page_downloaded(const QString& url);

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
    //! QString - parent url, where link was found
    std::map<QString /*parent_url*/, QStringList> m_downloaded_graph;
    std::queue<QString> m_work_queue;
    std::vector<int> m_processed;

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

    //! positions of found text - line and columns
    QVariantList m_results;
};

} // search
