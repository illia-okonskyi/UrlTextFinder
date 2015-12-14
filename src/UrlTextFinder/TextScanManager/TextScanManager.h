#pragma once

#include "TextScannerThread.h"

#include <QSet>

class TextScanManager: public QObject
{
    Q_OBJECT

public:
    explicit TextScanManager(QObject *parent = nullptr);
    ~TextScanManager();

    void setTextToFind(const QByteArray &textToFind);
    void addTask(int taskId, const QByteArray &text);
    void cancelActiveTasks();
    void stopAllTasks();

signals:
    void taskProgress(int taskId, int progress);
    void taskComplete(int taskId, bool textFound);
    void taskCancelled(int taskId);

    void urlFound(const QUrl &url);

private slots:
    void onTaskFinished(TextScannerThread *scanner);

private:
    TextScannerStopFlag _stopFlag;
    QByteArray _textToFind;
    QSet<TextScannerThread *> _scanners;
};

