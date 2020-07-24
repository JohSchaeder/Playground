#include "stdafx.h"
#include "main.h"

/**
 * @brief Playground currently works as a packet generation and handling example 
          for easy and very effecient management of client server communication.
 * @param argc 
 * @param argv 
 * @return 0
*/
int _tmain(int argc, _TCHAR* argv[])
{	
	/* Some connected user */
	Client client;

	Network::Packets::Login::SendLoginDetails login_details;
	login_details.Username = "root";
	login_details.Password = "root";

	// Simulate a client sending packet
	login_details.Write();


	/* Simulate server receive data: automatically translate and get registered callback for packet */
	Network::Packet packet(std::move(login_details));
	Network::PacketFunction PacketCallback = Network::GetFunction(packet);

	/* Data did not contain a valid packet */
	if (PacketCallback == nullptr) return 0;

	/* Handle packet */
	switch (PacketCallback->GetStatus())
	{
		case STATUS_AUTHENTICATED:
			/*if(Client.account == NULL && Client.player == NULL etc..) {*/
				
				// Calls registered callback handler, for LoginDetails it is Client::OnLoginReceived.
				PacketCallback->Call(&client, packet); 
			/* } */ 
		break;
		case STATUS_LOGGEDIN:
			/*if(Client.account != NULL && Client.player == NULL etc..) {*/ PacketCallback->Call(&client, packet); /* } */ break;
		case STATUS_INGAME:
			/*if(Client.account != NULL && Client.player != NULL etc..) {*/ PacketCallback->Call(&client, packet); /* } */ break;
	}


	return 0;
}

