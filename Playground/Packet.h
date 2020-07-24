#pragma once
#ifndef _PACKETMANAGER_H
#define _PACKETMANAGER_H

#define PACKET(_PacketID_, _Status_, _Function_) /**/ _PacketID_,

enum ClientPacketIdentificationCodes { ALL_CMSG_PACKETS CMSG_COUNT, };

enum ServerPacketIdentificationCodes { ALL_SMSG_PACKETS SMSG_COUNT, };
#undef PACKET

#if defined(CLIENT) 
	typedef ServerPacketIdentificationCodes ReceivedPacketIdentification;
	typedef ClientPacketIdentificationCodes SendPacketIdentification;
	#define NUM_PACKETS (unsigned int)SMSG_COUNT
#else
	typedef ClientPacketIdentificationCodes ReceivedPacketIdentification;
	typedef ServerPacketIdentificationCodes SendPacketIdentification;
	#define NUM_PACKETS	(unsigned int)CMSG_COUNT
#endif

namespace Network
{
	class Packet : public Allocator
	{
	public:
		Packet() : Allocator(0), m_PacketID(-1) { }
		Packet(unsigned int id) : Allocator(), m_PacketID(id) {}
		Packet(Packet const& right) : Allocator(right), m_PacketID(right.m_PacketID){ }
		Packet(Packet&& right) : Allocator(std::move(right)), m_PacketID(right.m_PacketID) { }
		Packet(File&& f) : m_PacketID(f.getInt()), Allocator(std::move(f)) {}
		uint32_t GetID() const { return m_PacketID; }
		void SetID(uint32_t code) { m_PacketID = code; }

		virtual Packet const* Write() { Allocator::operator<<(GetID()); return this; }
		virtual void Read() {}
	protected:
		uint32_t m_PacketID;
	};
	namespace LibraryPacketCallbackFunction
	{
		class Function
		{
		public:
			Function(char const* name, ClientStatus status) : Name(name), Status(status) {}
			virtual void Call(PacketHandler*, Packet&) const = 0;
			virtual ~Function() {}

			ClientStatus GetStatus() const { return Status; }
		protected:
			char const* Name;
			ClientStatus Status;
		};
		
		template<class PacketClass, void(PacketHandler::*)(PacketClass&)>
		class ArgumentSpecializedFunction : public Function
		{
		public:
			ArgumentSpecializedFunction(char const* name, ClientStatus status);
			void Call(PacketHandler*, Packet&) const override;
		};
		template<void(PacketHandler::*function)(Packet&)>
		class ArgumentSpecializedFunction<Packet, function> : public Function
		{
		public:
			ArgumentSpecializedFunction(char const* name, ClientStatus status);
			void Call(PacketHandler*, Packet&) const override;
		};
		class FunctionRegister
		{
		public:
			FunctionRegister();
			~FunctionRegister();

			Function const* operator[](ReceivedPacketIdentification) const;
		private:
			FunctionRegister(FunctionRegister const&) = delete;
			FunctionRegister& operator=(FunctionRegister const&) = delete;
			Function* m_Functions[NUM_PACKETS];
			
		};
	}
#undef PacketHandler
	typedef LibraryPacketCallbackFunction::Function const* PacketFunction;
	extern LibraryPacketCallbackFunction::FunctionRegister PacketList;
	inline PacketFunction GetFunction(const Packet& packet) { return PacketList[static_cast<ReceivedPacketIdentification>(packet.GetID())]; }
}


#endif