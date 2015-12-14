#include "TextScanManager.h"

#include <QDebug>

TextScanManager::TextScanManager(QObject *parent /*= nullptr*/)
    : QObject(parent)
{ }

TextScanManager::~TextScanManager()
{
    if (_scanners.size())
        qWarning() << "[Warning]" << Q_FUNC_INFO << "deleting object with active tasks.";

    for(const auto& scanner: _scanners)
    {
        scanner->disconnect();
        scanner->terminate();
        scanner->deleteLater();
    }
    _scanners.clear();
}

void TextScanManager::setTextToFind(const QByteArray &textToFind)
{
    _textToFind = textToFind;
}

void TextScanManager::addTask(int taskId, const QByteArray &text)
{
    if (_stopFlag.getValue() != TextScanner::StopFlag::Running)
        _stopFlag.setValue(TextScanner::StopFlag::Running);

    auto scanner = new TextScannerThread(_stopFlag, _textToFind, taskId, text, this);
    _scanners.insert(scanner);

    connect(scanner, SIGNAL(taskProgress(int,int)),
                     SIGNAL(taskProgress(int,int)), Qt::QueuedConnection);
    connect(scanner, SIGNAL(taskComplete(int,bool)),
                     SIGNAL(taskComplete(int,bool)), Qt::QueuedConnection);
    connect(scanner, SIGNAL(taskCancelled(int)),
                     SIGNAL(taskCancelled(int)), Qt::QueuedConnection);
    connect(scanner, SIGNAL(urlFound(QUrl)),
                     SIGNAL(urlFound(QUrl)), Qt::QueuedConnection);

    connect(scanner, SIGNAL(taskFinished(TextScannerThread*)),
                     SLOT(onTaskFinished(TextScannerThread*)), Qt::QueuedConnection);

    scanner->start();
}

void TextScanManager::cancelActiveTasks()
{
    _stopFlag.setValue(TextScanner::StopFlag::Cancel);
}

void TextScanManager::stopAllTasks()
{
    _stopFlag.setValue(TextScanner::StopFlag::Stop);
}

////////////////////////////////////////////////////////////////////////////////

void TextScanManager::onTaskFinished(TextScannerThread *scanner)
{
    scanner->disconnect();
    scanner->deleteLater();
    _scanners.remove(scanner);
}
