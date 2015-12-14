#include "UrlDownloadManager.h"

#include <QDebug>
#include <QVector>

UrlDownloadManager::UrlDownloadManager(QNetworkAccessManager &nam, QObject *parent /*= nullptr*/)
    : QObject(parent)
    , _nam(nam)
{ }

UrlDownloadManager::~UrlDownloadManager()
{
    if (_downloaders.size())
        qWarning() << "[Warning]" << Q_FUNC_INFO << "deleting object with active tasks.";

    stopAllTasks();
}

void UrlDownloadManager::addTask(int taskId, const QUrl &url)
{
    auto downloader = new UrlDownloader(taskId, url, _nam, this);
    _downloaders.insert(downloader);

    connect(downloader, SIGNAL(taskProgress(int,int)),
                        SIGNAL(taskProgress(int,int)));
    connect(downloader, SIGNAL(taskData(int,QByteArray)),
                        SIGNAL(taskData(int,QByteArray)));
    connect(downloader, SIGNAL(taskComplete(int)),
                        SIGNAL(taskComplete(int)));
    connect(downloader, SIGNAL(taskError(int,QString)),
                        SIGNAL(taskError(int,QString)));

    connect(downloader, SIGNAL(taskFinished(UrlDownloader*)),
                        SLOT(onTaskFinished(UrlDownloader*)), Qt::QueuedConnection);

    downloader->start();
}

void UrlDownloadManager::cancelActiveTasks()
{
    QVector<int> tasksIds(_downloaders.size(), -1);
    int i = 0;

    for(const auto &downloader: _downloaders)
    {
        tasksIds[i++] = downloader->taskId();
        downloader->stop();
        downloader->disconnect();
        downloader->deleteLater();
    }
    _downloaders.clear();

    for(const auto &taskId: tasksIds)
        emit taskCancelled(taskId);
}

void UrlDownloadManager::stopAllTasks()
{
    for(const auto &downloader: _downloaders)
    {
        downloader->stop();
        downloader->disconnect();
        downloader->deleteLater();
    }
    _downloaders.clear();
}

////////////////////////////////////////////////////////////////////////////////

void UrlDownloadManager::onTaskFinished(UrlDownloader *downloader)
{
    downloader->disconnect();
    downloader->deleteLater();
    _downloaders.remove(downloader);
}
