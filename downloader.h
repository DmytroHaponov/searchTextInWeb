#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QRunnable>

class DownLoader : public QObject, public QRunnable
{
    Q_OBJECT
public:
    /*!
     * \brief ctor
     * \param url to download
     * \param parent for QObject
     */
    explicit DownLoader(const QString& url, QObject* parent = nullptr);

    /*!
     * \brief main method for QRunnable
     */
    void run() override;

signals:
    /*!
     * \brief signal to update progress bar for url download
     * \param percent - new value for progress bar
     * \param url - which progress bar to update
     */
    void download_progress_changed(qint64 part, qint64 max, QString url);

    /*!
     * \brief signal that download is complete
     * \param url that was downloaded
     */
    void download_finished(QString url);

private:
    //! url to download
    QString m_url_str;

    //! implements download of web resource


};

#endif // DOWNLOADER_H
