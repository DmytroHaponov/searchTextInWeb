#include "downloader.h"
#include <QNetworkReply>

DownLoader::DownLoader(const QString& url, QObject* parent) :
    QObject(parent)
   , m_url_str(url)
{
}

void DownLoader::run()
{
    QUrl url(m_url_str);
    QNetworkRequest request(url);
    QNetworkAccessManager* m_qnam = new QNetworkAccessManager();
    QNetworkReply* reply = m_qnam->get(request);
    connect(reply, &QNetworkReply::downloadProgress, [this](qint64 part, qint64 max)
    {
        emit download_progress_changed(part, max, m_url_str);
    });
    connect(reply, &QNetworkReply::finished, [this]{ emit download_finished(m_url_str); });
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
