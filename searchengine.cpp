#include "searchengine.h"
#include "downloader.h"
#include "scannerinline.h"
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

    connect(downloader, &DownLoader::download_progress_changed,
            this, &SearchEngine::download_progress_changed, Qt::QueuedConnection);
    connect(downloader, &DownLoader::download_finished, this, &SearchEngine::on_page_downloaded,
            Qt::QueuedConnection);
    m_global_thread_pool->start(downloader);
}

void SearchEngine::on_main_URL_received(const QString& url)
{
    if (m_global_thread_pool->maxThreadCount() > m_max_URL_count)
    {
        m_can_start_scan = false;
        emit error_msg(QString("plz make threads quantity less than max URL count"));
    }
    if(!m_can_start_scan)
    {
        return;
    }
    m_current_parent_URL = url;
    download_page(m_current_parent_URL);
}

void SearchEngine::set_max_threads_count(const QString& count)
{
    int quantity = qstring_to_int(count, QString("threads quantity"));
    // thread pool will always use at least 1 thread
    // even if maxThreadCount limit is zero or negative.
    m_global_thread_pool->setMaxThreadCount(quantity);
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

void SearchEngine::on_page_downloaded(const QString& url_str)
{
    emit download_progress_changed(1, 1, url_str); // 1 out of 1, that is download complete
    if(!m_downloaded_graph.size())
    {
        // start first scan
        QDir cur_dir = QDir::current();
        QString auxilary_dir("downloads");
        cur_dir.mkdir(auxilary_dir);
        Scanner* scanner = new Scanner(this, url_str, m_target_text);
        // scanner will be deleted by QThreadPool
        m_local_search_thread_pool.start(scanner);


                                    //TODO
        // m_work_queue.append(new_urls);
        // m_downloaded_graph.insert(std::make_pair(m_current_parent_URL, std::move(new_urls)));

        // m_processed.push_back(m_current_parent_URL);
    }
    //m_downloaded_graph.at(url)
}

void SearchEngine::append_new_results(QString url_str, QVariantList new_results)
{
    m_results.append( QVariant::fromValue( ResultsProxy(url_str, std::move(new_results))));
    emit resultsChanged();
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
