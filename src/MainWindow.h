#pragma once

#include "UrlTextFinder/UrlTextFinder.h"

#include <QDialog>

namespace Ui {
class MainWindow;
}

class MainWindow: public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startScan();
    void pauseResumeScan();
    void stopScan();

    void updateUi();

protected:
    virtual void timerEvent(QTimerEvent * event) override;

private:
    bool _validateInput();

private:
    Ui::MainWindow *_ui;
    int _updateScanResultsViewTimerId;

    UrlTextFinder _urlTextFinder;
};
