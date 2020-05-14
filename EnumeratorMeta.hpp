#ifndef ELIGT_ENUMERATORMETA_H
#define ELIGT_ENUMERATORMETA_H

#include <stddef.h>
#include <sstream> 
#include <type_traits>
#include <climits>
#include <limits>


#ifdef ENUMERATORMETA_NAMESPACE
namespace ENUMERATORMETA_NAMESPACE
{
#endif

template <size_t BitLength>
class EnumeratorDataContainer
{
public:
	template <typename IntType = short int, typename ByteType = char>
	inline constexpr static bool isBigEndian()
	{
		static_assert(sizeof(IntType) != sizeof(ByteType), "isBigEndian() : IntType and ByteType need to be of different sizes.");
		static_assert(sizeof(IntType) > sizeof(ByteType), "isBigEndian() : IntType must be of size greater than ByteType.");
		
		IntType number = 0x1;
    ByteType *numPtr = (ByteType*)&number;
    return (numPtr[0] != 1);
	}
	
public:
	using MemoryType = unsigned char;
	using OperandType = unsigned int;
	static constexpr const size_t MEMORY_BITS = sizeof(MemoryType) * CHAR_BIT;
	static constexpr const MemoryType MEMORY_MASK = std::numeric_limits<MemoryType>::max();
	static constexpr const size_t OPERAND_BITS = sizeof(OperandType) * CHAR_BIT;
	static constexpr const size_t OPERAND_COUNT = (BitLength / OPERAND_BITS) + (BitLength % OPERAND_BITS != 0 ? 1 : 0);
	static constexpr const size_t ROUNDED_BITLENGTH = OPERAND_COUNT * OPERAND_BITS;
	static constexpr const size_t ARRAY_SIZE = (ROUNDED_BITLENGTH / MEMORY_BITS) + (ROUNDED_BITLENGTH % MEMORY_BITS != 0 ? 1 : 0);
	
private:
	EnumeratorDataContainer() { } // uninitialized array
	
public:
	constexpr EnumeratorDataContainer(OperandType value) : _data{0}
	{
		if (value != 0)
			*((OperandType*)&_data[0]) = value;
	}
	
	inline explicit constexpr operator bool() const
	{
		return !equals(0);
	}
	
	inline constexpr bool equals(const EnumeratorDataContainer& other) const
	{
		for (int i = 0; i < OPERAND_COUNT; i++)
		{
			if (((const OperandType*)_data)[i] != ((const OperandType*)other._data)[i])
				return false;
		}
		
		return true;
	}
	
	inline constexpr bool equals(OperandType other) const
	{
		if (((const OperandType*)_data)[0] != other)
			return false;
		
		for (int i = 0; i < OPERAND_COUNT; i++)
		{
			if (((const OperandType*)_data)[i] != 0)
				return false;
		}
		
		return true;
	}
	
	inline constexpr bool operator==(const EnumeratorDataContainer& other) const
	{
		return equals(other);
	}
	
	inline constexpr bool operator==(OperandType other) const
	{
		return equals(other);
	}
	
	inline constexpr bool operator!=(const EnumeratorDataContainer& other) const
	{
		return !equals(other);
	}
	
	inline constexpr bool operator!=(OperandType other) const
	{
		return !equals(other);
	}
	
	inline constexpr EnumeratorDataContainer operator&(const EnumeratorDataContainer& other) const
	{
		EnumeratorDataContainer r;
		
		for (int i = 0; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)r._data)[i]) = (((const OperandType*)_data)[i]) & (((const OperandType*)other._data)[i]);
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator&(OperandType other) const
	{
		EnumeratorDataContainer r;
		
		(((OperandType*)r._data)[0]) = (((const OperandType*)_data)[0]) & (other);
		
		for (int i = 1; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)r._data)[i]) = 0;
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator&=(const EnumeratorDataContainer& other)
	{
		for (int i = 0; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)_data)[i]) = (((const OperandType*)_data)[i]) & (((const OperandType*)other._data)[i]);
		}
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator&=(OperandType other)
	{
		(((OperandType*)_data)[0]) = (((const OperandType*)_data)[0]) & (other);
		
		for (int i = 1; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)_data)[i]) = 0;
		}
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator|(const EnumeratorDataContainer& other) const
	{
		EnumeratorDataContainer r;
		
		for (int i = 0; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)r._data)[i]) = (((const OperandType*)_data)[i]) | (((const OperandType*)other._data)[i]);
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator|(OperandType other) const
	{
		EnumeratorDataContainer r;
		
		(((const OperandType*)r._data)[0]) = (((const OperandType*)_data)[0]) | (other);
		
		for (int i = 1; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)r._data)[i]) = (((const OperandType*)_data)[i]);
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator|=(const EnumeratorDataContainer& other)
	{
		for (int i = 0; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)_data)[i]) = (((const OperandType*)_data)[i]) | (((const OperandType*)other._data)[i]);
		}
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator|=(OperandType other)
	{
		(((OperandType*)_data)[0]) = (((const OperandType*)_data)[0]) | (other);
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator^(const EnumeratorDataContainer& other) const
	{
		EnumeratorDataContainer r;
		
		for (int i = 0; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)r._data)[i]) = (((const OperandType*)_data)[i]) ^ (((const OperandType*)other._data)[i]);
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator^(OperandType other) const
	{
		EnumeratorDataContainer r;
		
		(((const OperandType*)r._data)[0]) = (((const OperandType*)_data)[0]) ^ (other);
		
		for (int i = 1; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)r._data)[i]) = (((const OperandType*)_data)[i]);
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator^=(const EnumeratorDataContainer& other)
	{
		for (int i = 0; i < OPERAND_COUNT; i++)
		{
			(((OperandType*)_data)[i]) = (((const OperandType*)_data)[i]) ^ (((const OperandType*)other._data)[i]);
		}
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator^=(OperandType other)
	{
		(((OperandType*)_data)[0]) = (((const OperandType*)_data)[0]) ^ (other);
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator<<(size_t bits) const
	{
		if (bits == 0)
			return *this;
		
		EnumeratorDataContainer ret(0);
		size_t offset = bits / MEMORY_BITS;
		size_t rest = (bits % MEMORY_BITS);
		int last = ARRAY_SIZE - 1;
		
		while (last >= 0)
		{
			auto value = _data[last];
			
			if (value != 0)
				break;
			
			last--;
		}
		
		if (last >= 0)
		{
			int index = last;
			int operand_index = index - (sizeof(OperandType) - 1);
			
			while (operand_index >= 0)
			{
				size_t source = operand_index;
				size_t dest = source + offset;
				auto source_p = (OperandType*)(&_data[source]);
				auto dest_p = (OperandType*)(&ret._data[dest]);
				
				if (dest + (sizeof(OperandType) - 1) >= ARRAY_SIZE)
					break;
				
				if (rest > 0)
				{
					(*dest_p) = (*source_p) << rest;
					
					if (dest + sizeof(OperandType) < ARRAY_SIZE)
						ret._data[dest + sizeof(OperandType)] |= (_data[source + (sizeof(OperandType) - 1)] >> (MEMORY_BITS - rest));
				}
				else
					(*dest_p) = (*source_p);
				
				index = operand_index;
				operand_index = index - (sizeof(OperandType));
			}
			
			while (index >= 0)
			{
				size_t source = index;
				size_t dest = source + offset;
				
				if (rest > 0)
				{
					ret._data[dest] = (_data[source] << rest);
					
					if (dest + 1 < ARRAY_SIZE)
						ret._data[dest + 1] |= (_data[source] >> (MEMORY_BITS - rest));
				}
				else
					ret._data[dest] = _data[source];
				
				index--;
			}
		}
		
		return ret;
	}
	
	inline constexpr EnumeratorDataContainer operator>>(size_t bits) const
	{
		if (bits == 0)
			return *this;
		
		EnumeratorDataContainer ret(0);
		size_t offset = bits / MEMORY_BITS;
		size_t rest = (bits % MEMORY_BITS);
		int last = ARRAY_SIZE - 1;
		
		while (last >= 0)
		{
			auto value = _data[last];
			
			if (value != 0)
				break;
			
			last--;
		}
		
		if (last >= 0)
		{
			int index = 0;
			int operand_index = 0;
			
			while (operand_index <= last)
			{
				size_t dest = operand_index;
				size_t source = dest + offset;
				auto source_p = (OperandType*)(&_data[source]);
				auto dest_p = (OperandType*)(&ret._data[dest]);
				
				if (source + (sizeof(OperandType) - 1) > last)
					break;
			
				if (rest > 0)
				{
					(*dest_p) = (*source_p) >> rest;
					
					if (source + (sizeof(OperandType) - 1) < last)
						ret._data[dest + (sizeof(OperandType) - 1)] |= (_data[source + sizeof(OperandType)] << (MEMORY_BITS - rest));
				}
				else
					(*dest_p) = (*source_p);
				
				operand_index += (sizeof(OperandType));
				index = operand_index;
			}
			
			while (index <= last)
			{
				size_t dest = index;
				size_t source = dest + offset;
				
				if (source > last)
					break;
				
				if (rest > 0)
				{
					ret._data[dest] = _data[source] >> rest;
					
					if (source < last)
						ret._data[dest] |= (_data[source + 1] << (MEMORY_BITS - rest));
				}
				else
					ret._data[dest] = _data[source];
				
				index++;
			}
		}
		
		return ret;
	}
	
public:
	MemoryType _data[ARRAY_SIZE];
};


template <typename EnumType, typename DataType, size_t bit_length, bool isFlags>
class EnumeratorConverter
{
public:
	static inline constexpr DataType get_data(EnumType value)
	{
		auto data_value = static_cast<typename std::make_unsigned<EnumType>::type>(value);
		
		if (data_value > bit_length)
			return static_cast<DataType>(0);
		
		if (data_value == 0)
			return 0;
		
		return static_cast<DataType>(1) << (data_value - 1);
	}
	
	static inline constexpr EnumType get_value(DataType data)
	{
		if (data == 0)
			return static_cast<EnumType>(0);
		
		DataType i=0;
		while( !((data >> i++) & 0x01) ) { ; }
		return static_cast<EnumType>(i);
	}
	
	static inline constexpr EnumType get_bit(size_t index)
	{
		return static_cast<EnumType>(index);
	}
};

template <typename EnumType, typename DataType, size_t bit_length>
class EnumeratorConverter<EnumType, DataType, bit_length, true>
{
public:
	static inline constexpr DataType get_data(EnumType value)
	{
		static_cast<DataType>(value);
	}
	
	static inline constexpr EnumType get_value(DataType data)
	{
		static_cast<EnumType>(data);
	}
	
	static inline constexpr EnumType get_bit(size_t index)
	{
		return static_cast<EnumType>(1 << index);
	}
};

template <typename EnumType>
class EnumeratorMeta
{
public:
	static constexpr const bool logic_operators = false;
	static constexpr const bool math_operators = false;
	static constexpr const bool string_operators = false;
	static constexpr const bool bitwise_conversion = false;
	static constexpr const size_t MAX_VALUE = 0;
};

template <typename EnumType>
class EnumeratorSerializer
{
public:
	static inline constexpr const char* get_name(EnumType value)
	{
		auto it = std::begin(EnumeratorMeta<EnumType>::enum_entries);
		auto end = std::end(EnumeratorMeta<EnumType>::enum_entries);
		
		for (; it != end; ++it)
		{
			auto&& entry = *it;
			
			if (entry.get_value() == value)
				return entry.get_name();
		}
		
		return nullptr;
	}
	
	static inline constexpr const char* get_label(EnumType value)
	{
		auto it = std::begin(EnumeratorMeta<EnumType>::enum_entries);
		auto end = std::end(EnumeratorMeta<EnumType>::enum_entries);
		
		for (; it != end; ++it)
		{
			auto&& entry = *it;
			
			if (entry.get_value() == value)
				return entry.get_label();
		}
		
		return nullptr;
	}
	
	static inline constexpr EnumType get_value(const char* name)
	{
		auto it = std::begin(EnumeratorMeta<EnumType>::enum_entries);
		auto end = std::end(EnumeratorMeta<EnumType>::enum_entries);
		
		for (; it != end; ++it)
		{
			auto&& entry = *it;
			
			if (entry.get_name() == name)
				return entry.get_value();
			else
			{
				if (name != nullptr && entry.get_name() != nullptr)
				{
					auto nameP = name;
					auto itName = entry.get_name();
					
					while (*itName != '\0' && *nameP != '\0' && *itName == *nameP)
					{
						itName++;
						nameP++;
					}
					
					if (*itName == '\0' && *nameP == '\0')
						return entry.get_value();
				}
			}
		}
		
		return EnumeratorMeta<EnumType>::Converter::get_value(0);
	}
};

template <typename EnumType>
class SmartEnumerator
{
protected:
	using Meta = EnumeratorMeta<EnumType>;
	
public:
	using InnerType = EnumType;
	using DataType = typename Meta::DataType;
	
public:
	constexpr SmartEnumerator(EnumType value) : m_data(Meta::Converter::get_data(value)) { }
	
protected:
	constexpr SmartEnumerator(DataType data) : m_data(data) { }
	
public:
	DataType data() const
	{
		return m_data;
	}
	
	inline constexpr bool operator==(SmartEnumerator a) const
	{
		return m_data == a.m_data;
	}
	
	inline constexpr bool operator==(EnumType a) const
	{
		return m_data == Meta::Converter::get_data(a);
	}
	
	inline constexpr bool operator!=(SmartEnumerator a) const
	{
		return m_data != a.m_data;
	}
	
	inline constexpr bool operator!=(EnumType a) const
	{
		return m_data != Meta::Converter::get_data(a);
	}
	
	inline constexpr operator bool() const
	{
		return m_data != 0;
	}
	
protected:
	DataType m_data;
};

template <typename EnumType, size_t bit_length = std::numeric_limits<typename std::make_unsigned<EnumType>::type>::digits>
class EnumeratorMask
{
protected:
	using Meta = EnumeratorMeta<EnumType>;
	
public:
	using InnerType = EnumType;
	static constexpr const size_t BIT_LENGTH = bit_length;
	using DataType = typename std::conditional<
			bit_length <= std::numeric_limits<unsigned char>::digits, unsigned char,
			typename std::conditional<
				bit_length <= std::numeric_limits<unsigned short int>::digits, unsigned short int,
				typename std::conditional<
					bit_length <= std::numeric_limits<unsigned int>::digits, unsigned int,
					typename std::conditional<bit_length <= std::numeric_limits<unsigned long long int>::digits, unsigned long long int, EnumeratorDataContainer<bit_length>>::type
				>::type
			>::type
	>::type;
	
public:
	constexpr EnumeratorMask(const EnumeratorMask<EnumType>& other) : m_data(other.data()) 
	{
		static_assert(
			!Meta::bitwise_conversion || ((size_t)Meta::MAX_VALUE) <= bit_length,
			"EnumeratorMask bit_length has to be large enough to contain enum MAX_VALUE"
		);
	}
	constexpr EnumeratorMask(EnumType value) : m_data(Meta::MaskConverter::get_data(value)) 
	{
		static_assert(
			!Meta::bitwise_conversion || ((size_t)Meta::MAX_VALUE) <= bit_length,
			"EnumeratorMask bit_length has to be large enough to contain enum MAX_VALUE"
		);
	}
	constexpr explicit EnumeratorMask(DataType data) : m_data(data)
	{
		static_assert(
			!Meta::bitwise_conversion || ((size_t)Meta::MAX_VALUE) <= bit_length,
			"EnumeratorMask bit_length has to be large enough to contain enum MAX_VALUE"
		);
	}
	
public:
	DataType data() const
	{
		return m_data;
	}
	
	inline constexpr bool operator==(const EnumeratorMask& a) const
	{
		return m_data == a.m_data;
	}
	
	inline constexpr bool operator==(EnumType a) const
	{
		return m_data == Meta::MaskConverter::get_data(a);
	}
	
	inline constexpr bool operator!=(const EnumeratorMask& a) const
	{
		return m_data != a.m_data;
	}
	
	inline constexpr bool operator!=(EnumType a) const
	{
		return m_data != Meta::MaskConverter::get_data(a);
	}
	
	inline constexpr bool is_empty() const
	{
		return m_data == 0;
	}
	
	bool has(EnumType bit) const
	{
		auto val = Meta::MaskConverter::get_data(bit);
		return (m_data & val) == val;
	}
	
	bool has(EnumeratorMask mask) const
	{
		return (m_data & mask.m_data) == mask.m_data;
	}
	
	bool has(DataType data) const
	{
		return (m_data & data) == data;
	}
	
	bool has_any(EnumeratorMask mask) const
	{
		return (m_data & mask.m_data) != 0;
	}
	
	bool has_any(DataType data) const
	{
		return (m_data & data) != 0;
	}
	
	void set(EnumType bit, bool value)
	{
		auto val = Meta::MaskConverter::get_data(bit);
		if (value)
			m_data |= val;
		else
			m_data &= ~val;
	}
	
	void set(EnumeratorMask mask, bool value)
	{
		if (value)
			m_data |= mask.m_data;
		else
			m_data &= ~mask.m_data;
	}
	
	void set(DataType data, bool value)
	{
		if (value)
			m_data |= data;
		else
			m_data &= ~data;
	}
	
	inline constexpr EnumeratorMask operator|(EnumType a) const
	{
		auto data = Meta::MaskConverter::get_data(a);
		EnumeratorMask m(m_data | data);
		return m;
	}
	
	inline constexpr EnumeratorMask operator|(EnumeratorMask a) const
	{
		EnumeratorMask m(m_data | a.m_data);
		return m;
	}
	
	inline constexpr EnumeratorMask operator|=(EnumType a)
	{ 
		m_data = m_data | Meta::MaskConverter::get_data(a);
		return *this;
	}
	
	inline constexpr EnumeratorMask operator|=(EnumeratorMask a)
	{ 
		m_data = m_data | a.m_data;
		return *this;
	}
	
	inline constexpr EnumeratorMask operator&(EnumType a) const
	{ 
		EnumeratorMask m(m_data & Meta::MaskConverter::get_data(a));
		return m;
	}
	
	inline constexpr EnumeratorMask operator&(EnumeratorMask a) const
	{ 
		EnumeratorMask m(m_data & a.m_data);
		return m;
	}
	
	inline constexpr EnumeratorMask operator&=(EnumType a)
	{ 
		m_data = m_data & Meta::MaskConverter::get_data(a);
		return *this;
	}
	
	inline constexpr EnumeratorMask operator&=(EnumeratorMask a)
	{ 
		m_data = m_data & a.m_data;
		return *this;
	}
	
	inline constexpr EnumeratorMask operator^(EnumType a) const
	{ 
		EnumeratorMask m(m_data ^ Meta::MaskConverter::get_data(a));
		return m;
	}
	
	inline constexpr EnumeratorMask operator^(EnumeratorMask a) const
	{ 
		EnumeratorMask m(m_data ^ a.m_data);
		return m;
	}
	
	inline constexpr EnumeratorMask operator^=(EnumType a)
	{ 
		m_data = m_data ^ Meta::MaskConverter::get_data(a);
		return *this;
	}
	
	inline constexpr EnumeratorMask operator^=(EnumeratorMask a)
	{ 
		m_data = m_data ^ a.m_data;
		return *this;
	}
	
	inline constexpr EnumeratorMask operator~() const
	{ 
		EnumeratorMask m(~m_data);
		return m;
	}
	
	inline std::string toString() const
	{
		std::stringstream ss;
		ss << *this;
		
		return ss.str();
	}
	
protected:
	DataType m_data;
};


template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator+(EnumType a, EnumType b)
{
	return (static_cast<DataType>(a) + static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator+(DataType a, EnumType b)
{
	return (a + static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator+(EnumType a, DataType b)
{
	return (static_cast<DataType>(a) + b);
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType& operator+=(EnumType& a, EnumType b)
{
	return (static_cast<DataType&>(a) += static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType& operator+=(DataType& a, EnumType b)
{
	return (a + static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType& operator+=(EnumType& a, DataType b)
{
	return (static_cast<DataType&>(a) + b);
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType& operator++(EnumType& a)
{
	EnumType* pa = &a;
	//DataType* pd = static_cast<DataType*>(pa);
	DataType* pd = (DataType*) pa; // XXX:!! what da faaak?
	
	return (++(*pd));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator++(EnumType& a, int)
{
	DataType result = static_cast<DataType>(a);
	DataType& d = static_cast<DataType&>(a);
	++d;
	return result;
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator-(EnumType a, EnumType b)
{
	return (static_cast<DataType>(a) - static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator-(DataType a, EnumType b)
{
	return (a - static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator-(EnumType a, DataType b)
{
	return (static_cast<DataType>(a) - b);
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator<<(EnumType a, EnumType b)
{
	return (static_cast<DataType>(a) << static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, 
					typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators && std::is_integral<DataType>::value>::type>
inline constexpr DataType operator<<(DataType a, EnumType b)
{
	return (a << static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator<<(EnumType a, DataType b)
{
	return (static_cast<DataType>(a) << b);
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator>>(EnumType a, EnumType b)
{
	return (static_cast<DataType>(a) >> static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator>>(DataType a, EnumType b)
{
	return (a >> static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::math_operators>::type>
inline constexpr DataType operator>>(EnumType a, DataType b)
{
	return (static_cast<DataType>(a) >> b);
}

template <typename EnumType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::logic_operators>::type>
constexpr typename EnumeratorMeta<EnumType>::MaskType operator|(EnumType a, EnumType b)
{
	return (typename EnumeratorMeta<EnumType>::MaskType(a) | b);
}

template <typename EnumType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::logic_operators>::type>
constexpr typename EnumeratorMeta<EnumType>::MaskType operator&(EnumType a, EnumType b)
{
	return (typename EnumeratorMeta<EnumType>::MaskType(a) & b);
}

template <typename EnumType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::logic_operators>::type>
constexpr typename EnumeratorMeta<EnumType>::MaskType operator^(EnumType a, EnumType b)
{
	return (typename EnumeratorMeta<EnumType>::MaskType(a) ^ b);
}

template <typename EnumType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::logic_operators>::type>
constexpr typename EnumeratorMeta<EnumType>::MaskType operator~(EnumType a)
{
	return (~typename EnumeratorMeta<EnumType>::MaskType(a));
}

template <typename EnumType, typename = typename std::enable_if<EnumeratorMeta<EnumType>::string_operators>::type>
inline std::ostream& operator<<(std::ostream&os, EnumType value)
{
	auto name = EnumeratorSerializer<EnumType>::get_name(value);
	
	if (name != nullptr)
		return os << name;
	
	return os;
}

template <typename EnumType, size_t bit_length, typename = typename std::enable_if<EnumeratorMeta<EnumType>::string_operators>::type>
inline std::ostream& operator<<(std::ostream&os, const EnumeratorMask<EnumType, bit_length>& value)
{
	using Meta = EnumeratorMeta<EnumType>;
	using Converter = typename Meta::MaskConverter;
	auto data = value.data();
	size_t index = 1;
	
	typename Meta::MaskDataType val = 0;
	
	while(data != 0)
	{
		if (val)
			os << ", ";
		
		val = (data & 0x01);
		
		if (val)
		{
			EnumType v = Converter::get_bit(index);
			os << v;
		}
		
		data = data >> 1;
		index++;
	}
	
	return os;
}

template <typename EnumType, bool isFlags = false, size_t max_value = std::numeric_limits<typename std::make_unsigned<EnumType>::type>::digits>
class EnumeratorMetaDefault
{
public:
	using DataType = size_t;
	using Converter = EnumeratorConverter<EnumType, DataType, max_value, isFlags>;
	using MaskType = EnumeratorMask<EnumType, max_value>;
	using MaskDataType = typename MaskType::DataType;
	using MaskConverter = EnumeratorConverter<EnumType, MaskDataType, max_value, isFlags>;
	static constexpr const bool bitwise_conversion = !isFlags;
	
	class EnumEntry
	{
	public:
		EnumType value;
		const char* name;
		const char* label;
		
		constexpr EnumEntry(EnumType val, const char* name_) : value(val), name(name_), label("") { }
		constexpr EnumEntry(EnumType val, const char* name_, const char* label_) : value(val), name(name_), label(label_) { }
		
		EnumType get_value() const { return value; }
		const char* get_name() const { return name; }
		const char* get_label() const { return label; }
	};
};

#ifdef ENUMERATORMETA_NAMESPACE
}
#endif


#endif
