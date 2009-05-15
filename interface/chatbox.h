#ifndef CHATBOX_H
#define CHATBOX_H

#include <QTabWidget>
#include <QString>
#include <QWebView>
#include <QVBoxLayout>
#include <QLineEdit>

class ChatBox : public QTabWidget
{
    Q_OBJECT

    public:
        ChatBox( QString jid );
        void addMsg( QString msg );

    signals:
        void sigSendMessage( QString from, QString msg);

    public slots:
        void slotSendMessage();

    private:
        QVBoxLayout *chatLayout;
        QWebView* chatView;
        QLineEdit* chatInput;
        QString contactJid;
        QString history;
};

#endif // CHATBOX_H
