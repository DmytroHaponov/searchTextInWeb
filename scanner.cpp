#include "scanner.h"
#include <QRegularExpressionMatchIterator>
#include <QVector>

const QString Scanner::url_pattern =
        QString(
            "^"
            // protocol identifier
            "(?:(?:https?|ftp|http)://)"
            // user:pass authentication
            "(?:\\S+(?::\\S*)?@)?"
            "(?:"
            // IP address exclusion
            // private & local networks
            "(?!(?:10|127)(?:\\.\\d{1,3}){3})"
            "(?!(?:169\\.254|192\\.168)(?:\\.\\d{1,3}){2})"
            "(?!172\\.(?:1[6-9]|2\\d|3[0-1])(?:\\.\\d{1,3}){2})"
            // IP address dotted notation octets
            // excludes loopback network 0.0.0.0
            // excludes reserved space >= 224.0.0.0
            // excludes network & broacast addresses
            // (first & last IP address of each class)
            "(?:[1-9]\\d?|1\\d\\d|2[01]\\d|22[0-3])"
            "(?:\\.(?:1?\\d{1,2}|2[0-4]\\d|25[0-5])){2}"
            "(?:\\.(?:[1-9]\\d?|1\\d\\d|2[0-4]\\d|25[0-4]))"
            "|"
            // host name
            "(?:(?:[a-z\\u00a1-\\uffff0-9]-*)*[a-z\\u00a1-\\uffff0-9]+)"
            // domain name
            "(?:\\.(?:[a-z\\u00a1-\\uffff0-9]-*)*[a-z\\u00a1-\\uffff0-9]+)*"
            // TLD identifier
            "(?:\\.(?:[a-z\\u00a1-\\uffff]{2,}))"
            // TLD may end with dot
            "\\.?"
            ")"
            // port number
            "(?::\\d{2,5})?"
            // resource path
            "(?:[/?#]\\S*)?"
            "$");

Scanner::Scanner(const QString& target_text, QObject* parent) : QObject(parent)
  , m_target_text_expr(target_text)
  , m_url_expr(url_pattern,
               QRegularExpression::ExtendedPatternSyntaxOption |
               QRegularExpression::MultilineOption |
               QRegularExpression::DotMatchesEverythingOption |
               QRegularExpression::CaseInsensitiveOption)
{
}

QVector<int> Scanner::search_target_in_line(const QString& line)
{
    QRegularExpressionMatchIterator it = m_url_expr.globalMatch(line);
    QVector<int> found_positions;
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        found_positions.append(match.capturedStart());
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
