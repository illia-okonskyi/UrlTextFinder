#pragma once

#include "UrlDownloader.h"

#include <QSet>

class UrlDownloadManager: public QObject
{
    Q_OBJECT

public:
    explicit UrlDownloadManager(QNetworkAccessManager &nam, QObject *parent = nullptr);
    ~UrlDownloadManager();

    void addTask(int taskId, const QUrl &url);
    void cancelActiveTasks();
    void stopAllTasks();

signals:
    void taskProgress(int taskId, int progress);
    void taskData(int taskId, const QByteArray &data);
    void taskComplete(int taskId);
    void taskError(int taskId, const QString &errorString);
    void taskCancelled(int taskId);

private slots:
    void onTaskFinished(UrlDownloader *downloader);

private:
    QNetworkAccessManager &_nam;
    QSet<UrlDownloader *> _downloaders;
};
