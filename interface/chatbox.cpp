#include "chatbox.h"
#include <QtDebug>

ChatBox::ChatBox( QString jid )
{
    chatLayout = new QVBoxLayout();
    chatView = new QWebView();
    chatInput = new QLineEdit();

    chatLayout->addWidget( chatView );
    chatLayout->addWidget( chatInput );

    setLayout( chatLayout );
    setWindowTitle( jid );
    contactJid = jid;
    show();

    connect( chatInput, SIGNAL( returnPressed() ), this, SLOT( slotSendMessage() ) );
}

void ChatBox::addMsg( QString msg )
{
    QString temp = "<p>" + msg + "</p>";
    history.append( temp );
    chatView->setHtml( history );
}

void ChatBox::slotSendMessage()
{
    emit sigSendMessage( contactJid, chatInput->text() );
    chatInput->clear();
}
