#pragma once

#include <QNetworkAccessManager>
#include <QUrl>

class UrlDownloader: public QObject
{
    Q_OBJECT

public:
    explicit UrlDownloader(int taskId, const QUrl &url, QNetworkAccessManager &nam, QObject *parent = nullptr);

    int taskId() const;

    void start();
    void stop();

signals:
    void taskProgress(int taskId, int progress);
    void taskData(int taskId, const QByteArray &data);
    void taskComplete(int taskId);
    void taskError(int taskId, const QString &errorString);

    void taskFinished(UrlDownloader *downloader);

private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onReadyRead();
    void onFinished();

private:
    int _taskId;
    QUrl _url;
    QNetworkAccessManager &_nam;
    QNetworkReply *_reply;
};
