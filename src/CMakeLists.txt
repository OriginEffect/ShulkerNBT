set(_src
    detail/Exceptions.cpp

    detail/io/input/NbtDecompressor.cpp
    detail/io/input/NbtReader.cpp
    detail/io/output/NbtCompressor.cpp
    detail/io/output/NbtWriter.cpp
    detail/io/output/SnbtSerializer.cpp

    tags/BasicTag.cpp
    tags/ByteArrayTag.cpp
    tags/ByteTag.cpp
    tags/CompoundTag.cpp
    tags/DoubleTag.cpp
    tags/EndTag.cpp
    tags/FloatTag.cpp
    tags/IntArrayTag.cpp
    tags/IntTag.cpp
    tags/ListTag.cpp
    tags/LongArrayTag.cpp
    tags/LongTag.cpp
    tags/ShortTag.cpp
    tags/StringTag.cpp
    tags/TagReference.cpp
)

target_sources(${SHULKER_NBT_LIB_NAME}
	PRIVATE
	${_src}
)
