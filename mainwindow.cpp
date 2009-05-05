#include <QtGui>
#include <comms.h>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    console = new QListWidget( this );
    setCentralWidget( console );

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

void MainWindow::connecMsg()
{
    QListWidgetItem * newItem = new QListWidgetItem( tr( "Connection successful!" ) );
    console->addItem(newItem);
    console->scrollToItem(newItem);
}
