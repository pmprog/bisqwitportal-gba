
#include "allegro5.h"
#include <enet/enet.h>

#define NETWORKCOMMAND_NULL             0
#define NETWORKCOMMAND_ID               1
#define NETWORKCOMMAND_DISCONNECT       2
#define NETWORKCOMMAND_VALUE            3
#define NETWORKCOMMAND_STATE            4

static NetworkConnectionCallback network_callback_connection;
static NetworkDataCallback network_callback_data;

static u8 network_ismaster;
static u8 network_currentid;

static u16 network_packet[] = { NETWORKCOMMAND_NULL, 0x00, 0x00, 0x00, 0x00 };
static u16 network_data[] = { 0, 0, 0, 0 };

static ENetAddress serveraddress;
static ENetHost* localhost;
static ENetPeer* clients[] = { NULL, NULL, NULL };

static char servername[51];
static enet_uint16 serverport = 9090;

static ALLEGRO_THREAD* network_thread;

static void network_sendpacket();
static void network_serial();
static void* network_thread_proc(ALLEGRO_THREAD* Thread, void* Data);
static void network_hostoverlay();
static void network_clientoverlay();

void network_init(NetworkConnectionCallback ConnectionCallback, NetworkDataCallback DataCallback)
{
    network_callback_connection = ConnectionCallback;
    network_callback_data = DataCallback;
}

void network_server_start()
{
    // Take control of the game to configure the server
    network_hostoverlay();

    network_currentid = 0;
    network_ismaster = 1;
    serveraddress.host = ENET_HOST_ANY;
    serveraddress.port = serverport;
    localhost = enet_host_create(&serveraddress /* the address to bind the server host to */,
        8 /* allow up to 8 clients and/or outgoing connections */,
        0 /* only use 1 channel for everything */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    network_thread = al_create_thread(network_thread_proc, NULL);
    al_start_thread(network_thread);
}

void network_server_stop()
{
    network_currentid = 0xFF;
    for(u8 c = 0; c < 3; c++)
    {
        if(clients[c] != NULL)
        {
            // Tell clients to disconnect
            network_packet[0] = NETWORKCOMMAND_DISCONNECT;
            network_sendpacket();
            // Disconnect
            enet_peer_disconnect(clients[c], 0);
            enet_peer_reset(clients[c]);
            clients[c] = NULL;
        }
    }
}

void network_client_start()
{
    // Take control of the game to configure network support
    memset((void*)&servername[0], 0, sizeof(servername));
    network_clientoverlay();

    network_currentid = 0x80;
    network_ismaster = 0;
    localhost = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        0 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);
    enet_address_set_host(&serveraddress, (char*)&servername[0]);
    serveraddress.port = serverport;
    clients[0] = enet_host_connect(localhost, &serveraddress, 1, 0);

    network_thread = al_create_thread(network_thread_proc, NULL);
    al_start_thread(network_thread);
}

void network_client_stop()
{
    network_server_stop();
}

void network_sendword(u16 Data)
{
    network_packet[0] = NETWORKCOMMAND_VALUE;
    network_packet[1] = Data;
    network_sendpacket();
}

static void* network_thread_proc(ALLEGRO_THREAD* Thread, void* Data)
{
	ENetEvent netevent;

    while(network_currentid != 0xFF)
    {
        // TODO: Check for data to receive
        /*
        while (enet_host_service(_privatedata->localhost, &netevent, 0) > 0)
        {
            switch (netevent.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    e = new Event();
                    e->data.network.server = this;
                    e->data.network.client = nullptr;
                    _privatedata->clients.Add(netevent.peer);
                    e->type = EventType::EVENT_NETWORK_CONNECTIONREQUEST;
                    e->data.network.serverpeer = netevent.peer;
                    e->data.network.packet = nullptr;
                    r->Add(e);
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    e = new Event();
                    e->data.network.server = this;
                    e->data.network.client = nullptr;
                    e->type = EventType::EVENT_NETWORK_PACKETRECEIVED;
                    e->data.network.serverpeer = netevent.peer;
                    e->data.network.packet = new Memory((void*)netevent.packet->data, netevent.packet->dataLength);
                    if (automaticrelaypackets)
                    {
                        SendPacket(new Memory(e->data.network.packet)); // e->data.network.packet );
                    }
                    r->Add(e);
                    enet_packet_destroy(netevent.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    e = new Event();
                    e->data.network.server = this;
                    e->data.network.client = nullptr;
                    enet_peer_reset(netevent.peer);
                    e->type = EventType::EVENT_NETWORK_DISCONNECTED;
                    e->data.network.serverpeer = netevent.peer;
                    e->data.network.packet = nullptr;
                    r->Add(e);
                    _privatedata->clients.Delete(netevent.peer);
                    break;

                default:
                    break;
            }
            if(network_currentid == 0xFF) { break; }
        }
        */

        if(network_currentid == 0xFF) { break; }

        if( network_ismaster != 0 )
        {
            network_packet[0] = NETWORKCOMMAND_STATE;
            network_packet[1] = network_data[0];
            network_packet[2] = network_data[1];
            network_packet[3] = network_data[2];
            network_packet[4] = network_data[3];
            network_sendpacket();
        }
    }

    return NULL;
}

static void network_serial()
{
    // network_callback_data(REG_SIOMULTI0, REG_SIOMULTI1, REG_SIOMULTI2, REG_SIOMULTI3);
}

static void network_sendpacket()
{
    ENetPacket* packet = enet_packet_create((void*)&network_packet, 10, ENET_PACKET_FLAG_RELIABLE);
    for(u8 c = 0; c < 3; c++)
    {
        if(clients[c] != NULL)
        {
            enet_peer_send(clients[c], 0, packet);
        }
    }
    enet_host_flush(localhost);
}


static void network_hostoverlay()
{
    // serverport
}

static void network_clientoverlay()
{
}
