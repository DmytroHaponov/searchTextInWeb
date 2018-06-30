#pragma once

#include <QObject>
#include <QRegularExpression>
#include <QPair>

namespace search {

class Scanner
{
public:
    explicit Scanner(const QString& target_text);
    QStringList search_target_in_line(const QString& line);
    QStringList search_urls_in_line(const QString& text);

private:
    QRegularExpression m_target_text_expr;
    QRegularExpression m_url_expr;
    static const QString url_pattern;
};

} //search
