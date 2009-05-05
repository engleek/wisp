#include <QtGui>
#include <comms.h>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    createActions();
    createToolBars();
    createDocks();

    centralWidget = new QFrame();
    chatList = new QListWidget();
    contactsList = new QListWidget();

    mainLayout = new QHBoxLayout();
    mainLayout->addWidget(chatList);
    mainLayout->addWidget(contactsList);
    centralWidget->setLayout( mainLayout );
    setCentralWidget( centralWidget );

    QListWidgetItem * newItem = new QListWidgetItem( tr( "Connecting..." ) );
    console->addItem(newItem);
    console->scrollToItem(newItem);

    Comms *r = new Comms();

    QObject::connect( r, SIGNAL(connec()), this, SLOT(connecMsg()) );

    QTimer *timer = new QTimer( this );
    QObject::connect( timer, SIGNAL( timeout() ), r, SLOT( recv() ) );

    r->start();
    timer->start( 500 );
}

void MainWindow::createActions()
{
    connectAct = new QAction( QIcon( ":/images/new.png" ), tr( "&Connect" ), this );
    connectAct->setStatusTip( tr( "Connect" ) );

    aboutAct = new QAction( QIcon( ":/images/new.png" ), tr( "A&bout" ), this );
    aboutAct->setStatusTip( tr( "About" ) );

    quitAct = new QAction( QIcon( ":/images/new.png" ), tr( "&Quit" ), this );
    quitAct->setStatusTip( tr( "Quit" ) );
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
}

void MainWindow::connectDialog()
{}

void MainWindow::aboutDialog()
{}

void MainWindow::connecMsg()
{
    QListWidgetItem * newItem = new QListWidgetItem( tr( "Connection successful!" ) );
    console->addItem(newItem);
    console->scrollToItem(newItem);
}
