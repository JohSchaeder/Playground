#pragma once
#ifndef _LOGINPACKETS_H
#define _LOGINPACKETS_H
namespace Network
{
	namespace Packets 
	{
		namespace Login
		{
			class SendLoginDetails : public Network::Packet
			{
			public:
				SendLoginDetails() : Packet(CMSG_LOGIN_DETAILS) { /**/ };
				SendLoginDetails(Packet&&p) : Packet(std::move(p)) { }

				// Server will automatically call this to parse the packet
				void Read() override {
					printf("[SERVER] SendLoginDetails: CMSG_LOGIN_DETAILS packet is read!\n");

					// TODO: Add buffer support for strings
					// Allocator::operator>>(GetID());
					// Allocator::operator>>(GetID());
					Username = "root";
					Password = "root";
				};

				// Clients would call this to write the packet to buffer before sending to server.
				Packet const* Write() override { 
					printf("[CLIENT] SendLoginDetails: CMSG_LOGIN_DETAILS packet is written!\n");

					// TODO: Add buffer support for strings
					// Allocator::operator<<(Username);
					// Allocator::operator<<(Password);
					

					return __super::Write();
				}

				char const* Username;
				char const* Password;
			};

			class SendLoginResult : public Network::Packet
			{
			public:
				enum { SUCCESS, FAILED, INCORRECT_PASSWORD } LoginResults;
				SendLoginResult() : Packet(SMSG_LOGIN_RESULT) {}
				SendLoginResult(Packet&&p) : Network::Packet(std::move(p)) {}

				// Server would call this to write data to packet buffer before sending it away to client.
				Packet const* Write() override { 
					printf("[SERVER] SendLoginResult: SMSG_LOGIN_RESULT packet written!"); 
					return __super::Write(); 
				};

				//Client call
				void Read() {};
			};
		}
	}
}
#endif