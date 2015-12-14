#pragma once

#include <QAbstractTableModel>
#include <QVector>

#include "UrlTasksContainer.h"

class UrlTasksModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    UrlTasksModel(UrlTasksContainer &tasks, QObject *parent = nullptr);

    void taskAdded(int taskId);
    void taskStatusChanged(int taskId);
    void taskProgressChanged(int taskId);
    void taskDescriptionChanged(int taskId);
    void allTasksRemoved();

    // QAbstractItemModel interface
public:
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    static QString _UrlTaskStatusToString(UrlTask::Status status);

private:
    UrlTasksContainer &_tasks;
};

