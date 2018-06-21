#include "downloader.h"
//#include


#include <QDebug>


DownLoader::DownLoader(QString url, SearchEngine* engine) :
    m_url(url)
  , m_engine(engine)
{
}

void DownLoader::run()
{


qDebug()<<m_url;
}
