#pragma once

#include <QByteArray>
#include <QUrl>

#include <functional>

// NOTE: Naive search algorithm realization
//
class TextScanner
{
public:
    enum class StopFlag: char
    {
        Running = 0,
        Stop,
        Cancel
    };

    enum class Result: char
    {
        Found = 0,
        NotFound,
        Stopped,
        Cancelled
    };

    typedef std::function<StopFlag()> CheckStopFuncT;
    typedef std::function<void(int)> ProgressFuncT;
    typedef std::function<void(const QUrl &)> UrlFoundFuncT;

public:
    TextScanner(const CheckStopFuncT &checkStop,
                const ProgressFuncT &onProgress,
                const UrlFoundFuncT &onUrlFound);

    Result process(const QByteArray &text, const QByteArray &textToFind);

private:
    Result _process(const QByteArray &text, const QByteArray &textToFind);
    void _findUrls(const QByteArray &text);

    void _callOnProgress(int progress);

private:
    CheckStopFuncT _checkStop;
    ProgressFuncT _onProgress;
    UrlFoundFuncT _onUrlFound;
    int _progress;
};
