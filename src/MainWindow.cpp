#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ProgressBarDelegate.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    : QDialog(parent)
    , _ui(new Ui::MainWindow())
    , _updateScanResultsViewTimerId(-1)
    , _urlTextFinder(this)
{
    _ui->setupUi(this);
    _ui->tvScanResults->setItemDelegateForColumn(1, new ProgressBarDelegate(this));
    _ui->tvScanResults->setModel(_urlTextFinder.tasksModel());
    _updateScanResultsViewTimerId = startTimer(41);

    connect(&_urlTextFinder, SIGNAL(stateChanged()), SLOT(updateUi()));
}

MainWindow::~MainWindow()
{
    killTimer(_updateScanResultsViewTimerId);
    _updateScanResultsViewTimerId = -1;

    _urlTextFinder.disconnect();

    auto delegate = _ui->tvScanResults->itemDelegateForColumn(1);
    _ui->tvScanResults->setItemDelegateForColumn(1, nullptr);
    _ui->tvScanResults->setModel(nullptr);

    delete delegate;
    delete _ui;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::startScan()
{
    if (!_validateInput())
        return;

    _urlTextFinder.startScan(QUrl(_ui->leStartingUrl->text()),
                             _ui->leTextToFind->text().toUtf8(),
                             _ui->sbDownloadingTasksLimit->value(),
                             _ui->sbScanningTasksLimit->value(),
                             _ui->sbMaximumTasksLimit->value());
}

void MainWindow::pauseResumeScan()
{
    _urlTextFinder.pauseResumeScan();
}

void MainWindow::stopScan()
{
    _urlTextFinder.stopScan();
}

void MainWindow::updateUi()
{
    switch (_urlTextFinder.state())
    {
    case UrlTextFinder::State::Waiting:
        _ui->gbScanOptions->setEnabled(true);
        _ui->btnStartScan->setEnabled(true);
        _ui->btnPauseResumeScan->setText(tr("Pause scan"));
        _ui->btnPauseResumeScan->setEnabled(false);
        _ui->btnStopScan->setEnabled(false);
        break;

    case UrlTextFinder::State::ScanInProgress:
        _ui->gbScanOptions->setEnabled(false);
        _ui->btnStartScan->setEnabled(false);
        _ui->btnPauseResumeScan->setText(tr("Pause scan"));
        _ui->btnPauseResumeScan->setEnabled(true);
        _ui->btnStopScan->setEnabled(true);
        break;

    case UrlTextFinder::State::PauseInProgress:
        _ui->gbScanOptions->setEnabled(false);
        _ui->btnStartScan->setEnabled(false);
        _ui->btnPauseResumeScan->setText(tr("Pausing scan..."));
        _ui->btnPauseResumeScan->setEnabled(false);
        _ui->btnStopScan->setEnabled(false);
        break;

    case UrlTextFinder::State::ScanPaused:
        _ui->gbScanOptions->setEnabled(false);
        _ui->btnStartScan->setEnabled(false);
        _ui->btnPauseResumeScan->setText(tr("Resume scan"));
        _ui->btnPauseResumeScan->setEnabled(true);
        _ui->btnStopScan->setEnabled(true);
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _updateScanResultsViewTimerId)
        _ui->tvScanResults->viewport()->update();
}

////////////////////////////////////////////////////////////////////////////////

bool MainWindow::_validateInput()
{
    QUrl startingUrl(_ui->leStartingUrl->text());
    if (startingUrl.isEmpty() || (!startingUrl.isValid()))
    {
        QMessageBox::critical(this, tr("Input error"), tr("Starting URL isn't valid."));
        return false;
    }

    if (_ui->leTextToFind->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Input error"), tr("Empty text to find."));
        return false;
    }

    return true;
}
