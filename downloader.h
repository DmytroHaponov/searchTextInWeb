#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QRunnable>
#include "searchengine.h"

class DownLoader : public QRunnable//, public QObject
{
    //Q_OBJECT
public:
    explicit DownLoader(QString url, SearchEngine* engine);
    void run() override;
private:
    QString m_url;
    SearchEngine* m_engine;
};

#endif // DOWNLOADER_H
