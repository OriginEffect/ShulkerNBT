#pragma once

#include <cstdint>

#include "shulker/detail/MacroScope.h"

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
	Compound	= 0x0a,
	IntArray	= 0x0b,
	LongArray	= 0x0c,

	Unknown		= 0xff
};

using TagId = std::underlying_type<TagType>::type;

inline TagId getTagId(const TagType tag) noexcept
{
    return static_cast<TagId>(tag);
}

inline std::string getTypeName(const TagType tag_type) noexcept
{
    switch (tag_type) {
        case TagType::End:
            return "end tag";
        case TagType::Byte:
            return "byte tag";
        case TagType::Short:
            return "short tag";
        case TagType::Int:
            return "int tag";
        case TagType::Long:
            return "long tag";
        case TagType::Float:
            return "float tag";
        case TagType::Double:
            return "double tag";
        case TagType::ByteArray:
            return "byte array tag";
        case TagType::String:
            return "string tag";
        case TagType::List:
            return "list tag";
        case TagType::Compound:
            return "compound tag";
        case TagType::IntArray:
            return "int array tag";
        case TagType::LongArray:
            return "long array tag";
        case TagType::Unknown:
        default:
            return "unknown tag";
    }
}

enum class TagValue : std::uint8_t
{
    End,
	Compound,
	List,
	ByteArray,
	IntArray,
	LongArray,
	Byte,
	Short,
	Int,
	Long,
	Float,
	Double,
	Bool,
	String,

	Discarded = 0xff
};

SHULKER_NBT_NAMESPACE_END
