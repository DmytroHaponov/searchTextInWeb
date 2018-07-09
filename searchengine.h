#pragma once

#include <QObject>
#include <map>
#include <list>
#include <set>
#include <QQueue>
#include <QNetworkReply>
#include <QThreadPool>

namespace search
{

class SearchEngine : public QObject
{
    Q_OBJECT
    //! scan results: url page as header and lines + columns where text was found
    Q_PROPERTY(QVariantList results READ results NOTIFY resultsChanged)

public:
    explicit SearchEngine(QObject *parent = nullptr);
    ~SearchEngine();

    /*!
     * \brief download page to local file
     * \param page_name - url to download
     */
    void download_page(const QString& page_name);

    //! getter for results
    const QVariantList& results() const
    {
        return m_results;
    }
    //! user pressed Stop btn
    Q_INVOKABLE void stop();

signals:

    //! stop all active downloads after user pressed Stop btn
    void abort_download();

    /*!
     * \brief signal for QML to display error happened in C++ processing data passed from QML
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

    /*!
     * \brief for view to update target search results
     */
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
    void on_page_downloaded(const QString& url_str);

    /*!
     * \brief add new_urls found on page url_str
     * \param url_str - where new urls were found
     * \param new_urls - what urls were found
     */
    void on_new_urls_result(QString url_str, QStringList q_new_urls);

    /*!
     * \brief add new search results to display - order does not matter
     * \param url_str - for what url results should be displayed
     * \param new_results - lines and columns where target text was found
     */
    void append_new_results(QString url_str, QVariantList new_results);

private:
    /*!
     * \brief convert QString to int, emit errors if need
     * \param count - QString to convert
     * \param msg - custom part of message, describes source of request
     * \return result of convertion. Can return "0", but in that case engine waits new data input,
     *           there's error message on UI
     */
    int qstring_to_int(const QString& count, const QString& msg);

    /*!
     * \brief reaction to page download result or scan result
     * \param url_str - what page was downloaded or scanned
     */
    void process_new_event(const QString& url_str);

    /*!
     * \brief add found url to queue of scanning
     * \param q_new_urls - found urls
     */
    void do_add_new_urls(const QStringList& q_new_urls);

private:
    //! downloaded urls to be scanned
    QQueue<QString> m_queue_to_scan;

    //! downloaded urls that were scanned
    QSet<QString> m_scanned;

    //! downloaded urls
    QSet<QString> m_downloaded;

    //! Qt's global threadpool is used for downloading URLs
    QThreadPool* m_thread_pool_for_downloads = QThreadPool::globalInstance();

    //! thread pool for search in downloaded URLs
    QThreadPool m_thread_pool_for_local_search;

    //! start new search from children of this URL
    QString m_URL_to_scan;

    //! text to search
    QString m_target_text;

    //! allowed maximum of URLs to be scanned
    int m_max_URL_count = -1;

    //! whether input data allows to implement search
    //! it's set to false in cases of fault inputs
    bool m_can_start_scan = true;

    //! positions of found text - line and columns
    QVariantList m_results;

    //! current total number of urls being processed
    static int s_total_urls;

    //! app is stopped by user
    bool m_stopped = false;
};

} // search
