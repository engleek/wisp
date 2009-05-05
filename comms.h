#ifndef COMMS_H
#define COMMS_H

#include <gloox/client.h>
#include <gloox/messagesessionhandler.h>
#include <gloox/messageeventhandler.h>
#include <gloox/messageeventfilter.h>
#include <gloox/chatstatehandler.h>
#include <gloox/chatstatefilter.h>
#include <gloox/connectionlistener.h>
#include <gloox/disco.h>
#include <gloox/stanza.h>
#include <gloox/gloox.h>
#include <gloox/lastactivity.h>
#include <gloox/loghandler.h>
#include <gloox/logsink.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/connectionsocks5proxy.h>
#include <gloox/connectionhttpproxy.h>
#include <gloox/messagehandler.h>

#include <QObject>

using namespace gloox;

#ifndef _WIN32
#include <unistd.h>
#endif

#include <stdio.h>
#include <string>

#if defined( WIN32 ) || defined( _WIN32 )
#include <windows.h>
#endif

class Comms : public QObject, ConnectionListener
{
    Q_OBJECT

    public:
        Comms();
        ~Comms();

        void start();

        virtual void onConnect();
        virtual void onDisconnect( ConnectionError e );
        virtual bool onTLSConnect( const CertInfo& info );

    public slots:
        void recv();

    signals:
        void connec();

    private:
        Client *client;
        ConnectionError ce;
};
#endif // COMMS_H
