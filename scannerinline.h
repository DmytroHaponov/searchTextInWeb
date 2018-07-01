#pragma once

#include <QObject>
#include <QRegularExpression>

namespace search {

class ScannerInLine
{
public:
    explicit ScannerInLine(const QString& target_text);

    /**
     * @brief searches target text in a single line
     * @param line in which search is implemented
     * @return list of positions of found targets
     */
    QStringList search_target_in_line(const QString& line);

    /**
     * @brief searches url pattern in a single line
     * @param line in which search for URLs is implemented
     * @return list of found URLs
     */
    QStringList search_urls_in_line(const QString& line);

private:
    //! regexp to search target text
    QRegularExpression m_target_text_expr;

    //! regexp to search URL
    QRegularExpression m_url_expr;

    //! pattern to search URL in text
    static const QString url_pattern;
};

} //search
