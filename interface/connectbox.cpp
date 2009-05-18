#include "connectbox.h"

ConnectBox::ConnectBox(QWidget *parent)
{
    userNameInput = new QLineEdit();
    userPassInput = new QLineEdit();
    userPassInput->setEchoMode( QLineEdit::Password );

    connectButton = new QPushButton( tr( "Connect" ), this );

    connectionLayout = new QFormLayout();
    connectionLayout->addRow( tr( "GMail" ), userNameInput );
    connectionLayout->addRow( tr( "Password" ), userPassInput );
    connectionLayout->addRow( connectButton );

    connectionWidget = new QWidget();
    connectionWidget->setLayout( connectionLayout );

    connectMsgLayout = new QGridLayout();

    connectionMsg = new QLabel( tr("Connecting...") );
    connectionAnim = new QLabel();
    loadAnim = new QMovie( ":/images/bouncingball.gif" );
    connectionAnim->setMovie( loadAnim );

    connectMsgLayout = new QGridLayout();
    connectMsgLayout->addWidget( connectionAnim, 0, 0, Qt::AlignRight );
    connectMsgLayout->addWidget( connectionMsg, 0, 1 );

    connectMsgWidget = new QWidget();
    connectMsgWidget->setLayout( connectMsgLayout );

    stack = new QStackedLayout();
    stack->addWidget( connectionWidget );
    stack->addWidget( connectMsgWidget );

    setLayout( stack );

    QObject::connect( connectButton, SIGNAL( clicked() ),
                      this, SLOT( checkIdents() ) );
    QObject::connect( userPassInput, SIGNAL( returnPressed() ),
                      this, SLOT( checkIdents() ) );
}

void ConnectBox::checkIdents()
{
    if( !userNameInput->text().isEmpty() && !userPassInput->text().isEmpty() )
    {
        //QRegExp mail( "[a-z0-9\.\-_]@[a-z
        //if userNameInput->
        emit tryConnection( userNameInput->text(), userPassInput->text() );
    }
}

void ConnectBox::toggleStack()
{
    if( stack->currentWidget() == connectionWidget )
    {
        stack->setCurrentWidget( connectMsgWidget );
        loadAnim->start();
    }
    else
    {
        stack->setCurrentWidget( connectionWidget );
        loadAnim->stop();
    }
}

void ConnectBox::hideMe()
{
    stack->setCurrentWidget( connectionWidget );
    loadAnim->stop();
}

void ConnectBox::nextMsg( const QString &msg )
{
    connectionMsg->setText( msg );
}
