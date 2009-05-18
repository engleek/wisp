#ifndef CHATBOX_H
#define CHATBOX_H

#include <QTabWidget>
#include <QString>
#include <QWebView>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTimer>
#include <QTime>

class ChatBox : public QTabWidget
{
    Q_OBJECT

    public:
        ChatBox( QString jid );
        void addMsg( QString name, QString msg );

    signals:
        void sigSendMessage( QString from, QString msg);

    public slots:
        void slotSendMessage();

    private slots:
        void time();

    private:
        QVBoxLayout *chatLayout;
        QWebView* chatView;
        QLineEdit* chatInput;
        QString lastToTalk;
        QString body;
        QString contactJid;
        QString history;
        QTime lastTalkedTime;
        QDate lastTalkedDate;
        QTimer *timer;
};

#endif // CHATBOX_H
