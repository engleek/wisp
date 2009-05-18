#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gloox/gloox.h>
#include "../comms.h"

#include <QMainWindow>
#include <QString>
#include <QListWidget>
#include <QDockWidget>
#include <QAction>
#include <QMap>
#include <QTimer>
#include <QToolBar>

#include "connectbox.h"
#include "contact.h"
#include "chatbox.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();

    public slots:
        void setIdents( QString name, QString pass );
        void about();

        void slotToggleConsole();
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
        void slotRosterPresence( QString jid, QString msg );
        void slotRosterNonPresence();
        void slotSubscriptionRequest();
        void slotUnsubscriptionRequest();
        void slotVCardReceived( QString jid, QString name );
        void slotMessage( QString from, QString body );
        void slotSendMsg( QString jid, QString msg );

        void slotShowChat();

    private:
        void createActions();
        void createToolBars();
        void createDocks();

        ConnectBox *connectBox;

        QListWidget* contactsList;

        QDockWidget* consoleDock;
        QListWidget* console;

        QToolBar *mainToolBar;

        QAction *connectAct;
        QAction *toggleConsoleAct;
        QAction *aboutAct;
        QAction *showChatAct;
        QAction *quitAct;

        QMap<QString, Contact*> roster;
        QMap<QString, ChatBox*> conversations;

        Comms *r;
        QTimer *timer;
        QString username;
        QString userpass;
};
#endif
