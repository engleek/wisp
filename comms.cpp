#include "comms.h"
#include <QTimer>
#include <QString>
#include <QStringList>
#include <QtDebug>

Comms::Comms()
{}

Comms::~Comms()
{
    delete( client );
}

void Comms::start( QString user, QString pass )
{
    username = user;
    userpass = pass;

    JID jid( username.toStdString() );

    client = new Client( jid, userpass.toStdString() );
    vManager = new VCardManager( client );

    client->disco()->setVersion( "wisp", GLOOX_VERSION );
    client->disco()->setIdentity( "client", "SuperMe" );
    client->disco()->addFeature( XMLNS_CHAT_STATES );

    client->registerConnectionListener( this );
    client->registerMessageHandler( this );
    client->registerMessageSessionHandler( this, 0 );
    client->rosterManager()->registerRosterListener( this );

    client->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );

/*    StringList ca;
    ca.push_back( "/path/to/cacert.crt" );
    client->setCACerts( ca );
*/
    client->connect( false );
}

void Comms::onConnect()
{
    emit sigConnected();
}

void Comms::slotReceive()
{
    client->recv( 400 );
}

void Comms::onDisconnect( ConnectionError e )
{
    printf( "message_test: disconnected: %d\n", e );
    if( e == ConnAuthenticationFailed )
        printf( "auth failed. reason: %d\n", client->authError() );
}

bool Comms::onTLSConnect( const CertInfo& info )
{
    emit sigConnected();
    printf( "status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
            "from: %s\nto: %s\n",
            info.status, info.issuer.c_str(), info.server.c_str(),
            info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
            info.compression.c_str(), ctime( (const time_t*)&info.date_from ),
            ctime( (const time_t*)&info.date_to ) );
    return true;
}

void Comms::onResourceBindError( ResourceBindError error )
{
    emit sigResourceBindError();
}

void Comms::onSessionCreateError( SessionCreateError error )
{
    emit sigSessionCreateError();
}

void Comms::handleItemSubscribed( const JID& jid )
{
    emit sigItemSubscribed( jid );
}

void Comms::handleItemAdded( const JID& jid )
{
    emit sigItemAdded( jid );
}

void Comms::handleItemUnsubscribed( const JID& jid )
{
    emit sigItemUnsubscribed( jid );
}

void Comms::handleItemRemoved( const JID& jid )
{
    emit sigItemRemoved( jid );
}

void Comms::handleItemUpdated( const JID& jid )
{
    emit sigItemUpdated( jid );
}

void Comms::handleRoster( const Roster& roster )
{
    QStringList list;

    Roster::const_iterator it = roster.begin();
    for( ; it != roster.end(); ++it )
    {
        list << (*it).second->jid().c_str();
    }

    emit sigRoster( &list );
}

void Comms::handleRosterError( Stanza * /*stanza*/ )
{
    emit sigRosterError();
}

void Comms::handleRosterPresence( const RosterItem& item, const std::string& resource, Presence presence, const std::string& msg )
{
    emit sigRosterPresence( QString::fromUtf8( item.jid().data() ), QString::fromUtf8( msg.data() ) );
    JID jid( item.jid().c_str() );
    vManager->fetchVCard( jid, this );
}

void Comms::handleSelfPresence( const RosterItem& item, const std::string& resource, Presence presence, const std::string& /*msg*/ )
{}

bool Comms::handleSubscriptionRequest( const JID& jid, const std::string& /*msg*/ )
{
    emit sigSubscriptionRequest( jid );

    StringList groups;
    JID id( jid );
    client->rosterManager()->subscribe( id, "", groups, "" );

    return true;
}

bool Comms::handleUnsubscriptionRequest( const JID& jid, const std::string& /*msg*/ )
{
    emit sigUnsubscriptionRequest( jid );
    return true;
}

void Comms::handleNonrosterPresence( Stanza* stanza )
{
    emit sigRosterNonPresence();
}

void Comms::handleVCard( const JID &jid, VCard *vcard )
{
    emit sigVCardReceived( QString::fromUtf8( jid.bare().data() ), QString::fromUtf8( vcard->formattedname().data() ) );
}

void Comms::handleVCardResult( VCardContext context, const JID &jid, StanzaError se = StanzaErrorUndefined )
{}

void Comms::handleMessage( Stanza *stanza, MessageSession * /*session*/ )
{
    emit sigMessage( QString::fromUtf8( stanza->from().bare().data() ), QString::fromUtf8( stanza->body().data() ) );
    //qDebug() << "Received:\n\tdata: " << QString( stanza->body().c_str() ).toUtf8();
}

void Comms::handleMessageSession( MessageSession *session )
{
    QString key = QString::fromUtf8( session->target().bare().data() );
    sessions[ key ] = session;
    sessions[ key ]->registerMessageHandler( this );
}

void Comms::localSession( QString jid )
{
    if( ! sessions.keys().contains( jid ) )
    {
        MessageSession *temp = new MessageSession( client, JID( jid.toStdString() ), false, 0 );
        sessions[ jid ] = temp;
        temp->registerMessageHandler( this );
    }
}

void Comms::slotSendMessage( QString dest, QString message )
{
    QString temp = QString( message.toUtf8() );
    sessions[ dest ]->send( temp.toStdString() );
}

void Comms::handleLog( LogLevel level, LogArea area, const std::string& message )
{
    QString temp = QString::fromUtf8( message.data(), message.length() );
    qDebug() << temp << "\n";
}
