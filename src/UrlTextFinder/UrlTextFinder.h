#pragma once

#include "UrlTasksContainer.h"
#include "UrlTasksModel.h"
#include "UrlDownloadManager/UrlDownloadManager.h"
#include "TextScanManager/TextScanManager.h"

#include <QQueue>

class UrlTextFinder: public QObject
{
    Q_OBJECT

public:
    enum class State: char
    {
        Waiting = 0,
        ScanInProgress,
        PauseInProgress,
        ScanPaused
    };

public:
    explicit UrlTextFinder(QObject *parent = nullptr);
    virtual ~UrlTextFinder();

    State state() const;
    QAbstractTableModel *tasksModel();

public slots:
    void startScan(const QUrl &startingUrl,
                   const QByteArray &textToFind,
                   int downloadingTasksLimit,
                   int scanningTasksLimit,
                   int maximumTasksLimit);
    void pauseResumeScan();
    void stopScan();

signals:
    void stateChanged();

private slots:
    void onUrlFound(const QUrl &url);

    void onTaskDownloadProgress(int taskId, int progress);
    void onTaskDownloadData(int taskId, const QByteArray &data);
    void onTaskDownloadComplete(int taskId);
    void onTaskDownloadCancelled(int taskId);

    void onTaskScanProgress(int taskId, int progress);
    void onTaskScanComplete(int taskId, bool textFound);
    void onTaskScanCancelled(int taskId);

    void onTaskError(int taskId, const QString &errorString);

private:
    void _pauseScan();
    void _resumeScan();

    void _updateDownloadingTasks();
    void _updateScanningTasks();

    void _checkForPausedState();
    void _checkForFinishedState();

    void _setState(State state);

private:
    UrlTasksContainer _tasks;
    UrlTasksModel _model;

    QQueue<int> _tasksForDownload;
    QSet<int> _downloadingTasks;
    QQueue<int> _tasksForScan;
    QSet<int> _scanningTasks;

    State _state;

    QNetworkAccessManager _nam;
    UrlDownloadManager _downloadManager;
    TextScanManager _scanManager;

    int _downloadingTasksLimit;
    int _scanningTasksLimit;
    int _maximumTasksLimit;
};
