#include "resultsproxy.h"

namespace search
{

ResultsInLine::ResultsInLine(const QString& line, const QStringList& columns)
    : m_line(line)
    , m_columns(columns)
{
}

//==================================================================================================


ResultsProxy::ResultsProxy(const QString& url, QVariantList&& results)
    : m_url(url)
    , m_results_of_url(std::move(results))
{
}

} // search
