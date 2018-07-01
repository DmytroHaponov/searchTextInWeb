#pragma once

#include <QRunnable>
#include <QObject>
#include <QFile>

namespace search {

class Scanner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Scanner(QObject* engine, const QString& url_str, const QString& target_text,
            QObject* parent = nullptr);

    /*!
     * \brief main method to start QRunnable
     */
    void run() override;

private:
    /*!
     * \brief open file of downloaded URL
     * \return success of opening file
     */
    bool open_downloaded_url();

private:
    //! invoke slots of SearchEngine
    QObject* m_engine;

    //! URL to process
    QString m_url_str;

    //! text to search
    QString m_target_text;

    //! downloaded URL file
    QFile m_input_file;
};

} // search
