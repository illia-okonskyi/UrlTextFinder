#include "TextScannerThread.h"
#include <memory>

TextScannerThread::TextScannerThread(const TextScannerStopFlag &stopFlag,
                                     const QByteArray &textToFind,
                                     int taskId,
                                     const QByteArray &text,
                                     QObject *parent /*= nullptr*/)
    : QThread(parent)
    , _stopFlag(stopFlag)
    , _textToFind(textToFind)
    , _scanner(std::bind(TextScannerStopFlag::getValue, &_stopFlag),
               std::bind(TextScannerThread::_onScannerProgress, this, std::placeholders::_1),
               std::bind(TextScannerThread::_onScannerUrlFound, this, std::placeholders::_1))
    , _taskId(taskId)
    , _text(text)
{ }

void TextScannerThread::run()
{
    _run();
    emit taskFinished(this);
}

////////////////////////////////////////////////////////////////////////////////

void TextScannerThread::_run()
{
    TextScanner::Result result = _scanner.process(_text, _textToFind);
    switch (result)
    {
    case TextScanner::Result::Found:
        emit taskComplete(_taskId, true);
        return;
    case TextScanner::Result::NotFound:
        emit taskComplete(_taskId, false);
        return;
    case TextScanner::Result::Cancelled:
        emit taskCancelled(_taskId);
        return;
    case TextScanner::Result::Stopped:
        return;
    }
}

void TextScannerThread::_onScannerProgress(int progress)
{
    emit taskProgress(_taskId, progress);
}

void TextScannerThread::_onScannerUrlFound(const QUrl &url)
{
    emit urlFound(url);
}
