#include "mainwindow.h"

#include <gloox/gloox.h>
#include <gloox/jid.h>
#include <gloox/rostermanager.h>
#include <gloox/presencehandler.h>


#include <QtDebug>

MainWindow::MainWindow()
{
    setUnifiedTitleAndToolBarOnMac( true );

    r = new Comms();

    createActions();
    createToolBars();
    createDocks();

    contactsList = new QListWidget();
    contactsList->setMinimumWidth( 150 );
    contactsList->setMinimumHeight( 250 );
    contactsList->setUniformItemSizes( true );
    contactsList->setSpacing( 3 );
    contactsList->setWordWrap( true );

    connectBox = new ConnectBox();
    setCentralWidget( connectBox );

    QObject::connect( r, SIGNAL( sigConnected() ), this, SLOT( slotConnected() ) );
    QObject::connect( r, SIGNAL( sigRoster( QStringList* ) ), this, SLOT( slotRoster( QStringList* ) ) );
    QObject::connect( r, SIGNAL( sigRosterPresence(QString, QString) ), this, SLOT( slotRosterPresence(QString, QString) ) );
    QObject::connect( r, SIGNAL( sigVCardReceived(QString,QString) ), this, SLOT( slotVCardReceived(QString,QString) ) );
    QObject::connect( r, SIGNAL( sigMessage(QString,QString) ), this, SLOT( slotMessage(QString,QString) ) );

    QObject::connect( connectBox, SIGNAL( tryConnection(QString,QString) ), this, SLOT( setIdents(QString,QString) ) );

    timer = new QTimer( this );
    QObject::connect( timer, SIGNAL( timeout() ), r, SLOT( slotReceive() ) );
}

void MainWindow::createActions()
{
    connectAct = new QAction( QIcon( ":/images/connect.png" ), tr( "&Connect" ), this );
    connectAct->setStatusTip( tr( "Connect" ) );
    //connect( connectAct, SIGNAL( triggered() ), identDialog, SLOT( exec() ) );

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
    mainToolBar = new QToolBar( tr( "Main" ), this );
    //mainToolBar->addAction( connectAct );
    //mainToolBar->addAction( toggleConsoleAct );
    //mainToolBar->addAction( aboutAct );
    mainToolBar->addAction( quitAct );
    //mainToolBar->addAction( showChatAct );

    mainToolBar->setMovable( false );
    addToolBar( Qt::LeftToolBarArea, mainToolBar );
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

void MainWindow::setIdents( QString name, QString pass )
{
    username = name;
    userpass = pass;

    slotConnect();
}

void MainWindow::slotConnect()
{
    r->start( username, userpass );
    timer->start( 500 );
    connectBox->toggleStack();
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

    QString temp = "Fetching Contacts...";
    connectBox->nextMsg( temp );

    //connectBox->hideMe();
    //setCentralWidget( contactsList );
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

    setCentralWidget( contactsList );
}

void MainWindow::slotRosterError(){}
void MainWindow::slotRosterPresence( QString jid, QString msg )
{
    QListWidgetItem *tempItem = roster.value( jid )->contactEntry;
    roster.value( jid )->statusMsg = QString( msg );
    tempItem->setIcon( QIcon( ":/images/user_green.png" ) );
    QString temp = jid + QString( "\n" ) + msg;
    tempItem->setText( temp );
    contactsList->repaint();
}

void MainWindow::slotRosterNonPresence(){}
void MainWindow::slotSubscriptionRequest(){}
void MainWindow::slotUnsubscriptionRequest(){}
void MainWindow::slotVCardReceived( QString jid, QString name )
{
    roster.value( jid )->name = name;
    QString temp = name + QString( "\n" ) + roster[ jid ]->statusMsg;
    roster.value( jid )->contactEntry->setText( temp );
    setMaximumWidth( contactsList->width() + mainToolBar->width() + 15 );
    repaint();
}

void MainWindow::slotMessage( QString from, QString body )
{
    if( !conversations.contains( from ) )
    {
        conversations.insert( from, new ChatBox( from ) );
        connect( conversations[ from ], SIGNAL( sigSendMessage(QString,QString) ), this, SLOT( slotSendMsg(QString,QString) ) );
    }
    conversations[ from ]->addMsg( roster[ from ]->name, body );
}

void MainWindow::slotSendMsg( QString jid, QString msg )
{
    qDebug() << "Message Send: " << jid << " :: " << msg;
    r->slotSendMessage( jid, msg );
    QString temp = QString( "me" );
    conversations[ jid ]->addMsg( temp, msg );
}

void MainWindow::slotShowChat()
{
    ChatBox *temp = new ChatBox( QString("myChatBox") );
    temp->addMsg( QString( "tester" ), QString( "hello" ) );
}
