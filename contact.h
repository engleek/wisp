#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QMap>
#include <QListWidget>
#include <gloox/messagesession.h>

class Contact
{
    public:
        Contact();
        QString jid;
        QString name;
        QString statusMsg;
        QMap<QString,int> resources;
        QListWidgetItem* contactEntry;
        QListWidget* conversation;
};

#endif // CONTACT_H
