#include "searchengine.h"
#include "downloader.h"
#include "scanner.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "resultsproxy.h"

#include <QDebug>

namespace search {

SearchEngine::SearchEngine(QObject *parent) : QObject(parent)
{
    m_local_search_thread_pool.setMaxThreadCount(2); // not specified by requirements
}

void SearchEngine::download_page(const QString& page_name)
{
    DownLoader* downloader = new DownLoader(page_name);
    // downloader will be deleted by global Thread pool
    QThreadPool* global_thread_pool = QThreadPool::globalInstance();
    connect(downloader, &DownLoader::download_progress_changed,
            this, &SearchEngine::download_progress_changed);
    connect(downloader, &DownLoader::download_finished, this, &SearchEngine::on_page_downloaded);
    global_thread_pool->start(downloader);
}

void SearchEngine::on_main_URL_received(const QString& url)
{
    if (m_local_search_thread_pool.maxThreadCount() > m_max_URL_count)
    {
        m_can_start_scan = false;
        emit error_msg(QString("plz make threads quantity less than max URL count"));
    }
    if(!m_can_start_scan)
    {
        return;
    }
    m_starting_URL = url;
    download_page(m_starting_URL);
}

void SearchEngine::set_max_threads_count(const QString& count)
{
    int quantity = qstring_to_int(count, QString("threads quantity"));
    // thread pool will always use at least 1 thread
    // even if maxThreadCount limit is zero or negative.
    m_local_search_thread_pool.setMaxThreadCount(quantity);
}

void SearchEngine::set_target_text(const QString& text)
{
    m_target_text = text;
}

void SearchEngine::set_max_URL_quantity(const QString& count)
{
    m_max_URL_count = qstring_to_int(count, QString("max URL count"));
    m_processed.assign(m_max_URL_count, 0); // fill with theoretical max of zeros
}

void SearchEngine::on_page_downloaded(const QString& url)
{
    emit download_progress_changed(1, 1, url); // 1 out of 1, that is download complete
    if(!m_downloaded_graph.size())
    {
        // start first scan
        QUrl url(m_starting_URL);
        QFileInfo fileInfo = url.path();
        QString auxilary_dir("downloads");
        QDir cur_dir = QDir::current();
        cur_dir.mkdir(auxilary_dir);
        QString file_name = auxilary_dir + "/" + fileInfo.completeBaseName();
        QFile input_file(file_name);
        if (!input_file.open(QIODevice::ReadOnly))
        {
            qDebug()<< "could not read "<<input_file.fileName();
            return;
        }
        Scanner scanner(m_target_text);
        QStringList new_urls;
        int line_number = 0;
        char line_text[1024];
        while(input_file.readLine(line_text, sizeof(line_text)) != -1)
        {
            ++line_number;
            QString line_text_str(line_text);
            QStringList scan_results = scanner.search_target_in_line(line_text_str);
            if (scan_results.size())
            {
                ResultsProxy result(QString::number(line_number), scan_results);
                m_results.append(QVariant::fromValue(result));
            }
            new_urls.append(scanner.search_urls_in_line(line_text_str));
        }
        emit resultsChanged();
        m_downloaded_graph.insert(std::make_pair(m_starting_URL, std::move(new_urls)));
    }
    //m_downloaded_graph.at(url)
}

int SearchEngine::qstring_to_int(const QString& count, const QString& msg)
{
    if (count == QString("0"))
    {
        emit error_msg(QString("is that QA playing? Please don't set 0 to %1").arg(msg));
        m_can_start_scan = false;
        return 0;
    }
    int quantity = count.toInt();
    if (!quantity)
    {
        m_can_start_scan = false;
        emit error_msg(QString("Please write adequate int to %1").arg(msg));
    }
    return quantity;
}

} //search
