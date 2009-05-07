#ifndef IDENTREQUEST_H
#define IDENTREQUEST_H

#include <QDialog>
#include <QtGui>

class IdentRequest : public QDialog
{
    Q_OBJECT

    public:
        IdentRequest(QWidget *parent = 0);
        QLineEdit *userNameInput;
        QLineEdit *userPassInput;

    private slots:
        void checkIdents();

    private:
        QGridLayout *layout;

        QLabel *userNameLabel;
        QLabel *userPassLabel;

        QPushButton *connectButton;
        QPushButton *cancelButton;
};

#endif // IDENTREQUEST_H
