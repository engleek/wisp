#include "chatbox.h"
#include <QtDebug>

ChatBox::ChatBox( QString jid )
{
    chatLayout = new QVBoxLayout();
    chatView = new QWebView();
    chatInput = new QLineEdit();

    chatLayout->addWidget( chatView );
    chatLayout->addWidget( chatInput );

    lastToTalk = "";

    setLayout( chatLayout );
    setWindowTitle( jid );
    contactJid = jid;
    body = "<html><head><style type=\"text/css\">body{font-size:10pt}p{margin: 3px 15px 3px 15px;}.time{color:silver;font-size:8pt;margin-top:1px;}hr{border: 0;width: 95%;color:silver;background-color: silver;height:1px;}</style></head><body>";
    setMinimumWidth( 200 );
    setMinimumHeight( 200 );
    show();

    timer = new QTimer();

    connect( chatInput, SIGNAL( returnPressed() ), this, SLOT( slotSendMessage() ) );
    connect( timer, SIGNAL( timeout() ), this, SLOT( time() ) );
}

void ChatBox::addMsg( QString name, QString msg )
{
    QString temp = "<p>" + msg + "</p>";
    if( lastToTalk.operator !=( name ) )
    {
        temp.prepend( "<b>" + name.split(' ', QString::SkipEmptyParts).at( 0 ) + ":</b>" );
    }

    history.append( temp );
    temp = body + history + "</body></html>";
    chatView->setHtml( temp );
    chatView->page()->mainFrame()->setScrollBarValue( Qt::Vertical, chatView->page()->mainFrame()->scrollBarMaximum( Qt::Vertical ) );

    lastToTalk = name;
    lastTalkedTime = QTime::currentTime();
    lastTalkedDate = QDate::currentDate();
    timer->start( 30000 );

    //qDebug() << chatView->styleSheet()
}

void ChatBox::time()
{
    QString temp = tr( "<hr/><p class=\"time\">%1:%2 on %3</p>" ).arg( lastTalkedTime.hour() ).arg( lastTalkedTime.minute() ).arg( QDate::longDayName( lastTalkedDate.dayOfWeek() ) );
    history.append( temp );
    temp = body + history + "</body></html>";
    chatView->setHtml( temp );
    chatView->page()->mainFrame()->setScrollBarValue( Qt::Vertical, chatView->page()->mainFrame()->scrollBarMaximum( Qt::Vertical ) );
    timer->stop();

    //qDebug() << temp;
}

void ChatBox::slotSendMessage()
{
    emit sigSendMessage( contactJid, chatInput->text() );
    chatInput->clear();
}
