#include <filesystem>
#include <utility>

#include "shulker/detail/io/output/NbtWriter.h"
#include "shulker/detail/io/output/OutputAdapters.h"
#include "shulker/detail/io/output/SnbtSerializer.h"
#include "shulker/tags/CompoundTag.h"

#include "shulker/tags/TagReference.h"

SHULKER_NBT_NAMESPACE_BEGIN

CompoundTag::CompoundTag() : BasicTag(TagValue::Compound) {}

CompoundTag::CompoundTag(const CompoundType& map): BasicTag(TagValue::Compound), m_value(map) {}

CompoundTag::CompoundTag(CompoundType&& map) : BasicTag(TagValue::Compound), m_value(std::move(map)) {}

CompoundTag::CompoundTag(std::initializer_list<std::pair<CompoundType::key_type, TagReference>> init)
    : BasicTag(TagValue::Compound), m_value(init.begin(), init.end()) {}

std::vector<char> CompoundTag::dumpBinary(
    bool use_big_endian,
    CompressionMode compression_mode)
{
    std::vector<char> result;
    detail::NbtWriter writer(detail::OutputAdapter<char>(result), use_big_endian, compression_mode);
    writer.write(*this, "");
    return result;
}

// TODO: 文件写入错误处理
std::size_t CompoundTag::dumpFile(
    const std::string& path,
    bool use_big_endian,
    CompressionMode compression_mode)
{
    namespace fs = std::filesystem;
    fs::path f = path;
    fs::path dir = f.parent_path();

    if (!dir.empty() && !fs::exists(dir)) {
        fs::create_directory(dir);
    }

    std::ofstream ofs(f, std::ios::binary);
    detail::NbtWriter writer(detail::OutputAdapter<char>(ofs), use_big_endian, compression_mode);
    return writer.write(*this, "");
}

TagReference& CompoundTag::operator[](CompoundType::key_type key)
{
    return m_value.emplace(std::move(key), EndTag{}).first->second;
}

// const TagReference& CompoundTag::operator[](const CompoundType::key_type& key) const
// {
//
// }

std::ostream& operator<<(std::ostream& os, const CompoundTag& compound_tag)
{
    const bool pretty_print = os.width() > 0;
    const auto indentation = pretty_print ? os.width() : 0;

    os.width(0);

    detail::SnbtSerializer serializer(detail::OutputAdapter<char>(os), os.fill());
    serializer.dump(compound_tag, pretty_print, false, false, static_cast<unsigned int>(indentation));
    return os;
}

SHULKER_NBT_NAMESPACE_END
