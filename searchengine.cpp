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

int SearchEngine::s_total_urls = 0;

SearchEngine::SearchEngine(QObject *parent) : QObject(parent)
{
    m_thread_pool_for_local_search.setMaxThreadCount(1); // not specified by requirements

    QDir cur_dir = QDir::current();
    QString auxilary_dir("downloads");
    cur_dir.mkdir(auxilary_dir);
}

SearchEngine::~SearchEngine()
{
    QDir cur_dir = QDir::current();
    QString auxilary_dir("downloads");
    cur_dir.cd(auxilary_dir);
    cur_dir.removeRecursively();
}

void SearchEngine::download_page(const QString& page_name)
{
    DownLoader* downloader = new DownLoader(page_name);
    // downloader will be deleted by global Thread pool

    connect(downloader, &DownLoader::download_progress_changed,
            this, &SearchEngine::download_progress_changed, Qt::QueuedConnection);
    connect(downloader, &DownLoader::download_finished, this, &SearchEngine::on_page_downloaded,
            Qt::QueuedConnection);
    m_thread_pool_for_downloads->start(downloader);
}

void SearchEngine::on_main_URL_received(const QString& url)
{
    if (m_thread_pool_for_downloads->maxThreadCount() > m_max_URL_count)
    {
        m_can_start_scan = false;
        emit error_msg(QString("plz make threads quantity less than max URL count"));
    }
    if(!m_can_start_scan)
    {
        return;
    }
    m_URL_to_scan = url;
    download_page(m_URL_to_scan);
}

void SearchEngine::set_max_threads_count(const QString& count)
{
    int quantity = qstring_to_int(count, QString("threads quantity"));
    // thread pool will always use at least 1 thread
    // even if maxThreadCount limit is zero or negative.
    m_thread_pool_for_downloads->setMaxThreadCount(quantity);
}

void SearchEngine::set_target_text(const QString& text)
{
    m_target_text = text;
}

void SearchEngine::set_max_URL_quantity(const QString& count)
{
    m_max_URL_count = qstring_to_int(count, QString("max URL count"));
    //m_scanned.assign(m_max_URL_count, 0); // fill with theoretical max of zeros
}

void SearchEngine::on_page_downloaded(const QString& url_str)
{
    emit download_progress_changed(1, 1, url_str); // 1 out of 1, that is download complete

    m_downloaded.insert(url_str);
    process_new_event(url_str);
}

void SearchEngine::on_new_urls_result(QString url_str, QStringList q_new_urls)
{
    m_scanned.insert(url_str);

    if(s_total_urls == m_max_URL_count)
    {
        qDebug()<< "received new urls while limit is already reached";
        process_new_event(url_str);
        return;
    }

    do_add_new_urls(q_new_urls);

    if (m_queue_to_scan.isEmpty())
    {
        qDebug()<<"nothing new to scan";
        return;
    }
    m_URL_to_scan = m_queue_to_scan.dequeue();

    if ( m_downloaded.contains(m_URL_to_scan) )
    {
        Scanner* scanner = new Scanner(this, m_URL_to_scan, m_target_text);
        // scanner will be deleted by QThreadPool
        m_thread_pool_for_local_search.start(scanner);
        return;
    }
    qDebug() << "no new downloaded urls?";
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
        emit error_msg(QString("is that QA playing? Please don't set %1 to 0").arg(msg));
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

void SearchEngine::process_new_event(const QString& url_str)
{
    if ( url_str == m_URL_to_scan && m_downloaded.contains(m_URL_to_scan) )
    {
        Scanner* scanner = new Scanner(this, m_URL_to_scan, m_target_text);
        // scanner will be deleted by QThreadPool
        m_thread_pool_for_local_search.start(scanner);
        if (m_queue_to_scan.size())
        {
            m_URL_to_scan = m_queue_to_scan.dequeue();
        }
        else if (m_downloaded.size() != 1)
        {
            m_URL_to_scan.clear(); // end
        }
        return;
    }
    qDebug() << "early url";
}

void SearchEngine::do_add_new_urls(const QStringList& q_new_urls)
{
    std::list<QString> new_urls = q_new_urls.toStdList();
    int quantity_to_add = new_urls.size();
    std::list<QString> allowed_list;
    int allowed_quantity = new_urls.size();
    if ( m_max_URL_count <  s_total_urls + quantity_to_add )
    {
        allowed_quantity = m_max_URL_count - s_total_urls;
        qDebug()<<"enough: s_total_urls is "<<s_total_urls<<" quantity_to_add is "<<quantity_to_add;
    }
    allowed_list.splice(allowed_list.begin(),
                        new_urls, new_urls.cbegin(), std::next(new_urls.begin(), allowed_quantity));
    s_total_urls += allowed_list.size();

    for(const auto& url : allowed_list)
    {
        if ( !m_downloaded.contains(url) )
        {
            download_page(url);
        }

        if ( !m_scanned.contains(url) && !m_queue_to_scan.contains(url) )
        {
            m_queue_to_scan.append(url);
        }
    }
}

} //search
