#-------------------------------------------------
#
# Project created by QtCreator 2015-11-15T13:19:51
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = UrlTextFinder
TEMPLATE = app

SOURCES += main.cpp\
           MainWindow.cpp \
           ProgressBarDelegate.cpp \
           UrlTextFinder/TextScanManager/TextScanManager.cpp \
           UrlTextFinder/TextScanManager/TextScanner.cpp \
           UrlTextFinder/TextScanManager/TextScannerStopCondition.cpp \
           UrlTextFinder/TextScanManager/TextScannerThread.cpp \
           UrlTextFinder/UrlDownloadManager/UrlDownloader.cpp \
           UrlTextFinder/UrlDownloadManager/UrlDownloadManager.cpp \
           UrlTextFinder/UrlTask.cpp \
           UrlTextFinder/UrlTasksContainer.cpp \
           UrlTextFinder/UrlTasksModel.cpp \
           UrlTextFinder/UrlTextFinder.cpp

HEADERS += MainWindow.h \
           ProgressBarDelegate.h \
           UrlTextFinder/TextScanManager/TextScanManager.h \
           UrlTextFinder/TextScanManager/TextScanner.h \
           UrlTextFinder/TextScanManager/TextScannerStopCondition.h \
           UrlTextFinder/TextScanManager/TextScannerThread.h \
           UrlTextFinder/UrlDownloadManager/UrlDownloader.h \
           UrlTextFinder/UrlDownloadManager/UrlDownloadManager.h \
           UrlTextFinder/UrlTask.h \
           UrlTextFinder/UrlTasksContainer.h \
           UrlTextFinder/UrlTasksModel.h \
           UrlTextFinder/UrlTextFinder.h

FORMS += MainWindow.ui
