#pragma once

#include <cstdint>
#include <array>
#include <string>

#include "SpellEnums.h"

#include "Engine/Objects/ItemEnums.h"
#include "Engine/Objects/PlayerEnums.h"
#include "Engine/Objects/SpriteObjectType.h"
#include "Engine/Time.h"

#include "Utility/IndexedArray.h"
#include "Utility/Geometry/Vec.h"

// TODO(pskelton): move to own files
// TODO(pskelton): style
struct SpellBuff {
    /**
     * @offset 0x4584E0
     */
    bool Apply(GameTime time, PLAYER_SKILL_MASTERY uSkillMastery,
               PLAYER_SKILL_LEVEL uPower, int uOverlayID, uint8_t caster);

    /**
     * @offset 0x458585
     */
    void Reset();

    /**
     * @offset 0x4585CA
     */
    bool IsBuffExpiredToTime(GameTime time);

    /**
     * @offset 0x42EB31
     * Active is state where spell buff is in effect
     */
    bool Active() const { return this->expireTime.Valid(); }
    /**
    * Inactive is state where spell buff is not in effect (includes state expired)
    */
    bool Inactive() const { return !Active(); }
    /**
    * Expired is state where spell buff is not in effect after previously being active    
    */
    bool Expired() const { return this->expireTime.Expired(); }
    GameTime &GetExpireTime() { return this->expireTime; }

    GameTime expireTime;
    uint16_t power = 0; // Spell power, semantics are spell-specific.
    PLAYER_SKILL_MASTERY skillMastery = PLAYER_SKILL_MASTERY_NONE; // 1-4, normal to grandmaster.
    uint16_t overlayID = 0;
    uint8_t caster = 0;
    bool isGMBuff = false; // Buff was casted at grandmaster mastery
};

struct SpellInfo {
    std::string name;
    std::string pShortName;
    std::string pDescription;
    std::string pBasicSkillDesc;
    std::string pExpertSkillDesc;
    std::string pMasterSkillDesc;
    std::string pGrandmasterSkillDesc;
    SPELL_SCHOOL uSchool;
    int field_20;
};

struct SpellStats {
    /**
     * @offset 0x45384A
     */
    void Initialize();

    IndexedArray<SpellInfo, SPELL_FIRST_REGULAR, SPELL_LAST_REGULAR> pInfos;
};

class SpellData {
 public:
    SpellData():SpellData(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PLAYER_SKILL_MASTERY_NONE) {}
    SpellData(int16_t inNormalMana, int16_t inExpertLevelMana,
              int16_t inMasterLevelMana, int16_t inMagisterLevelMana,
              int16_t inNormalLevelRecovery, int16_t inExpertLevelRecovery,
              int16_t inMasterLevelRecovery, int16_t inMagisterLevelRecovery,
              int8_t inBaseDamage, int8_t inBonusSkillDamage, int16_t inStats,
              PLAYER_SKILL_MASTERY inSkillMastery);
    union {
        uint16_t mana_per_skill[4];
        struct {
            uint16_t uNormalLevelMana;
            uint16_t uExpertLevelMana;
            uint16_t uMasterLevelMana;
            uint16_t uMagisterLevelMana;
        };
    };
    union {
        uint16_t recovery_per_skill[4];
        struct {
            uint16_t uNormalLevelRecovery;
            uint16_t uExpertLevelRecovery;
            uint16_t uMasterLevelRecovery;
            uint16_t uMagisterLevelRecovery;
        };
    };
    int8_t baseDamage;
    int8_t bonusSkillDamage;
    int16_t stats;
    PLAYER_SKILL_MASTERY skillMastery;
    // char field_12;
    // char field_13;
    // int16_t field_14;
};

struct SpellBookIconPos {
    int32_t Xpos;
    int32_t Ypos;
};

struct TownPortalData {
    Vec3i pos;
    int16_t _viewYaw;
    int16_t _viewPitch;
    uint16_t uMapInfoID;
    int16_t field_12;
};

extern std::array<TownPortalData, 6> TownPortalList;  // 4ECBB8

extern struct SpellStats *pSpellStats;

extern std::array<std::array<struct SpellBookIconPos, 12>, 9> pIconPos;

extern IndexedArray<SPRITE_OBJECT_TYPE, SPELL_FIRST_WITH_SPRITE, SPELL_LAST_WITH_SPRITE> SpellSpriteMapping;  // 4E3ACC
extern IndexedArray<SpellData, SPELL_FIRST_REGULAR, SPELL_LAST_REGULAR> pSpellDatas;
extern IndexedArray<SPELL_TYPE, ITEM_FIRST_WAND, ITEM_LAST_WAND> wandSpellIds;
extern IndexedArray<SPELL_TYPE, ITEM_FIRST_SPELL_SCROLL, ITEM_LAST_SPELL_SCROLL> scrollSpellIds;
extern IndexedArray<SPELL_TYPE, ITEM_FIRST_SPELL_BOOK, ITEM_LAST_SPELL_BOOK> bookSpellIds;
extern IndexedArray<uint16_t, SPELL_FIRST_WITH_SPRITE, SPELL_LAST_WITH_SPRITE> SpellSoundIds;

/**
 * @offset 0x43AFE3
 */
int CalcSpellDamage(SPELL_TYPE uSpellID, PLAYER_SKILL_LEVEL spellLevel, PLAYER_SKILL_MASTERY skillMastery, int currentHp);

/**
 * @offset 0x427769
 */
bool IsSpellQuickCastableOnShiftClick(SPELL_TYPE uSpellID);

/**
 * Function for processing spells cast from game scripts.
 */
void eventCastSpell(SPELL_TYPE uSpellID, PLAYER_SKILL_MASTERY skillMastery, PLAYER_SKILL_LEVEL skillLevel, int fromx,
                    int fromy, int fromz, int tox, int toy, int toz);  // sub_448DF8

void armageddonProgress();
