#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QRegularExpression>
#include <QPair>

//using Results = QVector<QPair<int /*line*/, int /*column*/>>;

class Scanner : public QObject
{
    Q_OBJECT
public:
    explicit Scanner(const QString& target_text, QObject *parent = nullptr);
    QVector<int> search_target_in_line(const QString& line);
    QStringList search_urls_in_line(const QString& line);

signals:
    //void found_target_text(Results results);

public slots:

private:
    QRegularExpression m_target_text_expr;
    QRegularExpression m_url_expr;
    static const QString url_pattern;
};

#endif // SCANNER_H
