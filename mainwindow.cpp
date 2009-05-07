#include <QtGui>
#include <comms.h>

#include <gloox/gloox.h>
#include <gloox/jid.h>
#include <gloox/rostermanager.h>
#include <gloox/presencehandler.h>

#include "mainwindow.h"
#include "interface/identrequest.h"

MainWindow::MainWindow()
{
    r = new Comms();
    identDialog = new IdentRequest();

    createActions();
    createToolBars();
    createDocks();

    chatList = new QListWidget();
    setCentralWidget( chatList );

    QObject::connect( r, SIGNAL( sigConnected() ), this, SLOT( slotConnected() ) );
    QObject::connect( r, SIGNAL( sigRoster( QStringList* ) ), this, SLOT( slotRoster( QStringList* ) ) );
    QObject::connect( r, SIGNAL( sigRosterPresence(QString, QString) ), this, SLOT( slotRosterPresence(QString, QString) ) );

    QObject::connect( identDialog, SIGNAL(accepted()), this, SLOT( setIdents() ) );

    timer = new QTimer( this );
    QObject::connect( timer, SIGNAL( timeout() ), r, SLOT( slotReceive() ) );
}

void MainWindow::createActions()
{
    connectAct = new QAction( QIcon( ":/images/user_red.png" ), tr( "&Connect" ), this );
    connectAct->setStatusTip( tr( "Connect" ) );
    connect( connectAct, SIGNAL( triggered() ), identDialog, SLOT( exec() ) );

    aboutAct = new QAction( QIcon( ":/images/user_red.png" ), tr( "A&bout" ), this );
    aboutAct->setStatusTip( tr( "About" ) );

    quitAct = new QAction( QIcon( ":/images/user_red.png" ), tr( "&Quit" ), this );
    quitAct->setStatusTip( tr( "Quit" ) );
    connect( quitAct, SIGNAL( triggered() ), this, SLOT( close() ) );
}

void MainWindow::createToolBars()
{
    mainToolBar = addToolBar( tr( "Main" ) );
    mainToolBar->addAction( connectAct );
    mainToolBar->addAction( aboutAct );
    mainToolBar->addAction( quitAct );
}

void MainWindow::createDocks()
{
    setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);

    consoleDock = new QDockWidget( tr( "Console" ), this );
    addDockWidget( Qt::BottomDockWidgetArea, consoleDock );
    console = new QListWidget( this );
    consoleDock->setWidget( console );
    console->setMaximumHeight( 120 );

    contactsDock = new QDockWidget( tr( "Contacts" ), this );
    addDockWidget( Qt::LeftDockWidgetArea, contactsDock );
    contactsList = new QListWidget( this );
    contactsDock->setWidget( contactsList );
    contactsList->setMaximumWidth( 150 );
}

void MainWindow::setIdents()
{
    username = identDialog->userNameInput->text();
    userpass = identDialog->userPassInput->text();

    slotConnect();
}

void MainWindow::slotConnect()
{
    r->start( username, userpass );
    timer->start( 500 );
}

void MainWindow::about()
{}

void MainWindow::slotConnected()
{
    QListWidgetItem * newItem = new QListWidgetItem( tr( "Connection!" ) );
    console->addItem(newItem);
    console->scrollToItem(newItem);
}

void MainWindow::slotResourceBindError(){}
void MainWindow::slotSessionCreateError(){}
void MainWindow::slotItemSubscribed(){}
void MainWindow::slotItemAdded(){}
void MainWindow::slotItemUnsubscribed(){}
void MainWindow::slotItemRemoved(){}
void MainWindow::slotItemUpdated(){}

void MainWindow::slotRoster( QStringList *newRoster )
{
    for( int i = 0; i < newRoster->size(); ++i)
    {
        QString temp = newRoster->at( i );
        QListWidgetItem *tempItem = new QListWidgetItem( temp );
        tempItem->setIcon( QIcon( ":/images/user_gray.png" ) );
        contactsList->addItem( tempItem );
        roster.insert( temp , tempItem );
    }
}

void MainWindow::slotRosterError(){}
void MainWindow::slotRosterPresence( QString jid, QString resource )
{
    QListWidgetItem *tempItem = roster.value( jid );
    tempItem->setIcon( QIcon( ":/images/user_green.png" ) );
}

void MainWindow::slotRosterNonPresence(){}
void MainWindow::slotSubscriptionRequest(){}
void MainWindow::slotUnsubscriptionRequest(){}
void MainWindow::slotMessage(){}
