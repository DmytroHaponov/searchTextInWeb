#include "downloader.h"
#include <QNetworkReply>

#include <QEventLoop>
#include <QThread>
#include <QFile>

DownLoader::DownLoader(const QString& url, QObject* parent) :
    QObject(parent)
   , m_url_str(url)
{
}

void DownLoader::run()
{
    //emit download_progress_changed(10, 100, m_url_str);
    QUrl url(m_url_str);
    QNetworkRequest request(url);
    QNetworkAccessManager* m_qnam = new QNetworkAccessManager();
    QNetworkReply* reply = m_qnam->get(request);
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

    QFile temp("html.out");
    if(!temp.open(QIODevice::WriteOnly/*| QIODevice::Text*/))
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


progress
QNetworkReply *reply = NetworkManager->get(downloadRequest);

    connect(reply, &QNetworkReply::downloadProgress,this, &NetworkAccess::downloadProgress);
void NetworkAccess::downloadProgress(qint64 ist, qint64 max)
{
    pBar->setRange(0,max);
    pBar->setValue(ist);
    if(max < 0) hideProgress();
}
 */
