#include "scanner.h"
#include "scannerinline.h"
#include "resultsproxy.h"
#include "searchengine.h"
#include <QFileInfo>
#include <QUrl>

namespace search {

Scanner::Scanner(QObject* engine, const QString& url_str, const QString& target_text,
                 QObject *parent)
    : QObject(parent)
    , m_engine(engine)
    , m_url_str(url_str)
    , m_target_text(target_text)
{   
}

void Scanner::run()
{
    if (!open_downloaded_url())
    {
        return;
    }
    ScannerInLine scanner(m_target_text);
    QStringList new_urls;
    QVariantList results;
    int line_number = 0;
    char line_text[1024];
    while(m_input_file.readLine(line_text, sizeof(line_text)) != -1)
    {
        ++line_number;
        QString line_text_str(line_text);

        QStringList scan_results = scanner.search_target_in_line(line_text_str);
        if (scan_results.size())
        {
            ResultsInLine result(QString::number(line_number), scan_results);
            results.append(QVariant::fromValue(result));
        }

        QStringList found_urls = scanner.search_urls_in_line(line_text_str);
        if (!found_urls.empty())
        {
            new_urls.append(found_urls);
        }
    }
    if (!results.isEmpty())
    {
         QMetaObject::invokeMethod(m_engine, "append_new_results",
                                   Qt::QueuedConnection,
                                   Q_ARG(QString, m_url_str),
                                   Q_ARG(QVariantList, results));
    }
    QMetaObject::invokeMethod(m_engine, "add_new_urls",
                              Qt::QueuedConnection,
                              Q_ARG(QString, m_url_str),
                              Q_ARG(QStringList, new_urls));
}

bool Scanner::open_downloaded_url()
{
    QUrl url(m_url_str);
    QFileInfo fileInfo = url.path();
    QString auxilary_dir("downloads/");
    QString file_name = auxilary_dir + fileInfo.completeBaseName();
    if (file_name == auxilary_dir)
    {
        file_name += m_url_str.remove(QRegExp("[-:/.//]"));
    }
    m_input_file.setFileName(file_name);
    if (!m_input_file.open(QIODevice::ReadOnly))
    {
        qDebug()<< "could not read " << m_input_file.fileName();
        return false;
    }
    return true;
}

} //search
