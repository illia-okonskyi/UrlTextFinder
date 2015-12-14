#pragma once

#include <QReadWriteLock>

#include "TextScanner.h"

class TextScannerStopFlag
{
public:
    TextScannerStopFlag();

    TextScanner::StopFlag getValue() const;
    void setValue(TextScanner::StopFlag value);

private:
    TextScanner::StopFlag _value;
    mutable QReadWriteLock _lock;
};

