#include "UrlDownloader.h"

#include <QNetworkReply>

namespace
{
    inline int _calculateProgress(qint64 bytesReceived, qint64 bytesTotal)
    {
        if (!bytesTotal)
            return 0;

        if (bytesTotal == -1)
            return -1;

        return (bytesReceived * 100) /  bytesTotal;
    }
}

////////////////////////////////////////////////////////////////////////////////

UrlDownloader::UrlDownloader(int taskId, const QUrl &url, QNetworkAccessManager &nam, QObject *parent /*= nullptr*/)
    : QObject(parent)
    , _taskId(taskId)
    , _url(url)
    , _nam(nam)
    , _reply(nullptr)
{ }

int UrlDownloader::taskId() const
{
    return _taskId;
}

void UrlDownloader::start()
{
    _reply = _nam.get(QNetworkRequest(_url));

    connect(_reply, SIGNAL(downloadProgress(qint64,qint64)),
                    SLOT(onDownloadProgress(qint64,qint64)));
    connect(_reply, SIGNAL(readyRead()),
                    SLOT(onReadyRead()));
    connect(_reply, SIGNAL(finished()),
                    SLOT(onFinished()));
}

void UrlDownloader::stop()
{
    if (!_reply)
        return;

    _reply->disconnect();
    _reply->close();
    _reply->deleteLater();
    _reply = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void UrlDownloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit taskProgress(_taskId, _calculateProgress(bytesReceived, bytesTotal));
}

void UrlDownloader::onReadyRead()
{
    emit taskData(_taskId, _reply->readAll());
}

void UrlDownloader::onFinished()
{
    QNetworkReply::NetworkError errorCode = _reply->error();

    switch(errorCode)
    {
    case QNetworkReply::NoError:
        emit taskProgress(_taskId, 100);
        emit taskComplete(_taskId);
        break;
    default:
        emit taskError(_taskId, _reply->errorString());
        break;
    }

    _reply->disconnect();
    _reply->deleteLater();
    _reply = nullptr;

    emit taskFinished(this);
}
