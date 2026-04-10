#pragma once

#include "shulker/detail/MacroScope.h"

#include "shulker/CompressionMode.h"
#include "shulker/detail/io/input/InputAdapters.h"
#include "shulker/detail/io/input/NbtReader.h"

SHULKER_NBT_NAMESPACE_BEGIN

using NbtRecord = detail::NbtRecord;

template<typename InputType>
CompoundTag readNbt(
    InputType&& input,
    bool useBigEndian = true,
    CompressionMode compression_type = CompressionMode::Unknown)
{
    detail::NbtReader reader(detail::InputAdapter(std::forward<InputType>(input)), useBigEndian, compression_type);
    return std::move(reader.readRoot().as<CompoundTag>());
}

template<typename InputType>
NbtRecord readTag(
    InputType&& input,
    bool useBigEndian = true,
    TagType tag_type = TagType::Unknown,
    CompressionMode compression_type = CompressionMode::Unknown)
{
    detail::NbtReader reader(detail::InputAdapter(std::forward<InputType>(input)), useBigEndian, compression_type);
    return reader.read(tag_type);
}

SHULKER_NBT_NAMESPACE_END
