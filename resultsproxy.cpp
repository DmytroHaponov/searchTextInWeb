#include "resultsproxy.h"

namespace search
{

ResultsProxy::ResultsProxy(const QString& line, const QStringList& columns)
    : m_line(line)
    , m_columns(columns)
{
}

} // search
