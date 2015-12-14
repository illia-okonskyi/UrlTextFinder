#include "UrlTasksModel.h"

UrlTasksModel::UrlTasksModel(UrlTasksContainer &tasks, QObject *parent /*= nullptr*/)
    : QAbstractTableModel(parent)
    , _tasks(tasks)
{ }

void UrlTasksModel::taskAdded(int taskId)
{
    beginInsertRows(QModelIndex(), taskId, taskId);
    endInsertRows();
}

void UrlTasksModel::taskStatusChanged(int taskId)
{
    // When task status is changed - task description may be also changed

    QModelIndex dataChangedTopLeftIndex = index(taskId, 0);
    QModelIndex dataChangedBottomRightIndex = index(taskId, 1);
    emit dataChanged(dataChangedTopLeftIndex, dataChangedBottomRightIndex);
}

void UrlTasksModel::taskProgressChanged(int taskId)
{
    QModelIndex dataChangedIndex = index(taskId, 1);
    emit dataChanged(dataChangedIndex, dataChangedIndex);
}

void UrlTasksModel::taskDescriptionChanged(int taskId)
{
    QModelIndex dataChangedIndex = index(taskId, 1);
    emit dataChanged(dataChangedIndex, dataChangedIndex);
}

void UrlTasksModel::allTasksRemoved()
{
    beginResetModel();
    endResetModel();
}

////////////////////////////////////////////////////////////////////////////////

Qt::ItemFlags UrlTasksModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren);
}

int UrlTasksModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 3;
}

int UrlTasksModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _tasks.tasksCount();
}

QVariant UrlTasksModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int taskId = index.row();
        UrlTask::Status taskStatus = _tasks.taskStatus(taskId);
        switch(index.column())
        {
        case 0:
            return _UrlTaskStatusToString(taskStatus);
        case 1:
            if ((taskStatus == UrlTask::Status::Downloading) || (taskStatus == UrlTask::Status::Scanning))
                return _tasks.taskProgress(taskId);
            else if (taskStatus == UrlTask::Status::Error)
                return _tasks.taskDescription(taskId);
            else
                return _UrlTaskStatusToString(taskStatus);
        case 2:
            return _tasks.taskUrl(taskId);
        default:
            return QVariant();
        }
    }

    return QVariant();
}


QVariant UrlTasksModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
    {
        switch(section)
        {
        case 0:
            return tr("Status");
        case 1:
            return tr("Description");
        case 2:
            return tr("URL");
        default:
            return QVariant();
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

////////////////////////////////////////////////////////////////////////////////

QString UrlTasksModel::_UrlTaskStatusToString(UrlTask::Status status)
{
    switch(status)
    {
    case UrlTask::Status::QueuedForDownload:
        return tr("Queued for download");
    case UrlTask::Status::Downloading:
        return tr("Downloading");
    case UrlTask::Status::QueuedForScan:
        return tr("Queued for scan");
    case UrlTask::Status::Scanning:
        return tr("Scanning");
    case UrlTask::Status::FinishedFound:
        return tr("Text found");
    case UrlTask::Status::FinishedNotFound:
        return tr("Text not found");
    case UrlTask::Status::Error:
        return tr("Error");
    default:
        return tr("<UNKNOWN_STATUS>");
    }
}
