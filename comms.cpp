#include "comms.h"
#include <QTimer>

Comms::Comms()
{}

Comms::~Comms()
{
    delete( client );
}

void Comms::start()
{
    JID jid( "myaccount@gmail.com" );

    client = new Client( jid, "superpass" );

    client->registerConnectionListener( this );
    client->disco()->setVersion( "messageTest", GLOOX_VERSION, "Linux" );
    client->disco()->setIdentity( "client", "SuperBot" );
    client->disco()->addFeature( XMLNS_CHAT_STATES );

    StringList ca;
    ca.push_back( "/path/to/cacert.crt" );
    client->setCACerts( ca );

    client->connect( false );
}

void Comms::onConnect()
{
    emit connec();
}

void Comms::recv()
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
    printf( "status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
            "from: %s\nto: %s\n",
            info.status, info.issuer.c_str(), info.server.c_str(),
            info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
            info.compression.c_str(), ctime( (const time_t*)&info.date_from ),
            ctime( (const time_t*)&info.date_to ) );
    return true;
}
