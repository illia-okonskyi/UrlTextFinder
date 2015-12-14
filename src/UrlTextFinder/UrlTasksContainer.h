#pragma once

#include "UrlTask.h"

#include <QSet>
#include <QVector>

class UrlTasksContainer
{
public:
    bool isTaskPresent(const QUrl &url) const;
    bool isTaskIdValid(int taskId) const;

    int tasksCount() const;

    // Returns created task id.
    //
    int addNewTask(const QUrl &url);

    QUrl taskUrl(int taskId) const;

    UrlTask::Status taskStatus(int taskId) const;
    void setTaskStatus(int taskId, UrlTask::Status status);

    int taskProgress(int taskId) const;
    void setTaskProgress(int taskId, int progress);

    QByteArray taskData(int taskId) const;
    void appendTaskData(int taskId, const QByteArray &data);
    void clearTaskData(int taskId);

    QString taskDescription(int taskId) const;
    void setTaskDescription(int taskId, const QString &description);

    void clear();

private:
    void _ensureTaskId(int taskId) const;

private:
    typedef QSet<QUrl> QUrlSet;
    typedef QVector<UrlTask> UrlTaskVectorT;

private:
    QUrlSet _urls;
    UrlTaskVectorT _tasks;
};

