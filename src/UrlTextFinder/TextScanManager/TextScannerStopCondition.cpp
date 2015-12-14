#include "TextScannerStopCondition.h"

#include <QReadLocker>
#include <QWriteLocker>

TextScannerStopFlag::TextScannerStopFlag()
    : _value(TextScanner::StopFlag::Running)
{ }

TextScanner::StopFlag TextScannerStopFlag::getValue() const
{
    QReadLocker locker(&_lock);
    return _value;
}

void TextScannerStopFlag::setValue(TextScanner::StopFlag value)
{
    QWriteLocker locker(&_lock);
    _value = value;
}
