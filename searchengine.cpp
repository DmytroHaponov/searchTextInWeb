#include "searchengine.h"
#include "downloader.h"


#include <QDebug>


SearchEngine::SearchEngine(QObject *parent) : QObject(parent)
{

}

void SearchEngine::on_main_URL_received(QString url)
{
    if (m_thread_pool.maxThreadCount() > m_max_URL_count)
    {
        m_can_start_scan = false;
        emit error_msg(QString("plz make threads quantity less than max URL count"));
    }
    if(!m_can_start_scan)
    {
        return;
    }
    m_starting_URL = url;

}

void SearchEngine::set_max_threads_count(QString count)
{
    int quantity = qstring_to_int(count, QString("threads quantity"));
    // thread pool will always use at least 1 thread
    // even if maxThreadCount limit is zero or negative.
    m_thread_pool.setMaxThreadCount(quantity);
}

void SearchEngine::set_target_text(QString text)
{
    m_target_text = text;
}

void SearchEngine::set_max_URL_quantity(QString count)
{
    m_max_URL_count = qstring_to_int(count, QString("max URL count"));
}

int SearchEngine::qstring_to_int(QString count, QString msg)
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
