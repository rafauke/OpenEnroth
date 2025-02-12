#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include "Utility/Memory/Blob.h"
#include "Utility/Flags.h"

enum class ObjectDescFlag : int16_t {
    OBJECT_DESC_NO_SPRITE = 0x1,
    OBJECT_DESC_NO_COLLISION = 0x2,
    OBJECT_DESC_TEMPORARY = 0x4,
    OBJECT_DESC_SFT_LIFETIME = 0x8,
    OBJECT_DESC_UNPICKABLE = 0x10,
    OBJECT_DESC_NO_GRAVITY = 0x20,
    OBJECT_DESC_INTERACTABLE = 0x40,
    OBJECT_DESC_BOUNCE = 0x80,
    OBJECT_DESC_TRIAL_PARTICLE = 0x100,
    OBJECT_DESC_TRIAL_FIRE = 0x200,
    OBJECT_DESC_TRIAL_LINE = 0x400,
};
using enum ObjectDescFlag;
MM_DECLARE_FLAGS(ObjectDescFlags, ObjectDescFlag)
MM_DECLARE_OPERATORS_FOR_FLAGS(ObjectDescFlags)

struct ObjectDesc {
    inline bool NoSprite() const { return uFlags & OBJECT_DESC_NO_SPRITE; }

    std::array<char, 32> field_0;
    int16_t uObjectID;
    int16_t uRadius;
    int16_t uHeight;
    ObjectDescFlags uFlags;
    uint16_t uSpriteID;
    int16_t uLifetime;
    uint32_t uParticleTrailColor;
    int16_t uSpeed;
    uint8_t uParticleTrailColorR;
    uint8_t uParticleTrailColorG;
    uint8_t uParticleTrailColorB;
};

class ObjectList {
 public:
    void FromFile(const Blob &data_mm6, const Blob &data_mm7, const Blob &data_mm8);
    void InitializeSprites();
    void InitializeColors();
    unsigned int ObjectIDByItemID(unsigned int uItemID);

 public:
    std::vector<ObjectDesc> pObjects;
};

extern ObjectList *pObjectList;
