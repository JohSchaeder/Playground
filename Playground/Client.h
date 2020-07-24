#pragma once
class Client
{
public:
	void OnLoginReceived(Network::Packets::Login::SendLoginDetails& packet) 
	{ 
		printf("[SERVER] OnLoginReceived: Handling CMSG_LOGIN_DETAILS (SendLoginDetails packet).\n");
		printf("[SERVER] Authenticating user: %s password: %s\n", packet.Username, packet.Password);
		printf("[SERVER] Authentication successful\n");

		// Send login success
		Network::Packets::Login::SendLoginResult result;
		result.Write();
	}
};
