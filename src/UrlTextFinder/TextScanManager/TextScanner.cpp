#include "TextScanner.h"

#include <stdexcept>
#include <QHash>
#include <QRegExp>

TextScanner::TextScanner(const CheckStopFuncT &checkStop,
                         const ProgressFuncT &onProgress,
                         const UrlFoundFuncT &onUrlFound)
    : _checkStop(checkStop)
    , _onProgress(onProgress)
    , _onUrlFound(onUrlFound)
    , _progress(0)
{
    if (!_checkStop)
        throw std::invalid_argument("checkStop");
}

TextScanner::Result TextScanner::process(const QByteArray &text, const QByteArray &textToFind)
{
    Result result = _process(text, textToFind);

    switch (result)
    {
    case Result::Found:
    case Result::NotFound:
        if (_onUrlFound)
            _findUrls(text);
        break;
    default:
        break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

TextScanner::Result TextScanner::_process(const QByteArray &text, const QByteArray &textToFind)
{
    _progress = 0;

    auto textToFindSize = textToFind.size();
    auto totalComparesCount = text.size() - textToFindSize;

    auto textToFindHash = qHash(textToFind);

    for (auto i = 0; i < totalComparesCount; ++i)
    {
        StopFlag stopFlag = _checkStop();
        switch(stopFlag)
        {
        case StopFlag::Cancel:
            return Result::Cancelled;
        case StopFlag::Stop:
            return Result::Stopped;
        default:
            break;
        }

        auto textToCompareHash = qHash(text.mid(i, textToFindSize));

        if (textToFindHash == textToCompareHash)
            return Result::Found;

        auto newProgress = (i * 100) / totalComparesCount;
        if (newProgress != _progress)
        {
            _progress = newProgress;
            _callOnProgress(_progress);
        }
    }

    return Result::NotFound;
}

void TextScanner::_findUrls(const QByteArray &textToFind)
{
    QRegExp re("http://[\\w\\-\\._]+", Qt::CaseInsensitive);
    QString str(textToFind);

    int pos = 0;
    while ((pos = re.indexIn(str, pos)) != -1)
    {
        _onUrlFound(QUrl(re.cap(0)));
        pos += re.matchedLength();
    }
}

void TextScanner::_callOnProgress(int progress)
{
    if (_onProgress)
        _onProgress(progress);
}
