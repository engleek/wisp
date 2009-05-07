#include "identrequest.h"

IdentRequest::IdentRequest(QWidget *parent)
{
    layout = new QGridLayout( this );

    connectButton = new QPushButton( tr( "Connect" ), this );
    cancelButton = new QPushButton( tr( "Cancel" ), this );

    userNameInput = new QLineEdit();
    userPassInput = new QLineEdit();
    userPassInput->setEchoMode( QLineEdit::Password );

    userNameLabel = new QLabel( tr( "username" ) );
    userPassLabel = new QLabel( tr( "password" ) );

    userNameLabel->setBuddy( userNameInput );
    userPassLabel->setBuddy( userPassInput );

    layout->addWidget( userNameLabel, 0, 0, Qt::AlignRight );
    layout->addWidget( userPassLabel, 1, 0, Qt::AlignRight );
    layout->addWidget( userNameInput, 0, 1, Qt::AlignLeft );
    layout->addWidget( userPassInput, 1, 1, Qt::AlignLeft );
    layout->addWidget( connectButton, 2, 0, Qt::AlignRight );
    layout->addWidget( cancelButton, 2, 1, Qt::AlignLeft );

    QObject::connect( connectButton, SIGNAL( clicked() ),
                      this, SLOT( checkIdents() ) );
    QObject::connect( cancelButton, SIGNAL( clicked() ),
                      this, SLOT( reject() ) );
}

void IdentRequest::checkIdents()
{
    if( !userNameInput->text().isEmpty() && !userPassInput->text().isEmpty() )
    {
        emit accept();
    }
}
