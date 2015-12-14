#pragma once

#include "TextScannerStopCondition.h"

#include <QThread>
#include <QByteArray>
#include <QUrl>

class TextScannerThread : public QThread
{
    Q_OBJECT

public:
    explicit TextScannerThread(const TextScannerStopFlag &stopFlag,
                               const QByteArray &textToFind,
                               int taskId,
                               const QByteArray &text,
                               QObject *parent = nullptr);
signals:
    void taskProgress(int taskId, int progress);
    void taskComplete(int taskId, bool textFound);
    void taskCancelled(int taskId);
    void urlFound(const QUrl &url);

    void taskFinished(TextScannerThread *scanner);

private:
    virtual void run() override;
    void _run();

    void _onScannerProgress(int progress);
    void _onScannerUrlFound(const QUrl &url);

private:
    const TextScannerStopFlag &_stopFlag;
    QByteArray _textToFind;
    TextScanner _scanner;
    int _taskId;
    QByteArray _text;
};

