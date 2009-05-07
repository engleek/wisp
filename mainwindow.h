#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gloox/gloox.h>
#include "comms.h"

#include <QMainWindow>
#include <QtGui>

#include "interface/identrequest.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();

    public slots:
        void setIdents();
        void about();

        void slotConnect();
        void slotConnected();
        void slotResourceBindError();
        void slotSessionCreateError();
        void slotItemSubscribed();
        void slotItemAdded();
        void slotItemUnsubscribed();
        void slotItemRemoved();
        void slotItemUpdated();
        void slotRoster( QStringList *newRoster );
        void slotRosterError();
        void slotRosterPresence( QString jid, QString resource );
        void slotRosterNonPresence();
        void slotSubscriptionRequest();
        void slotUnsubscriptionRequest();
        void slotMessage();

    private:
        void createActions();
        void createToolBars();
        void createDocks();

        IdentRequest *identDialog;

        QListWidget* chatList;

        QDockWidget* contactsDock;
        QListWidget* contactsList;

        QDockWidget* consoleDock;
        QListWidget* console;

        QToolBar *mainToolBar;

        QAction *connectAct;
        QAction *aboutAct;
        QAction *quitAct;

        QMap<QString, QListWidgetItem*> roster;

        Comms *r;
        QTimer *timer;
        QString username;
        QString userpass;
};
#endif
