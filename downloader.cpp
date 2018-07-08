#include "downloader.h"
#include <QNetworkReply>

#include <QEventLoop>
#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <memory>

namespace search {

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
    connect(reply, &QNetworkReply::downloadProgress, [this](qint64 part, qint64 max)
    {
        emit download_progress_changed(part, max, m_url_str);
    });
    connect(reply, &QNetworkReply::finished, [this, &event, url, &reply]
    {
        save_to_file(url, reply);
        emit download_finished(m_url_str);
        event.quit();
    });
    event.exec();    
}

void DownLoader::save_to_file(const QUrl& url, QNetworkReply *reply)
{
    QFileInfo fileInfo = url.path();
    QString auxilary_dir("downloads/");
    QString file_name = auxilary_dir + fileInfo.completeBaseName();
    if (file_name == auxilary_dir)
    {
        file_name += m_url_str.remove(QRegExp("[-:/.//]"));
    }
    QFile output_file(file_name);
    if(!output_file.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        qDebug()<< "couldn't write to "<< output_file.fileName();
        return;
    }
    output_file.write(reply->readAll());
}

} //search


/*
 *      shift
 *
if (m_nPos)
{
    QByteArray data;
    QString strData("bytes=" + QString::number(m_nPos) + "-");

    data = strData.toLatin1();
    request.setRawHeader("Range", data);
}

 */
