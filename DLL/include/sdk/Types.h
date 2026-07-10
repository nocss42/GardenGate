#pragma once

#include <stdint.h>

namespace fb
{
using s8 = int8_t;
using u8 = uint8_t;
using s16 = int16_t;
using u16 = uint16_t;
using s32 = int32_t;
using u32 = uint32_t;
using s64 = int64_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using String = char*;

class MemberInfoData
{
public:
    char* name;
    uint16_t flags;
};

class TypeInfoData : public MemberInfoData
{
public:
    uint16_t totalSize;
    uint32_t guid;
    class ModuleInfo* module;
    class TypeInfo* arrayTypeInfo;
    uint16_t alignment;
    uint16_t fieldCount;
    uint32_t signature;
};

class TypeInfo
{
public:
    class TypeInfoData* typeInfoData;
    TypeInfo* next;

    char pad[0x28];
    const TypeInfo* m_super;
    const intptr_t m_defaultInstance;
    uint16_t m_classId;
    uint16_t m_lastClassId;

    const char* getName() const
    {
        return typeInfoData->name;
    }
};

struct ITypedObject
{
    virtual class TypeInfo* getType() const = 0;

protected:
    virtual ~ITypedObject() = default;
};
} // namespace fb