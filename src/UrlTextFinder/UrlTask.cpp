#include "UrlTask.h"

UrlTask::UrlTask(const QUrl &url /*= QUrl()*/)
    : _url(url)
    , _status(Status::QueuedForDownload)
    , _progress(0)
{ }

QUrl UrlTask::url() const
{
    return _url;
}

UrlTask::Status UrlTask::status() const
{
    return _status;
}

void UrlTask::setStatus(UrlTask::Status status)
{
    _status = status;

    switch(_status)
    {
    case Status::Downloading:
        setProgress(-1);
        break;
    case Status::QueuedForDownload:
    case Status::QueuedForScan:
    case Status::Scanning:
        setProgress(0);
        break;
    default:
        break;
    }
}

int UrlTask::progress() const
{
    return _progress;
}

void UrlTask::setProgress(int progress)
{
    _progress = progress;
}

QByteArray UrlTask::data() const
{
    return _data;
}

void UrlTask::appendData(const QByteArray &data)
{
    _data.append(data);
}

void UrlTask::clearData()
{
    _data.clear();
}

QString UrlTask::description() const
{
    return _description;
}

void UrlTask::setDescription(const QString &description)
{
    _description = description;
}
