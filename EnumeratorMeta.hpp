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
	
	static inline constexpr EnumeratorDataContainer max()
	{
		EnumeratorDataContainer c;
		
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			c._data[i] = std::numeric_limits<OperandType>::max();
		}
		
		return c;
	}
	
public:
	inline constexpr EnumeratorDataContainer() noexcept = default;
	inline constexpr EnumeratorDataContainer(const EnumeratorDataContainer& other) noexcept = default;
	inline constexpr EnumeratorDataContainer(OperandType value) noexcept : _data{value} { }
	
	inline explicit constexpr operator bool() const
	{
		return !equals(0);
	}
	
	inline constexpr bool equals(const EnumeratorDataContainer& other) const
	{
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			if (_data[i] != other._data[i])
				return false;
		}
		
		return true;
	}
	
	inline constexpr bool equals(OperandType other) const
	{
		if (_data[0] != other)
			return false;
		
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			if (_data[i] != 0)
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
	
	inline constexpr bool has_bit(size_t bit) const
	{
		const size_t offset = bit / OPERAND_BITS;
		const size_t rest = bit % OPERAND_BITS;
		
		if (offset < OPERAND_COUNT)
			return _data[offset] & (static_cast<OperandType>(1) << rest);
		
		return false;
	}
	
	inline constexpr EnumeratorDataContainer operator&(const EnumeratorDataContainer& other) const
	{
		EnumeratorDataContainer r{};
		
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			r._data[i] = _data[i] & other._data[i];
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator&(OperandType other) const
	{
		EnumeratorDataContainer r{};
		
		r._data[0] = _data[0] & other;
		
		for (size_t i = 1; i < OPERAND_COUNT; ++i)
		{
			r._data[i] = 0;
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator&=(const EnumeratorDataContainer& other)
	{
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			_data[i] = _data[i] & other._data[i];
		}
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator&=(OperandType other)
	{
		_data[0] = _data[0] & other;
		
		for (size_t i = 1; i < OPERAND_COUNT; ++i)
		{
			_data[i] = 0;
		}
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator|(const EnumeratorDataContainer& other) const
	{
		EnumeratorDataContainer r{};
		
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			r._data[i] = _data[i] | other._data[i];
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator|(OperandType other) const
	{
		EnumeratorDataContainer r{};
		
		r._data[0] = _data[0] | other;
		
		for (size_t i = 1; i < OPERAND_COUNT; ++i)
		{
			r._data[i] = _data[i];
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator|=(const EnumeratorDataContainer& other)
	{
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			_data[i] = _data[i] | other._data[i];
		}
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator|=(OperandType other)
	{
		_data[0] = _data[0] | other;
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator^(const EnumeratorDataContainer& other) const
	{
		EnumeratorDataContainer r{};
		
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			r._data[i] = _data[i] ^ other._data[i];
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator^(OperandType other) const
	{
		EnumeratorDataContainer r{};
		
		r._data[0] = _data[0] ^ other;
		
		for (size_t i = 1; i < OPERAND_COUNT; ++i)
		{
			r._data[i] = _data[i] ^ 0;
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator^=(const EnumeratorDataContainer& other)
	{
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			_data[i] = _data[i] ^ other._data[i];
		}
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator^=(OperandType other)
	{
		_data[0] = _data[0] ^ other;
		
		for (size_t i = 1; i < OPERAND_COUNT; ++i)
		{
			_data[i] = _data[i] ^ 0;
		}
		
		return *this;
	}
	
	inline constexpr EnumeratorDataContainer operator~() const
	{
		EnumeratorDataContainer r{};
		
		for (size_t i = 0; i < OPERAND_COUNT; ++i)
		{
			r._data[i] = ~_data[i];
		}
		
		return r;
	}
	
	inline constexpr EnumeratorDataContainer operator<<(size_t bits) const
	{
		if (bits == 0)
			return *this;
		
		if (bits == OPERAND_BITS * OPERAND_COUNT)
			return {};
		
		EnumeratorDataContainer ret{};
		const size_t offset = bits / OPERAND_BITS;
		const size_t rest = (bits % OPERAND_BITS);
		
		if (rest > 0)
		{
			OperandType carry = 0;
			
			for (size_t indexRead = 0, indexWrite = offset; indexRead < OPERAND_COUNT - offset; ++indexRead, ++indexWrite)
			{
				OperandType value = _data[indexRead];
				ret._data[indexWrite] = (value << rest) | carry;
				carry = value >> (OPERAND_BITS - rest);
			}
		}
		else
		{
			for (size_t indexRead = 0, indexWrite = offset; indexRead < OPERAND_COUNT - offset; ++indexRead, ++indexWrite)
				ret._data[indexWrite] = _data[indexRead];
		}
	
		return ret;
	}
	
	inline constexpr EnumeratorDataContainer operator>>(size_t bits) const
	{
		if (bits == 0)
			return *this;
		
		if (bits == OPERAND_BITS * OPERAND_COUNT)
			return {};
		
		EnumeratorDataContainer ret{};
		const int offset = bits / OPERAND_BITS;
		const int rest = (bits % OPERAND_BITS);
		
		if (rest > 0)
		{
			OperandType carry = 0;
			
			for (int indexRead = OPERAND_COUNT - 1, indexWrite = indexRead - offset; indexRead >= offset; --indexRead, --indexWrite)
			{
				OperandType value = _data[indexRead];
				ret._data[indexWrite] = (value >> rest) | carry;
				carry = value << (OPERAND_BITS - rest);
			}
		}
		else
		{
			for (int indexRead = OPERAND_COUNT - 1, indexWrite = indexRead - offset; indexRead >= offset; --indexRead, --indexWrite)
				ret._data[indexWrite] = _data[indexRead];
		}
	
		return ret;
	}
	
public:
	OperandType _data[OPERAND_COUNT];
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
};

template <typename EnumType, typename EntryType_ = typename std::remove_const< typename std::remove_reference< typename std::remove_pointer< decltype(EnumeratorMeta<EnumType>::enum_entries[0]) >::type >::type >::type >
class EnumeratorInfo
{
public:
	using EntryType = EntryType_;
	static constexpr const size_t ENTRY_COUNT = sizeof(EnumeratorMeta<EnumType>::enum_entries) / sizeof(EntryType);
	
	struct Iterator
	{
	public:
		using iterator_category = std::input_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = EntryType;
		using pointer           = const EntryType*;
		using reference         = const EntryType&;
		
	public:
		constexpr Iterator(pointer ptr) : _ptr{ptr} { }
		
		reference operator*() const { return *_ptr; }
		pointer operator->() { return _ptr; }
	
		Iterator& operator++() { _ptr++; return *this; }  
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
	
		friend bool operator== (const Iterator& a, const Iterator& b) { return a._ptr == b._ptr; };
		friend bool operator!= (const Iterator& a, const Iterator& b) { return a._ptr != b._ptr; };     
		
	private:
		pointer _ptr;
	};
	
	static constexpr Iterator cbegin() { return Iterator(EnumeratorMeta<EnumType>::enum_entries); }
	static constexpr Iterator cend() { return Iterator(EnumeratorMeta<EnumType>::enum_entries + ENTRY_COUNT);}
	
	static constexpr Iterator find(EnumType value)
	{
		auto it = findQuick(value);
		if (it != cend())
			return it;
		
		return findSlow(value);
	}
	
	static constexpr Iterator find(const char* name)
	{
		auto it = cbegin();
		auto end = cend();
		
		for (; it != end; ++it)
		{
			auto&& entry = *it;
			
			if (entry.get_name() == name)
				return Iterator(&entry);
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
						return Iterator(&entry);
				}
			}
		}
		
		return cend();
	}
	
protected:
	static constexpr Iterator findQuick(EnumType value)
	{
		// Return if the enum values aren't sequential, i.e. their values are already bitflags (isFlags parameter passed to EnumeratorMetaDefault was true)
		if (!EnumeratorMeta<EnumType>::bitwise_conversion)
			return cend();
		
		auto index = static_cast<size_t>(value);
		
		if (index < ENTRY_COUNT)
		{
			auto&& entry = EnumeratorMeta<EnumType>::enum_entries[index];
			if (entry.get_value() == value)
				return Iterator(&entry);
		}
		
		// Special case if enum_entries skips the first 0-valued entry
		if (index > 0 && index <= ENTRY_COUNT)
		{
			auto&& entry = EnumeratorMeta<EnumType>::enum_entries[index - 1];
			if (entry.get_value() == value)
				return Iterator(&entry);
		}
		
		return cend();
	}
	
	static constexpr Iterator findSlow(EnumType value)
	{
		auto it = cbegin();
		auto end = cend();
		
		for (; it != end; ++it)
		{
			auto&& entry = *it;
			
			if (entry.get_value() == value)
				return Iterator(&entry);
		}
		
		return cend();
	}
};

template <typename EnumType>
class EnumeratorSerializer
{
	using Info = EnumeratorInfo<EnumType>;
	using EntryType = typename Info::EntryType;
	
public:
	static constexpr decltype(std::declval<EntryType>().get_name()) get_name(EnumType value)
	{
		auto it = Info::find(value);
		
		if (it != Info::cend())
			return it->get_name();
		
		return nullptr;
	}
	
	static constexpr decltype(std::declval<EntryType>().get_label()) get_label(EnumType value)
	{
		auto it = Info::find(value);
		
		if (it != Info::cend())
			return it->get_label();
		
		return nullptr;
	}
	
	static constexpr EnumType get_value(const char* name)
	{
		auto it = Info::find(name);
		
		if (it != Info::cend())
			return it->get_value();
		
		return EnumeratorMeta<EnumType>::Converter::get_value(0);
	}
};

template <typename EnumType>
class EnumeratorExtender
{
protected:
	using Meta = EnumeratorMeta<EnumType>;
	using DataType = typename Meta::DataType;
	static constexpr const DataType max_enum_value = static_cast<DataType>(std::numeric_limits<DataType>::max());
	
	template<typename T, typename = void>
	struct has_extension_meta : std::false_type { };
	template<typename T>
	struct has_extension_meta<T, decltype((void)EnumeratorMeta<T>::enum_extension, void())> : std::true_type { };
	
	template<typename T, typename = void>
	struct has_extension_value : std::false_type { };
	template<typename T>
	struct has_extension_value<T, decltype((void)T::EXTENSION, void())> : std::true_type { };
	
public:
	template <typename T = EnumType, typename std::enable_if<has_extension_meta<T>::value || has_extension_value<T>::value, int>::type = 0>
	static constexpr bool has_extension()
	{
		return true;
	}
	
	template <typename T = EnumType, typename std::enable_if<!has_extension_meta<T>::value && !has_extension_value<T>::value, int>::type = 0>
	static constexpr bool has_extension()
	{
		return false;
	}
	
	template <typename T = EnumType, typename std::enable_if<has_extension_meta<T>::value, int>::type = 0>
	static constexpr T get_enum_extension()
	{
		return static_cast<T>(Meta::enum_extension);
	}
	
	template <typename T = EnumType, typename std::enable_if<has_extension_value<T>::value && !has_extension_meta<T>::value, int>::type = 0>
	static constexpr T get_enum_extension()
	{
		return static_cast<T>(EnumType::EXTENSION);
	}
	
	template <typename T = EnumType, typename std::enable_if<!has_extension_value<T>::value && !has_extension_meta<T>::value, int>::type = 0>
	static constexpr EnumType get_enum_extension()
	{
		static_assert(sizeof(T) == 0, "EnumeratorExtender requires the EnumeratorMeta to define an enum_extension variable or the enum to contain an EXTENSION value.");
	}
	
	static EnumType extend()
	{
		static EnumType enum_extension = get_enum_extension();
		
		enum_extension += 1;
		
		if (static_cast<DataType>(enum_extension) <= max_enum_value)
			return enum_extension;
		
		// This is not ideal, if extensions overflow we should throw? or at least assert?
		return static_cast<EnumType>(0);
	}
};

template <typename EnumType>
class EnumeratorInheritor
{
protected:
	using Meta = EnumeratorMeta<EnumType>;
	using DataType = typename Meta::DataType;
	
	template<typename T, typename = void>
	struct has_inheritance_meta : std::false_type { };
	template<typename T>
	struct has_inheritance_meta<T, decltype((void)EnumeratorMeta<T>::enum_inheritance, void())> : std::true_type { };
	
	template<typename T, typename = void>
	struct has_inheritance_value : std::false_type { };
	template<typename T>
	struct has_inheritance_value<T, decltype((void)T::INHERITANCE, void())> : std::true_type { };
	
public:
	template <typename T = EnumType, typename std::enable_if<has_inheritance_meta<T>::value || has_inheritance_value<T>::value, int>::type = 0>
	static constexpr bool has_inheritance()
	{
		return true;
	}
	
	template <typename T = EnumType, typename std::enable_if<!has_inheritance_meta<T>::value && !has_inheritance_value<T>::value, int>::type = 0>
	static constexpr bool has_inheritance()
	{
		return false;
	}
	
	template <typename T = EnumType, typename std::enable_if<has_inheritance_meta<T>::value, int>::type = 0>
	static constexpr T get_enum_inheritance()
	{
		return static_cast<T>(Meta::enum_inheritance);
	}
	
	template <typename T = EnumType, typename std::enable_if<has_inheritance_value<T>::value && !has_inheritance_meta<T>::value, int>::type = 0>
	static constexpr T get_enum_inheritance()
	{
		return static_cast<T>(EnumType::INHERITANCE);
	}
	
	template <typename T = EnumType, typename std::enable_if<!has_inheritance_value<T>::value && !has_inheritance_meta<T>::value, int>::type = 0>
	static constexpr EnumType get_enum_inheritance()
	{
		static_assert(sizeof(T) == 0, "EnumeratorInheritor requires the EnumeratorMeta to define an enum_inheritance variable or the enum to contain an INHERITANCE value.");
	}
	
	static constexpr DataType inherit()
	{
		return static_cast<DataType>(get_enum_inheritance());
	}
};

template <typename EnumType>
class EnumeratorSpecializer
{
protected:
	using Meta = EnumeratorMeta<EnumType>;
	using DataType = typename Meta::DataType;
	
	template<typename T, typename = void>
	struct has_base_type : std::false_type { };
	template<typename T>
	struct has_base_type<T, decltype((void)typename EnumeratorMeta<T>::BaseEnumType(), void())> : std::true_type { };
	
public:
	template <typename T = EnumType, typename std::enable_if<has_base_type<T>::value, int>::type = 0>
	static constexpr bool has_base()
	{
		return true;
	}
	
	template <typename T = EnumType, typename std::enable_if<!has_base_type<T>::value, int>::type = 0>
	static constexpr bool has_base()
	{
		return false;
	}
};

template <typename EnumType_>
class SmartEnumerator
{
protected:
	using Meta = EnumeratorMeta<EnumType_>;
	
public:
	using EnumType = EnumType_;
	using DataType = typename Meta::DataType;
	
public:
	constexpr SmartEnumerator(EnumType value) : m_data(value) { }
	
	template<typename EnumT, typename std::enable_if<std::is_enum<EnumT>::value && (std::is_same<EnumT, typename Meta::BaseEnumType>::value || std::is_same<EnumType, typename EnumeratorMeta<EnumT>::BaseEnumType>::value), int>::type = 0>
	constexpr SmartEnumerator(EnumT value) : SmartEnumerator(static_cast<DataType>(value)) { }
	
protected:
	constexpr SmartEnumerator(DataType data) : m_data(static_cast<EnumType>(data)) { }
	
public:
	constexpr DataType data() const
	{
		return m_data;
	}
	
	constexpr bool operator==(SmartEnumerator a) const
	{
		return m_data == a.m_data;
	}
	
	constexpr bool operator==(EnumType a) const
	{
		return m_data == Meta::Converter::get_data(a);
	}
	
	constexpr bool operator!=(SmartEnumerator a) const
	{
		return m_data != a.m_data;
	}
	
	constexpr bool operator!=(EnumType a) const
	{
		return m_data != Meta::Converter::get_data(a);
	}
	
	constexpr operator bool() const
	{
		return m_data != 0;
	}
	
protected:
	EnumType m_data;
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
	
	struct Iterator
	{
	public:
		using iterator_category = std::input_iterator_tag;
		using difference_type   = size_t;
		using value_type        = EnumType;
		using pointer           = const EnumType*;
		using reference         = EnumType;
		
	public:
		constexpr Iterator(const EnumeratorMask* mask, size_t current = 1) : _mask{mask}, _current{findValue(current)}
		{ }
		
		constexpr reference operator*() const { return Meta::MaskConverter::get_bit(_current); }
	
		constexpr Iterator& operator++() { _current = findValue(_current + 1); return *this; }
		constexpr Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
	
		constexpr friend bool operator== (const Iterator& a, const Iterator& b) { return a._mask == b._mask && a._current == b._current; }
		constexpr friend bool operator!= (const Iterator& a, const Iterator& b) { return !(a == b); }
		
	protected:
		constexpr size_t findValue(size_t current) const
		{
			while (!_mask->has_bit(current) && current < BIT_LENGTH)
			{
				++current;
			}
			
			return current;
		}
		
	private:
		const EnumeratorMask* _mask;
		size_t _current;
	};
	
	struct ReverseIterator
	{
	public:
		using iterator_category = std::input_iterator_tag;
		using difference_type   = size_t;
		using value_type        = EnumType;
		using pointer           = const EnumType*;
		using reference         = EnumType;
		
	public:
		constexpr ReverseIterator(const EnumeratorMask* mask, size_t current = BIT_LENGTH) : _mask{mask}, _current{findValue(current)}
		{ }
		
		constexpr reference operator*() const { return Meta::MaskConverter::get_bit(_current); }
	
		constexpr ReverseIterator& operator++() { _current = findValue(_current - 1); return *this; }
		constexpr ReverseIterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
	
		constexpr friend bool operator== (const ReverseIterator& a, const ReverseIterator& b) { return a._mask == b._mask && a._current == b._current; }
		constexpr friend bool operator!= (const ReverseIterator& a, const ReverseIterator& b) { return !(a == b); }
		
	protected:
		constexpr size_t findValue(size_t current) const
		{
			while (!_mask->has_bit(current) && current > 0)
			{
				--current;
			}
			
			return current;
		}
		
	private:
		const EnumeratorMask* _mask;
		size_t _current;
	};
	
	template <typename T = DataType, typename std::enable_if<std::is_scalar<T>::value, int>::type = 0>
	static inline constexpr EnumeratorMask all()
	{
		return EnumeratorMask(std::numeric_limits<T>::max());
	}
	
	template <typename T = DataType, typename std::enable_if<!std::is_scalar<T>::value, int>::type = 0>
	static inline constexpr EnumeratorMask all()
	{
		return EnumeratorMask(T::max());
	}
	
public:
	constexpr EnumeratorMask() noexcept : m_data{}
	{	}
	constexpr EnumeratorMask(const EnumeratorMask& other) noexcept = default;
	constexpr EnumeratorMask(EnumType value) noexcept : m_data(Meta::MaskConverter::get_data(value)) 
	{
		static_assert(
			!Meta::bitwise_conversion || ((size_t)Meta::MAX_VALUE) <= bit_length,
			"EnumeratorMask bit_length has to be large enough to contain enum MAX_VALUE"
		);
	}
	constexpr explicit EnumeratorMask(DataType data) noexcept : m_data(data)
	{	}
	
public:
	constexpr DataType data() const
	{
		return m_data;
	}
	
	constexpr bool operator==(const EnumeratorMask& a) const
	{
		return m_data == a.m_data;
	}
	
	constexpr bool operator==(EnumType a) const
	{
		return m_data == Meta::MaskConverter::get_data(a);
	}
	
	constexpr bool operator!=(const EnumeratorMask& a) const
	{
		return m_data != a.m_data;
	}
	
	constexpr bool operator!=(EnumType a) const
	{
		return m_data != Meta::MaskConverter::get_data(a);
	}
	
	constexpr bool is_empty() const
	{
		return m_data == 0;
	}
	
	constexpr bool has(EnumType bit) const
	{
		static_assert(
			!Meta::bitwise_conversion || ((size_t)Meta::MAX_VALUE) <= bit_length,
			"EnumeratorMask bit_length has to be large enough to contain enum MAX_VALUE"
		);
		
		auto val = Meta::MaskConverter::get_data(bit);
		return (m_data & val) == val;
	}
	
	constexpr bool has(EnumeratorMask mask) const
	{
		return (m_data & mask.m_data) == mask.m_data;
	}
	
	constexpr bool has(DataType data) const
	{
		return (m_data & data) == data;
	}
	
	constexpr bool has_any(EnumeratorMask mask) const
	{
		return (m_data & mask.m_data) != 0;
	}
	
	constexpr bool has_any(DataType data) const
	{
		return (m_data & data) != 0;
	}
	
	constexpr void set(EnumType bit, bool value)
	{
		static_assert(
			!Meta::bitwise_conversion || ((size_t)Meta::MAX_VALUE) <= bit_length,
			"EnumeratorMask bit_length has to be large enough to contain enum MAX_VALUE"
		);
		
		auto val = Meta::MaskConverter::get_data(bit);
		if (value)
			m_data |= val;
		else
			m_data &= ~val;
	}
	
	constexpr void set(EnumeratorMask mask, bool value)
	{
		if (value)
			m_data |= mask.m_data;
		else
			m_data &= ~mask.m_data;
	}
	
	constexpr void set(DataType data, bool value)
	{
		if (value)
			m_data |= data;
		else
			m_data &= ~data;
	}
	
	constexpr void clear()
	{
		m_data = 0;
	}

	constexpr EnumeratorMask operator|(EnumType a) const
	{
		static_assert(
			!Meta::bitwise_conversion || ((size_t)Meta::MAX_VALUE) <= bit_length,
			"EnumeratorMask bit_length has to be large enough to contain enum MAX_VALUE"
		);
		
		auto data = Meta::MaskConverter::get_data(a);
		EnumeratorMask m(m_data | data);
		return m;
	}
	
	constexpr EnumeratorMask operator|(EnumeratorMask a) const
	{
		EnumeratorMask m(m_data | a.m_data);
		return m;
	}
	
	constexpr EnumeratorMask operator|=(EnumType a)
	{
		static_assert(
			!Meta::bitwise_conversion || ((size_t)Meta::MAX_VALUE) <= bit_length,
			"EnumeratorMask bit_length has to be large enough to contain enum MAX_VALUE"
		);
		
		m_data = m_data | Meta::MaskConverter::get_data(a);
		return *this;
	}
	
	constexpr EnumeratorMask operator|=(EnumeratorMask a)
	{ 
		m_data = m_data | a.m_data;
		return *this;
	}
	
	constexpr EnumeratorMask operator&(EnumType a) const
	{ 
		EnumeratorMask m(m_data & Meta::MaskConverter::get_data(a));
		return m;
	}
	
	constexpr EnumeratorMask operator&(EnumeratorMask a) const
	{ 
		EnumeratorMask m(m_data & a.m_data);
		return m;
	}
	
	constexpr EnumeratorMask operator&=(EnumType a)
	{ 
		m_data = m_data & Meta::MaskConverter::get_data(a);
		return *this;
	}
	
	constexpr EnumeratorMask operator&=(EnumeratorMask a)
	{ 
		m_data = m_data & a.m_data;
		return *this;
	}
	
	constexpr EnumeratorMask operator^(EnumType a) const
	{ 
		EnumeratorMask m(m_data ^ Meta::MaskConverter::get_data(a));
		return m;
	}
	
	constexpr EnumeratorMask operator^(EnumeratorMask a) const
	{ 
		EnumeratorMask m(m_data ^ a.m_data);
		return m;
	}
	
	constexpr EnumeratorMask operator^=(EnumType a)
	{ 
		m_data = m_data ^ Meta::MaskConverter::get_data(a);
		return *this;
	}
	
	constexpr EnumeratorMask operator^=(EnumeratorMask a)
	{ 
		m_data = m_data ^ a.m_data;
		return *this;
	}
	
	constexpr EnumeratorMask operator~() const
	{ 
		EnumeratorMask m(~m_data);
		return m;
	}
	
	constexpr Iterator begin() const
	{
		return Iterator(this);
	}
	
	constexpr Iterator end() const
	{
		return Iterator(this, BIT_LENGTH + 1);
	}
	
	constexpr Iterator cbegin() const
	{
		return Iterator(this);
	}
	
	constexpr Iterator cend() const
	{
		return Iterator(this, BIT_LENGTH + 1);
	}
		
	constexpr ReverseIterator rbegin() const
	{
		return ReverseIterator(this);
	}
	
	constexpr ReverseIterator rend() const
	{
		return ReverseIterator(this, 0);
	}
	
	constexpr ReverseIterator crbegin() const
	{
		return ReverseIterator(this);
	}
	
	constexpr ReverseIterator crend() const
	{
		return ReverseIterator(this, 0);
	}
	
	constexpr EnumType at(size_t index) const
	{
		auto data = m_data;
		
		while (data != 0)
		{
			auto value = data & 1;
			
			if (value)
			{
				if (index == 0)
					return value;
				
				index--;
			}
			
			data >> 1;
		}
		
		return {};
	}
	
	std::string toString() const
	{
		std::stringstream ss;
		ss << *this;
		
		return ss.str();
	}
	
protected:
	template<typename ValueType = DataType, typename std::enable_if<!std::is_same<ValueType, EnumeratorDataContainer<bit_length>>::value, int>::type = 0>
	constexpr bool has_bit(size_t bit) const
	{
		if (bit == 0)
			return false;
		
		return m_data.has_bit(bit - 1);
	}
	template<typename ValueType = DataType, typename std::enable_if<std::is_same<ValueType, EnumeratorDataContainer<bit_length>>::value, int>::type = 0>
	constexpr bool has_bit(size_t bit) const
	{
		if (bit == 0)
			return false;
		
		auto val = static_cast<DataType>(1) << (bit - 1);
		return (m_data & val) == val;
	}
	
protected:
	DataType m_data;
};


template<typename EnumType, typename std::enable_if<std::is_enum<EnumType>::value && EnumeratorMeta<EnumType>::Specializer::has_base(), int>::type = 0>
inline constexpr typename EnumeratorMeta<EnumType>::BaseEnumType operator+(EnumType value)
{
	using BaseType = typename EnumeratorMeta<EnumType>::BaseEnumType;
	return static_cast<BaseType>(value);
}

template <typename EnumType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType operator+(EnumType a, EnumType b)
{
	using DataType = typename EnumeratorMeta<EnumType>::DataType;
	return static_cast<EnumType>(static_cast<DataType>(a) + static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType operator+(EnumType a, DataType b)
{
	return static_cast<EnumType>(static_cast<DataType>(a) + b);
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr DataType operator+(DataType a, EnumType b)
{
	return (a + static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType& operator+=(EnumType& a, EnumType b)
{
	a = static_cast<EnumType>(static_cast<DataType>(a) + static_cast<DataType>(b));
	return a;
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType& operator+=(EnumType& a, DataType b)
{
	a = static_cast<EnumType>(static_cast<DataType>(a) + b);
	return a;
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr DataType& operator+=(DataType& a, EnumType b)
{
	return (a += static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType& operator++(EnumType& a)
{
	a = static_cast<EnumType>(static_cast<DataType>(a) + 1);
	return a;
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr DataType operator++(EnumType& a, int)
{
	EnumType ret = a;
	a = static_cast<EnumType>(static_cast<DataType>(a) + 1);
	return ret;
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType operator-(EnumType a, EnumType b)
{
	return static_cast<EnumType>(static_cast<DataType>(a) - static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType operator-(EnumType a, DataType b)
{
	return static_cast<EnumType>(static_cast<DataType>(a) - b);
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr DataType operator-(DataType a, EnumType b)
{
	return (a - static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType& operator-=(EnumType& a, EnumType b)
{
	a = static_cast<EnumType>(static_cast<DataType>(a) - static_cast<DataType>(b));
	return a;
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType& operator-=(EnumType& a, DataType b)
{
	a = static_cast<EnumType>(static_cast<DataType>(a) - b);
	return a;
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr DataType& operator-=(DataType& a, EnumType b)
{
	return (a -= static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType& operator--(EnumType& a)
{
	a = static_cast<EnumType>(static_cast<DataType>(a) - 1);
	return a;
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr DataType operator--(EnumType& a, int)
{
	EnumType ret = a;
	a = static_cast<EnumType>(static_cast<DataType>(a) - 1);
	return ret;
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType operator<<(EnumType a, EnumType b)
{
	return static_cast<EnumType>(static_cast<DataType>(a) << static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, 
					typename std::enable_if<EnumeratorMeta<EnumType>::math_operators && std::is_integral<DataType>::value, int>::type = 0>
inline constexpr DataType operator<<(DataType a, EnumType b)
{
	return (a << static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators && std::is_integral<DataType>::value, int>::type = 0>
inline constexpr EnumType operator<<(EnumType a, DataType b)
{
	return static_cast<EnumType>(static_cast<DataType>(a) << b);
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators, int>::type = 0>
inline constexpr EnumType operator>>(EnumType a, EnumType b)
{
	return static_cast<EnumType>(static_cast<DataType>(a) >> static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators && std::is_integral<DataType>::value, int>::type = 0>
inline constexpr DataType operator>>(DataType a, EnumType b)
{
	return (a >> static_cast<DataType>(b));
}

template <typename EnumType, typename DataType = typename EnumeratorMeta<EnumType>::DataType, typename std::enable_if<EnumeratorMeta<EnumType>::math_operators && std::is_integral<DataType>::value, int>::type = 0>
inline constexpr EnumType operator>>(EnumType a, DataType b)
{
	return static_cast<EnumType>(static_cast<DataType>(a) >> b);
}

template <typename EnumType, typename std::enable_if<EnumeratorMeta<EnumType>::logic_operators, int>::type = 0>
constexpr typename EnumeratorMeta<EnumType>::MaskType operator|(EnumType a, EnumType b)
{
	return (typename EnumeratorMeta<EnumType>::MaskType(a) | b);
}

template <typename EnumType, typename std::enable_if<EnumeratorMeta<EnumType>::logic_operators, int>::type = 0>
constexpr typename EnumeratorMeta<EnumType>::MaskType operator&(EnumType a, EnumType b)
{
	return (typename EnumeratorMeta<EnumType>::MaskType(a) & b);
}

template <typename EnumType, typename std::enable_if<EnumeratorMeta<EnumType>::logic_operators, int>::type = 0>
constexpr typename EnumeratorMeta<EnumType>::MaskType operator^(EnumType a, EnumType b)
{
	return (typename EnumeratorMeta<EnumType>::MaskType(a) ^ b);
}

template <typename EnumType, typename std::enable_if<EnumeratorMeta<EnumType>::logic_operators, int>::type = 0>
constexpr typename EnumeratorMeta<EnumType>::MaskType operator~(EnumType a)
{
	return (~typename EnumeratorMeta<EnumType>::MaskType(a));
}

template <typename EnumType, typename std::enable_if<EnumeratorMeta<EnumType>::string_operators, int>::type = 0>
inline std::ostream& operator<<(std::ostream& os, EnumType value)
{
	auto name = EnumeratorSerializer<EnumType>::get_name(value);
	
	if (name != nullptr)
		return os << name;
	
	return os;
}

template <typename EnumType, size_t bit_length, typename std::enable_if<EnumeratorMeta<EnumType>::string_operators, int>::type = 0>
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

template <typename EnumType_, bool isFlags = false, size_t max_value = std::numeric_limits<typename std::make_unsigned<EnumType_>::type>::digits>
class EnumeratorMetaDefault
{
public:
	using EnumType = EnumType_;
	using DataType = typename std::underlying_type<EnumType>::type;
	using Converter = EnumeratorConverter<EnumType, DataType, max_value, isFlags>;
	using Extender = EnumeratorExtender<EnumType>;
	using Inheritor = EnumeratorInheritor<EnumType>;
	using Specializer = EnumeratorSpecializer<EnumType>;
	using MaskType = EnumeratorMask<EnumType, max_value>;
	using MaskDataType = typename MaskType::DataType;
	using MaskConverter = EnumeratorConverter<EnumType, MaskDataType, max_value, isFlags>;
	static constexpr const bool bitwise_conversion = !isFlags;
	
	struct EnumEntry
	{
		EnumType value;
		const char* name;
		const char* label;
		
		constexpr EnumEntry(EnumType val, const char* name_) : value(val), name(name_), label("") { }
		constexpr EnumEntry(EnumType val, const char* name_, const char* label_) : value(val), name(name_), label(label_) { }
		
		constexpr EnumType get_value() const { return value; }
		constexpr const char* get_name() const { return name; }
		constexpr const char* get_label() const { return label; }
	};
};

#ifdef ENUMERATORMETA_NAMESPACE
}
#endif


#endif
