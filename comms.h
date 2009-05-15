#ifndef COMMS_H
#define COMMS_H

#include <gloox/client.h>
#include <gloox/connectionlistener.h>
#include <gloox/discohandler.h>
#include <gloox/disco.h>
#include <gloox/rostermanager.h>
#include <gloox/loghandler.h>
#include <gloox/logsink.h>
#include <gloox/messagehandler.h>
#include <gloox/messagesessionhandler.h>
#include <gloox/vcardhandler.h>
#include <gloox/vcardmanager.h>
#include <gloox/vcard.h>

#include <QObject>
#include <QString>
#include <QMap>

using namespace gloox;

#ifndef _WIN32
#include <unistd.h>
#endif

#include <stdio.h>
#include <string>

#if defined( WIN32 ) || defined( _WIN32 )
#include <windows.h>
#endif

class Comms : public QObject, RosterListener, ConnectionListener, MessageHandler, MessageSessionHandler, VCardHandler
{
    Q_OBJECT

    public:
        Comms();
        ~Comms();

        void start( QString user, QString pass );

        virtual void onConnect();
        virtual void onDisconnect( ConnectionError e );
        virtual bool onTLSConnect( const CertInfo& info );

        virtual void onResourceBindError( ResourceBindError error );
        virtual void onSessionCreateError( SessionCreateError error );

        virtual void handleItemSubscribed( const JID& jid );
        virtual void handleItemAdded( const JID& jid );
        virtual void handleItemUnsubscribed( const JID& jid );
        virtual void handleItemRemoved( const JID& jid );
        virtual void handleItemUpdated( const JID& jid );
        virtual void handleRoster( const Roster& roster );
        virtual void handleRosterError( Stanza * /*stanza*/ );
        virtual void handleRosterPresence( const RosterItem& item, const std::string& resource, Presence presence, const std::string& /*msg*/ );
        virtual void handleSelfPresence( const RosterItem& item, const std::string& resource, Presence presence, const std::string& /*msg*/ );
        virtual bool handleSubscriptionRequest( const JID& jid, const std::string& /*msg*/ );
        virtual bool handleUnsubscriptionRequest( const JID& jid, const std::string& /*msg*/ );
        virtual void handleNonrosterPresence( Stanza* stanza );
        virtual void handleVCard( const JID& jid, VCard *vcard );
        virtual void handleVCardResult( VCardContext context, const JID& jid, StanzaError se );
        virtual void handleMessage( Stanza *stanza, MessageSession * /*session*/ );
        virtual void handleMessageSession( MessageSession *session );

    public slots:
        void slotReceive();
        void slotSendMessage( QString dest, QString message );

    signals:
        void sigConnected();
        void sigResourceBindError();
        void sigSessionCreateError();
        void sigItemSubscribed( const JID& jid );
        void sigItemAdded( const JID& jid );
        void sigItemUnsubscribed( const JID& jid );
        void sigItemRemoved( const JID& jid );
        void sigItemUpdated( const JID& jid );
        void sigRoster( QStringList *roster );
        void sigRosterError();
        void sigRosterPresence( QString jid, QString msg );
        void sigRosterNonPresence();
        void sigSubscriptionRequest( const JID& jid );
        void sigUnsubscriptionRequest( const JID& jid );
        void sigVCardReceived( QString jid, QString name );
        void sigMessage( QString from, QString body );
        void sigMessageSession( MessageSession *session );

    private:
        QString username;
        QString userpass;

        Client *client;
        VCardManager *vManager;
        ConnectionError ce;
        QMap<QString,MessageSession*> sessions;
};
#endif // COMMS_H
