#include "UrlTasksContainer.h"

#include <stdexcept>

bool UrlTasksContainer::isTaskPresent(const QUrl &url) const
{
    return _urls.contains(url);
}

bool UrlTasksContainer::isTaskIdValid(int taskId) const
{
    return (taskId >= 0) && (taskId < _tasks.size());
}

int UrlTasksContainer::tasksCount() const
{
    return _tasks.size();
}

int UrlTasksContainer::addNewTask(const QUrl &url)
{
    int taskIndex = _tasks.size();
    _tasks.push_back(UrlTask(url));
    _urls.insert(url);
    return taskIndex;
}

QUrl UrlTasksContainer::taskUrl(int taskId) const
{
    _ensureTaskId(taskId);
    return _tasks[taskId].url();
}

UrlTask::Status UrlTasksContainer::taskStatus(int taskId) const
{
    _ensureTaskId(taskId);
    return _tasks[taskId].status();
}

void UrlTasksContainer::setTaskStatus(int taskId, UrlTask::Status status)
{
    _ensureTaskId(taskId);
    _tasks[taskId].setStatus(status);
}

int UrlTasksContainer::taskProgress(int taskId) const
{
    _ensureTaskId(taskId);
    return _tasks[taskId].progress();
}

void UrlTasksContainer::setTaskProgress(int taskId, int progress)
{
    _ensureTaskId(taskId);
    _tasks[taskId].setProgress(progress);
}

QByteArray UrlTasksContainer::taskData(int taskId) const
{
    _ensureTaskId(taskId);
    return _tasks[taskId].data();
}

void UrlTasksContainer::appendTaskData(int taskId, const QByteArray &data)
{
    _ensureTaskId(taskId);
    _tasks[taskId].appendData(data);
}

void UrlTasksContainer::clearTaskData(int taskId)
{
    _ensureTaskId(taskId);
    _tasks[taskId].clearData();
}

QString UrlTasksContainer::taskDescription(int taskId) const
{
    _ensureTaskId(taskId);
    return _tasks[taskId].description();
}

void UrlTasksContainer::setTaskDescription(int taskId, const QString &description)
{
    _ensureTaskId(taskId);
    _tasks[taskId].setDescription(description);
}

void UrlTasksContainer::clear()
{
    _urls.clear();
    _tasks.clear();
}

////////////////////////////////////////////////////////////////////////////////

void UrlTasksContainer::_ensureTaskId(int taskId) const
{
    if (!isTaskIdValid(taskId))
        throw std::runtime_error("Wrong task id");
}
