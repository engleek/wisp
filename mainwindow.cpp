#include <comms.h>
#include <contact.h>

#include <gloox/gloox.h>
#include <gloox/jid.h>
#include <gloox/rostermanager.h>
#include <gloox/presencehandler.h>

#include "mainwindow.h"
#include "interface/identrequest.h"
#include "interface/chatbox.h"

#include <QtDebug>

MainWindow::MainWindow()
{
    r = new Comms();
    identDialog = new IdentRequest();

    createActions();
    createToolBars();
    createDocks();

    contactsList = new QListWidget();
    contactsList->setMinimumWidth( 150 );
    contactsList->setMinimumHeight( 250 );
    contactsList->setUniformItemSizes( true );
    contactsList->setSpacing( 5 );
    contactsList->setGridSize( QSize( 200, 50 ) );
    contactsList->setWordWrap( true );
    //contactsList->setWrapping( true );
    //contactsList->
    setCentralWidget( contactsList );

    QObject::connect( r, SIGNAL( sigConnected() ), this, SLOT( slotConnected() ) );
    QObject::connect( r, SIGNAL( sigRoster( QStringList* ) ), this, SLOT( slotRoster( QStringList* ) ) );
    QObject::connect( r, SIGNAL( sigRosterPresence(QString, QString) ), this, SLOT( slotRosterPresence(QString, QString) ) );
    QObject::connect( r, SIGNAL( sigVCardReceived(QString,QString) ), this, SLOT( slotVCardReceived(QString,QString) ) );
    QObject::connect( r, SIGNAL( sigMessage(QString,QString) ), this, SLOT( slotMessage(QString,QString) ) );

    QObject::connect( identDialog, SIGNAL(accepted()), this, SLOT( setIdents() ) );

    timer = new QTimer( this );
    QObject::connect( timer, SIGNAL( timeout() ), r, SLOT( slotReceive() ) );
}

void MainWindow::createActions()
{
    connectAct = new QAction( QIcon( ":/images/connect.png" ), tr( "&Connect" ), this );
    connectAct->setStatusTip( tr( "Connect" ) );
    connect( connectAct, SIGNAL( triggered() ), identDialog, SLOT( exec() ) );

    toggleConsoleAct = new QAction( QIcon( ":/images/application_view_list.png" ), tr( "Co&nsole" ), this );
    toggleConsoleAct->setStatusTip( tr( "Toggle Console" ) );
    connect( toggleConsoleAct, SIGNAL( triggered() ), this, SLOT( slotToggleConsole() ) );

    aboutAct = new QAction( QIcon( ":/images/information.png" ), tr( "A&bout" ), this );
    aboutAct->setStatusTip( tr( "About" ) );

    quitAct = new QAction( QIcon( ":/images/door_open.png" ), tr( "&Quit" ), this );
    quitAct->setStatusTip( tr( "Quit" ) );
    connect( quitAct, SIGNAL( triggered() ), this, SLOT( close() ) );

    showChatAct = new QAction( QIcon( ":/images/information.png" ), tr("Show Chat"), this);
    showChatAct->setStatusTip( "Show Chat" );
    connect( showChatAct, SIGNAL( triggered() ), this, SLOT( slotShowChat() ) );
}

void MainWindow::createToolBars()
{
    mainToolBar = addToolBar( tr( "Main" ) );
    mainToolBar->addAction( connectAct );
    mainToolBar->addAction( toggleConsoleAct );
    mainToolBar->addAction( aboutAct );
    mainToolBar->addAction( quitAct );
    mainToolBar->addAction( showChatAct );
}

void MainWindow::createDocks()
{
    setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);

    consoleDock = new QDockWidget( tr( "Console" ), this );
    addDockWidget( Qt::BottomDockWidgetArea, consoleDock );
    console = new QListWidget( this );
    consoleDock->setWidget( console );
    console->setMaximumHeight( 120 );
    console->setAlternatingRowColors( true );
    consoleDock->setVisible( false );
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

void MainWindow::slotToggleConsole()
{
    consoleDock->setVisible( !consoleDock->isVisible() );
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
        QString jid = newRoster->at( i );
        QListWidgetItem *tempItem = new QListWidgetItem( jid );
        tempItem->setIcon( QIcon( ":/images/user_gray.png" ) );

        Contact *contact = new Contact();
        contact->jid = jid;
        contact->contactEntry = tempItem;

        roster.insert( jid , contact );
        contactsList->addItem( tempItem );
    }
}

void MainWindow::slotRosterError(){}
void MainWindow::slotRosterPresence( QString jid, QString msg )
{
    QListWidgetItem *tempItem = roster.value( jid )->contactEntry;
    roster.value( jid )->statusMsg = QString( msg );
    tempItem->setIcon( QIcon( ":/images/user_green.png" ) );
    QString temp = jid + QString( "\n" ) + msg;
    tempItem->setText( temp );
}

void MainWindow::slotRosterNonPresence(){}
void MainWindow::slotSubscriptionRequest(){}
void MainWindow::slotUnsubscriptionRequest(){}
void MainWindow::slotVCardReceived( QString jid, QString name )
{
    roster.value( jid )->name = name;
    QString temp = name + QString( "\n" ) + roster[ jid ]->statusMsg;
    roster.value( jid )->contactEntry->setText( temp );
}

void MainWindow::slotMessage( QString from, QString body )
{
    if( !conversations.contains( from ) )
    {
        conversations.insert( from, new ChatBox( from ) );
        connect( conversations[ from ], SIGNAL( sigSendMessage(QString,QString) ), this, SLOT( slotSendMsg(QString,QString) ) );
    }
    QString temp = roster[ from ]->name + QString( ": " ) + body;
    conversations[ from ]->addMsg( temp );
}

void MainWindow::slotSendMsg( QString jid, QString msg )
{
    qDebug() << "Message Send: " << jid << " :: " << msg;
    r->slotSendMessage( jid, msg );
    QString temp = QString( "me: " ) + msg;
    conversations[ jid ]->addMsg( temp );
}

void MainWindow::slotShowChat()
{
    ChatBox *temp = new ChatBox( QString("myChatBox") );
    temp->addMsg( QString( "hello" ) );
}
