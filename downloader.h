#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QRunnable>

class DownLoader : public QRunnable
{
public:
    explicit DownLoader(QString url);
    void run() override;
private:
    QString m_url;
};

#endif // DOWNLOADER_H
