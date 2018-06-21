#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QRegularExpression>

class Scanner : public QObject
{
    Q_OBJECT
public:
    explicit Scanner(const QString& target_text, QObject *parent = nullptr);
    QVector<int> search_target_in_line(const QString& line);
    QStringList search_urls_in_line(const QString& line);

signals:

public slots:

private:
    QRegularExpression m_target_text_expr;
    QRegularExpression m_url_expr;
    static const QString url_pattern;
};

#endif // SCANNER_H
