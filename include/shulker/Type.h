#pragma once

#include <cstdint>

#include "shulker/common/MacroScope.h"

SHULKER_NBT_NAMESPACE_BEGIN

enum class TagType : std::uint8_t
{
	End			= 0x00,
	Byte		= 0x01,
	Short		= 0x02,
	Int			= 0x03,
	Long		= 0x04,
	Float		= 0x05,
	Double		= 0x06,
	ByteArray	= 0x07,
	String		= 0x08,
	List		= 0x09,
	Compound	= 0x0A,
	IntArray	= 0x0B,
	LongArray	= 0x0C,

	Unknown		= 0xFF
};

using TagId = std::underlying_type<TagType>::type;

enum class TagValue : std::uint8_t
{
    End,
	Compound,
	List,
	ByteArray,
	IntArray,
	LongArray,
	SignedByte,
	UnsignedByte,
	SignedShort,
	UnsignedShort,
	SignedInt,
	UnsignedInt,
	SignedLong,
	UnsignedLong,
	Float,
	Double,
	Bool,
	String,

	Discarded = 0xFF
};

SHULKER_NBT_NAMESPACE_END
