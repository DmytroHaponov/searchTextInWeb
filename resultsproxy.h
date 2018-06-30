#pragma once

#include <QObject>

namespace search
{

class ResultsProxy
{
    Q_GADGET
    Q_PROPERTY(QString line MEMBER m_line)
    Q_PROPERTY(QStringList columns MEMBER m_columns)

public:
    //! default ctor
    ResultsProxy() = default;
    //! non-explicit ctor
    ResultsProxy(const QString& line, const QStringList& columns);

private:
    //! line with found target text
    QString m_line;

    //! column with found target text
    QStringList m_columns;
};

} //search

Q_DECLARE_METATYPE( search::ResultsProxy)
