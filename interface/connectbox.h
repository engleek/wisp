#ifndef CONNECTBOX_H
#define CONNECTBOX_H

#include <QDialog>
#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QMovie>
#include <QStackedLayout>
#include <QRegExp>

class ConnectBox : public QWidget
{
    Q_OBJECT

    public:
        ConnectBox(QWidget *parent = 0);

    signals:
        void tryConnection( QString username, QString userpass);

    public slots:
        void toggleStack();
        void hideMe();
        void nextMsg( const QString &msg );

    private slots:
        void checkIdents();

    private:
        QStackedLayout *stack;

        QWidget *connectionWidget;
        QWidget *connectMsgWidget;

        QFormLayout *connectionLayout;
        QGridLayout *connectMsgLayout;

        QLineEdit *userNameInput;
        QLineEdit *userPassInput;

        QLabel *connectionAnim;
        QLabel *connectionMsg;
        QMovie *loadAnim;

        QPushButton *connectButton;

        QLabel *connectMsgLabel;
};

#endif // IDENTREQUEST_H
