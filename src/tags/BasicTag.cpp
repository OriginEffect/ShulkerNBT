#include "shulker/io/adapters/OutputAdapters.h"
#include "shulker/io/snbt/SnbtSerializer.h"
#include "shulker/tags/BasicTag.h"

SHULKER_NBT_NAMESPACE_BEGIN

TagValue BasicTag::valueType() const
{
    return m_value_type;
}

std::string BasicTag::dumpSnbt(
    const int indent,
    const char indent_char,
    const bool keep_snbt_type) const
{
    std::string result;
    SnbtSerializer serializer(OutputAdapter<char>(result), indent_char);

    if (indent >= 0) {
        serializer.dump(*this, true, keep_snbt_type, static_cast<unsigned int>(indent));
    } else {
        serializer.dump(*this, false, keep_snbt_type, 0);
    }

    return result;
}

SHULKER_NBT_NAMESPACE_END
