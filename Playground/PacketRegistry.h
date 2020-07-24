#pragma once
#ifndef _PACKET_BINDING_H
#define _PACKET_BINDING_H

	#if defined(CLIENT) 
		class Server;
		#define PacketHandler Server
	#else
		class Client;
		#define PacketHandler Client
	#endif

	enum ClientStatus : unsigned char
	{
		STATUS_NONE_PACKET,
		STATUS_AUTHENTICATED,
		STATUS_LOGGEDIN,
		STATUS_INGAME,
	};
	/*
		A packet uses helper classes, they look like this: 
		class SendLoginInformation : public Packet
		{
			SendLoginInformation() {  }
			SendLoginInformation(Packet&& packet) : Packet(packet) {  }

			void Read() override final 
			{
				this function is called automatically when a packet which matching id is received if registered in the regions below.
				Keep in mind that the callbacks are not directed just to this, the program will call a function inside 
				class Client, Example: Client::onLoginResult(SendLoginInformation &result); where the argument' class would be this functions 
				object class SendLoginInformation. Hence once you've received the packet and extracted information like such:

				username = file.getStr();

				You are free to use it in the function body of 
				Client::onLoginResult(SendLoginInformation &result) {
					
					Here you could declare an object of your response packet, that class could have functions for database control etc or any other functions

					LoginInformationResult login_result; 
					if(result.getUsername().is())
						login_result.set(database.findUser(result.getUsername()));

					connection.Send(login_result.Write());
				}
			}
			Packet const* Write() override 
			{ 
				__super::Write();

				this functions is called manually in code, whenever wished. 
				Upon registration in the regions below you'll be able to control 
				if it's the server or client that should use the function "Write"	
				you could call it Send, example of execution to send a packet would be...

				SendLoginInformation information; 
				... 
				information.username = "blabla";

				Connection.send(information.Write());
				
			}
			char const* username;
		}
	
	
	*/
	
#pragma region ServerRecvPackets 
	/*

	Client Sends To Server ..
	Callbacks are functions on the server.  (void Client::OnLoginReceived(LoginRequest &packet))

	Connection Status = is the required status to process the packets, (Incase invalid packets were sent or bugs etc.
	Login Packet should example only require STATUS_AUTHENTICATED (I.e. connected to server) and now STATUS_LOGGEDIN or STATUS_INGAME.

				   				Packet Operation Code		Connection Status			Callback on Receive
	*/
	#define ALL_CMSG_PACKETS PACKET(CMSG_LOGIN_DETAILS     , STATUS_AUTHENTICATED, &PacketHandler::OnLoginReceived)\
					         PACKET(ClientSendTradeRequest , STATUS_AUTHENTICATED, &PacketHandler::OnLoginReceived)\
							 PACKET(ClientSendTradeComplete, STATUS_AUTHENTICATED, &PacketHandler::OnLoginReceived)
	//						 PACKET(Nameyouwantforpacketid , ClientStatus::value,  &Client::YourFunction(MyPacketClassExtension &packet) OR just default packet &Client::YourFunction(Packet&)

#pragma endregion
#pragma region ClientRecvPackets
	/*
	Server Sends To Client ..
	Callbacks are functions on the client.  (void Server::OnLoginReceived(LoginResultPacket &packet))
	
								 Packet Operation Code		Connection Status			Callback on Receive
	*/
	#define ALL_SMSG_PACKETS PACKET(SMSG_LOGIN_RESULT   , STATUS_AUTHENTICATED, &PacketHandler::OnLoginResult)\
							 PACKET(ServerVersionControl, STATUS_AUTHENTICATED, &PacketHandler::OnLoginReceived)\
							 PACKET(ServerRegisterResult, STATUS_AUTHENTICATED, &PacketHandler::OnLoginReceived)
	//						 PACKET(Nameyouwantforpacketid , ClientStatus::value,  &Server::YourFunction(MyPacketClassExtension &packet) OR just default packet &Server::YourFunction(Packet&)
#pragma endregion
	
#endif // !_PACKET_BINDING_H