#include "downloader.h"

#include <QThread>
#include <QDebug>

DownLoader::DownLoader(QString url) :
    m_url(url)
{
}

void DownLoader::run()
{
    QThread::sleep(2);

}
