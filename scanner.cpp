#include "scanner.h"
#include <QRegularExpressionMatchIterator>
#include <QVector>

namespace search {

const QString Scanner::url_pattern =
        // most adequate pattern from StackOverFlow
        QString(
            "(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?"
            );

Scanner::Scanner(const QString& target_text)
  : m_target_text_expr(target_text)
  , m_url_expr(url_pattern,
               QRegularExpression::ExtendedPatternSyntaxOption |
               QRegularExpression::MultilineOption |
               QRegularExpression::DotMatchesEverythingOption |
               QRegularExpression::CaseInsensitiveOption)
{
}

QStringList Scanner::search_target_in_line(const QString& line)
{
    QRegularExpressionMatchIterator it = m_target_text_expr.globalMatch(line);
    QStringList found_positions;
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        found_positions.append(QString::number( match.capturedStart()));
    }
    return found_positions;
}

QStringList Scanner::search_urls_in_line(const QString& line)
{
    QRegularExpressionMatchIterator it = m_url_expr.globalMatch(line);
    QStringList URLs;
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        URLs.append(match.captured());
    }
    return URLs;
}

} //search
