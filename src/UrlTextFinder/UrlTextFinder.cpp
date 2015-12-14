#include "UrlTextFinder.h"

UrlTextFinder::UrlTextFinder(QObject *parent /*= nullptr*/)
    : QObject(parent)
    , _model(_tasks, this)
    , _state(State::Waiting)
    , _nam(this)
    , _downloadManager(_nam, this)
    , _scanManager(this)
    , _downloadingTasksLimit(0)
    , _scanningTasksLimit(0)
    , _maximumTasksLimit(0)
{
    connect(&_downloadManager, SIGNAL(taskProgress(int,int)),
                               SLOT(onTaskDownloadProgress(int,int)));
    connect(&_downloadManager, SIGNAL(taskData(int,QByteArray)),
                               SLOT(onTaskDownloadData(int,QByteArray)));
    connect(&_downloadManager, SIGNAL(taskComplete(int)),
                               SLOT(onTaskDownloadComplete(int)));
    connect(&_downloadManager, SIGNAL(taskError(int,QString)),
                               SLOT(onTaskError(int,QString)));
    connect(&_downloadManager, SIGNAL(taskCancelled(int)),
                               SLOT(onTaskDownloadCancelled(int)));

    connect(&_scanManager, SIGNAL(taskProgress(int,int)),
                           SLOT(onTaskScanProgress(int,int)));
    connect(&_scanManager, SIGNAL(taskComplete(int,bool)),
                           SLOT(onTaskScanComplete(int,bool)));
    connect(&_scanManager, SIGNAL(taskCancelled(int)),
                           SLOT(onTaskScanCancelled(int)));
    connect(&_scanManager, SIGNAL(urlFound(QUrl)),
            SLOT(onUrlFound(QUrl)));
}

UrlTextFinder::~UrlTextFinder()
{
    _downloadManager.disconnect();
    _scanManager.disconnect();
}

UrlTextFinder::State UrlTextFinder::state() const
{
    return _state;
}

QAbstractTableModel *UrlTextFinder::tasksModel()
{
    return &_model;
}

////////////////////////////////////////////////////////////////////////////////

void UrlTextFinder::startScan(const QUrl &startingUrl,
                              const QByteArray &textToFind,
                              int downloadingTasksLimit,
                              int scanningTasksLimit,
                              int maximumTasksLimit)
{
    _downloadingTasksLimit = downloadingTasksLimit;
    _scanningTasksLimit = scanningTasksLimit;
    _maximumTasksLimit = maximumTasksLimit;

    _tasks.clear();
    _model.allTasksRemoved();

    _scanManager.setTextToFind(textToFind);
    onUrlFound(startingUrl);

    _setState(State::ScanInProgress);
}

void UrlTextFinder::pauseResumeScan()
{
    if (_state == State::ScanInProgress)
        _pauseScan();
    else
        _resumeScan();
}

void UrlTextFinder::stopScan()
{
    _downloadManager.stopAllTasks();

    _tasksForDownload.clear();
    _downloadingTasks.clear();
    _tasksForScan.clear();
    _scanningTasks.clear();

    _checkForFinishedState();
}

void UrlTextFinder::onUrlFound(const QUrl &url)
{
    if (_tasks.tasksCount() == _maximumTasksLimit)
        return;

    if (_tasks.isTaskPresent(url))
        return;

    auto taskId = _tasks.addNewTask(url);
    _model.taskAdded(taskId);
    _tasksForDownload.enqueue(taskId);
    _updateDownloadingTasks();
}

void UrlTextFinder::onTaskDownloadProgress(int taskId, int progress)
{
    _tasks.setTaskProgress(taskId, progress);
    _model.taskProgressChanged(taskId);
}

void UrlTextFinder::onTaskDownloadData(int taskId, const QByteArray &data)
{
    _tasks.appendTaskData(taskId, data);
}

void UrlTextFinder::onTaskDownloadComplete(int taskId)
{
    _downloadingTasks.remove(taskId);
    _tasksForScan.enqueue(taskId);

    _tasks.setTaskStatus(taskId, UrlTask::Status::QueuedForScan);
    _model.taskStatusChanged(taskId);

    _updateDownloadingTasks();
    _updateScanningTasks();

    if (_state == State::PauseInProgress)
        _checkForPausedState();
    else
        _checkForFinishedState();
}

void UrlTextFinder::onTaskDownloadCancelled(int taskId)
{
    _downloadingTasks.remove(taskId);
    _tasksForDownload.push_front(taskId);

    _tasks.setTaskStatus(taskId, UrlTask::Status::QueuedForDownload);
    _model.taskStatusChanged(taskId);

    _checkForPausedState();
}

void UrlTextFinder::onTaskScanProgress(int taskId, int progress)
{
    _tasks.setTaskProgress(taskId, progress);
    _model.taskProgressChanged(taskId);
}

void UrlTextFinder::onTaskScanComplete(int taskId, bool textFound)
{
    _scanningTasks.remove(taskId);

    auto status = textFound? UrlTask::Status::FinishedFound
                           : UrlTask::Status::FinishedNotFound;
    _tasks.setTaskStatus(taskId, status);
    _tasks.clearTaskData(taskId);
    _model.taskStatusChanged(taskId);

    _updateScanningTasks();

    if (_state == State::PauseInProgress)
        _checkForPausedState();
    else
        _checkForFinishedState();
}

void UrlTextFinder::onTaskScanCancelled(int taskId)
{
    _scanningTasks.remove(taskId);
    _tasksForScan.push_front(taskId);

    _tasks.setTaskStatus(taskId, UrlTask::Status::QueuedForScan);
    _model.taskStatusChanged(taskId);

    _checkForPausedState();
}

void UrlTextFinder::onTaskError(int taskId, const QString &errorString)
{
    _downloadingTasks.remove(taskId);

    _tasks.setTaskStatus(taskId, UrlTask::Status::Error);
    _tasks.setTaskDescription(taskId, errorString);
    _tasks.clearTaskData(taskId);
    _model.taskStatusChanged(taskId);

    _updateDownloadingTasks();

    _checkForFinishedState();
}

////////////////////////////////////////////////////////////////////////////////

void UrlTextFinder::_pauseScan()
{
    _setState(State::PauseInProgress);
    _downloadManager.cancelActiveTasks();
    _scanManager.cancelActiveTasks();
}

void UrlTextFinder::_resumeScan()
{
    _setState(State::ScanInProgress);
    _updateDownloadingTasks();
    _updateScanningTasks();
}

void UrlTextFinder::_updateDownloadingTasks()
{
    while((_downloadingTasks.size() < _downloadingTasksLimit) && (!_tasksForDownload.empty()))
    {
        int taskId = _tasksForDownload.dequeue();
        _downloadingTasks.insert(taskId);

        _tasks.setTaskStatus(taskId, UrlTask::Status::Downloading);
        _model.taskStatusChanged(taskId);

        _downloadManager.addTask(taskId, _tasks.taskUrl(taskId));
    }
}

void UrlTextFinder::_updateScanningTasks()
{
    while((_scanningTasks.size() < _scanningTasksLimit) && (!_tasksForScan.empty()))
    {
        int taskId = _tasksForScan.dequeue();
        _scanningTasks.insert(taskId);
        _tasks.setTaskStatus(taskId, UrlTask::Status::Scanning);
        _model.taskStatusChanged(taskId);

        _scanManager.addTask(taskId, _tasks.taskData(taskId));
    }
}

void UrlTextFinder::_checkForPausedState()
{
    // If there are no pending tasks - scan is paused.
    //
    if (_downloadingTasks.empty() && _scanningTasks.empty())
        _setState(State::ScanPaused);
}

void UrlTextFinder::_checkForFinishedState()
{
    //
    // If all queues are empty and there are no pending tasks - scan is finished.
    //
    if (!(_tasksForDownload.empty() && _downloadingTasks.empty()))
        return;

    if (!(_tasksForScan.empty() && _scanningTasks.empty()))
        return;

    _setState(State::Waiting);
}

void UrlTextFinder::_setState(UrlTextFinder::State state)
{
    if (_state == state)
        return;

    _state = state;
    emit stateChanged();
}
