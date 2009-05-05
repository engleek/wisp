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
        void connectDialog();
        void aboutDialog();

        void connecMsg();

    private:
        void createActions();
        void createToolBars();
        void createDocks();

        QFrame* centralWidget;
        QHBoxLayout* mainLayout;
        QListWidget* contactsList;
        QListWidget* chatList;

        QDockWidget* consoleDock;
        QListWidget* console;

        QToolBar *mainToolBar;

        QAction *connectAct;
        QAction *aboutAct;
        QAction *quitAct;
};
#endif
