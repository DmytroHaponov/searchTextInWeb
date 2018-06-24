#include "downloader.h"
#include <QNetworkReply>

#include <QEventLoop>
#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <memory>

DownLoader::DownLoader(const QString& url, QObject* parent) :
    QObject(parent)
  , m_url_str(url)
{
}

void DownLoader::run()
{
    QUrl url(m_url_str);
    QNetworkRequest request(url);
    std::unique_ptr< QNetworkAccessManager> m_qnam = std::make_unique<QNetworkAccessManager>();
    QNetworkReply* reply = m_qnam.get()->get(request);
    QEventLoop event;
    connect(reply, &QNetworkReply::downloadProgress, [this, &event](qint64 part, qint64 max)
    {
        event.processEvents();
        emit download_progress_changed(part, max, QString(""));
    });
    connect(reply, &QNetworkReply::finished, [this, &event]{
        emit download_finished(m_url_str);
        event.quit();});

    event.exec();


    QFileInfo fileInfo = url.path();
    QFile temp(QString("test/") + fileInfo.completeBaseName());
    if(!temp.open(QIODevice::WriteOnly| QIODevice::Text))
        return;
    temp.write(reply->readAll());
}
/*
 *shift
 *
if (m_nPos)
{
    QByteArray data;
    QString strData("bytes=" + QString::number(m_nPos) + "-");

    data = strData.toLatin1();
    request.setRawHeader("Range", data);
}

 */
