#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();

    public slots:
        void connecMsg();

    private:
        QListWidget* console;
};
#endif
