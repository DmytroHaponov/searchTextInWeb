#pragma once

#include <QObject>
#include <QVariant>
#include <QVariantList>

namespace search
{

class ResultsInLine
{
    Q_GADGET
    Q_PROPERTY(QString line MEMBER m_line)
    Q_PROPERTY(QStringList columns MEMBER m_columns)

public:
    //! default ctor
    ResultsInLine() = default;
    //! non-explicit ctor
    ResultsInLine(const QString& line, const QStringList& columns);

private:
    //! line with found target text
    QString m_line;

    //! column with found target text
    QStringList m_columns;
};

//==================================================================================================
//==================================================================================================

class ResultsProxy
{
    Q_GADGET
    Q_PROPERTY(QString url MEMBER m_url)
    Q_PROPERTY(QVariantList results_in_line MEMBER m_results)
public:

    ResultsProxy() = default;

    explicit ResultsProxy(const QString& url, QVariantList&& results);
private:
    QString m_url;
    QVariantList m_results;
};

} //search

Q_DECLARE_METATYPE( search::ResultsInLine)
Q_DECLARE_METATYPE( search::ResultsProxy)
