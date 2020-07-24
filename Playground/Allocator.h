#pragma once
#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

/* IGNORE THIS */
class File {
public:
	bool get(void*, int) { return false; }
	signed int left() const { return 0; }
	int getInt() { return buffer_value; }
	unsigned char getByte() { return 0; }
	void putInt(int i) {
		buffer_value = i;
	}

private:

	// Should be a real buffer
	int buffer_value;
};


namespace Network
{
	#define NULLED(c) c == NULL
	class Allocator
	{
	public:
		static size_t const RESERVE_SIZE = 0x1000;

		Allocator() : m_ReadIndex(0), m_WriteIndex(0) { m_Buffer.reserve(RESERVE_SIZE); }
		Allocator(size_t reserve) : m_ReadIndex(0), m_WriteIndex(0) { m_Buffer.reserve(RESERVE_SIZE); }
		Allocator(Allocator&& packet) : m_ReadIndex(packet.m_ReadIndex), m_WriteIndex(0), m_Buffer(packet.Move()) {  }
		Allocator(Allocator const& rval) : m_ReadIndex(rval.m_ReadIndex), m_WriteIndex(0), m_Buffer(rval.m_Buffer) {}
		Allocator(File&& right) : m_ReadIndex(0), m_WriteIndex(0)
		{
			m_Buffer.reserve(right.left());
			right.get(GetContent(), right.left());
		}
		Allocator& operator=(Allocator const& right)
		{
			if (this != &right)
			{
				m_ReadIndex = right.m_ReadIndex;
				m_WriteIndex = right.m_WriteIndex;
				m_Buffer = right.m_Buffer;
			}
			return *this;
		}
		Allocator& operator=(Allocator&& right)
		{
			if (this != &right)
			{
				m_ReadIndex = right.m_ReadIndex;
				m_WriteIndex = right.m_WriteIndex;
				m_Buffer = right.Move();
			}
			return *this;
		}
		virtual ~Allocator() { }

		template<typename DataType>
		Allocator &operator >> (DataType &value)
		{
			value = read<DataType>();
		}
		template<typename DataType>
		Allocator &operator << (DataType value)
		{
			write(&value, 1);
			return *this;
		}
		void clear() { m_WriteIndex = 0; m_ReadIndex = 0; m_Buffer.clear(); }
		std::vector<uint8_t>&& Move()
		{
			m_ReadIndex = 0;
			m_WriteIndex = 0;
			return std::move(m_Buffer);
		}

		uint8_t* GetContent() { return (!m_Buffer.empty()) ? m_Buffer.data() : NULL; }
		uint8_t const* GetContent() const { return (!m_Buffer.empty()) ? m_Buffer.data() : NULL; }

		template<typename DataType>
		void write(const DataType *src, size_t count)
		{
			return write((const uint8_t *)src, count * sizeof(DataType));
		}
		inline void write(const char *src, size_t count) { write((const uint8_t *)src, count); }
		void write(const uint8_t *src, size_t count)
		{
			if (NULLED(src) || NULLED(count))return;

			if (m_Buffer.size() < m_WriteIndex + count)
				m_Buffer.resize(m_WriteIndex + count);
			std::memcpy(&m_Buffer[m_WriteIndex], src, count);
			m_WriteIndex += count;
		}
		void write(const Allocator& buffer)
		{
			if (NULLED(buffer.GetContent()))return;
			write(buffer.GetContent(), buffer.m_WriteIndex);
		}

		template<typename DataType>
		DataType read(size_t pos) const
		{
			if (pos + sizeof(DataType) > m_Buffer.size())return;
			m_ReadIndex += sizeof(DataType);
			return *((DataType const*)&m_Buffer[pos]);
		}
		template<typename DataType>
		DataType read() { return read<DataType>(m_ReadIndex); }
		void read(uint8_t *dest, size_t len)
		{
			if (m_ReadIndex + len > m_Buffer.size())return;

			memcpy(dest, &m_Buffer[m_ReadIndex], len);
			m_ReadIndex += len;
		}
	protected:
		size_t m_ReadIndex, m_WriteIndex;
		std::vector<uint8_t> m_Buffer;

	};
	#undef NULLED
}
#endif
