#pragma once

#include <QUrl>
#include <QByteArray>

class UrlTask
{
public:
    enum class Status: char
    {
        QueuedForDownload = 0,
        Downloading,
        QueuedForScan,
        Scanning,
        FinishedFound,
        FinishedNotFound,
        Error
    };

public:
    UrlTask(const QUrl &url = QUrl());

    QUrl url() const;

    Status status() const;
    void setStatus(Status status);

    int progress() const;
    void setProgress(int progress);

    QByteArray data() const;
    void appendData(const QByteArray &data);
    void clearData();

    QString description() const;
    void setDescription(const QString &description);

private:
    QUrl _url;
    Status _status;
    int _progress;
    QByteArray _data;
    QString _description;
};
