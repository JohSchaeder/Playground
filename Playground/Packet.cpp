#include "stdafx.h"
#include "main.h"

#if defined(CLIENT)
	#define PacketHandler Server
#elif defined(SERVER) 
	#define PacketHandler Client
#endif

namespace Network 
{
	LibraryPacketCallbackFunction::FunctionRegister PacketList;
	namespace LibraryPacketCallbackFunction
	{
		template<typename Datatype> struct GetArgumentClass { };
		template<typename PacketClass> struct GetArgumentClass<void(PacketHandler::*)(PacketClass&)> { using type = PacketClass; };

		Function const* FunctionRegister::operator[](ReceivedPacketIdentification code) const
		{
			if (code < 0)return nullptr;
			if (code >= NUM_PACKETS)return nullptr;

			return m_Functions[static_cast<unsigned int>(code)];
		}
		#define PACKET(_PacketID_, _Status_, _Function_) new ArgumentSpecializedFunction<typename GetArgumentClass<decltype(_Function_)>::type, _Function_>(#_PacketID_, (ClientStatus)_Status_),
			FunctionRegister::FunctionRegister() : m_Functions{
		#if defined(SERVER)
			ALL_CMSG_PACKETS
		#elif defined(CLIENT)
			ALL_SMSG_PACKETS
		#endif 
		} { }
		#undef PACKET

		FunctionRegister::~FunctionRegister()
		{
			for (int index = 0; index < NUM_PACKETS; index++)
				delete m_Functions[index];
		}

		/*Callbacks Client::MyFunction(AnyClass& packet); where AnyClass must extend Packet.*/
		template<typename PacketClass, void(PacketHandler::*function)(PacketClass&)>
		ArgumentSpecializedFunction<PacketClass, function>::ArgumentSpecializedFunction(char const* name, ClientStatus status) : Function(name, status) { }
		template<typename PacketClass, void(PacketHandler::*function)(PacketClass&)>
		void ArgumentSpecializedFunction<PacketClass, function>::Call(PacketHandler* session, Packet& packet) const
		{
			PacketClass cPacket(std::move(packet));
			cPacket.Read();
			(session->*function)(cPacket);
		}

		/*Callbacks such as PacketHandler::MyFunction(Packet& packet);*/
		template<void(PacketHandler::*function)(Packet&)>
		ArgumentSpecializedFunction<Packet, function>::ArgumentSpecializedFunction(char const* name, ClientStatus status) : Function(name, status) { }
		template<void(PacketHandler::*function)(Packet&)>
		void ArgumentSpecializedFunction<Packet, function>::Call(PacketHandler* session, Packet& packet) const
		{
			(session->*function)(packet);
		}
		#undef PacketHandler
	}
}

