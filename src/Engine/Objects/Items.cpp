#include "Engine/Objects/Items.h"

#include <map>
#include <string>
#include <vector>

#include "Engine/Engine.h"
#include "Engine/Localization.h"
#include "Engine/Objects/ItemEnchantment.h"
#include "Engine/Objects/ItemTable.h"
#include "Engine/Objects/Monsters.h"
#include "Engine/Objects/PlayerEnums.h"
#include "Engine/OurMath.h"
#include "Engine/Party.h"

#include "GUI/GUIButton.h"


std::array<std::array<char, 14>, 7> byte_4E8168 = {{  // byte_4E8178 -treasure levles
    {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
    {{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}},
    {{1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3}},
    {{2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4}},
    {{2, 2, 2, 2, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5}},
    {{2, 2, 2, 2, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6}},
    {{2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7}}
}};

ItemGen* ptr_50C9A4_ItemToEnchant;

struct ItemTable* pItemTable;  // 005D29E0

static std::map<int, std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment>> regularBonusMap;
static std::map<int, std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment>> specialBonusMap;
static std::map<ITEM_TYPE, std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment>> artifactBonusMap;

//----- (00439DF3) --------------------------------------------------------
int ItemGen::_439DF3_get_additional_damage(DAMAGE_TYPE* damage_type,
                                           bool* draintargetHP) {
    *draintargetHP = false;
    *damage_type = DMGT_FIRE;
    if (uItemID == ITEM_NULL) return 0;

    UpdateTempBonus(pParty->GetPlayingTime());
    if (uItemID == ITEM_ARTIFACT_IRON_FEATHER) {
        *damage_type = DMGT_ELECTR;
        return rand() % 10 + 6;
    }
    if (uItemID == ITEM_ARTIFACT_GHOULSBANE) {
        *damage_type = DMGT_FIRE;
        return rand() % 16 + 3;
    }
    if (uItemID == ITEM_ARTIFACT_ULLYSES) {
        *damage_type = DMGT_COLD;
        return rand() % 4 + 9;
    }
    if (uItemID == ITEM_RELIC_OLD_NICK) {
        *damage_type = DMGT_BODY;
        return 8;
    }

    switch (special_enchantment) {
        case ITEM_ENCHANTMENT_OF_COLD:
            *damage_type = DMGT_COLD;
            return rand() % 2 + 3;
            break;
        case ITEM_ENCHANTMENT_OF_FROST:
            *damage_type = DMGT_COLD;
            return rand() % 3 + 6;
            break;
        case ITEM_ENCHANTMENT_OF_ICE:
            *damage_type = DMGT_COLD;
            return rand() % 4 + 9;
            break;
        case ITEM_ENCHANTMENT_OF_SPARKS:
            *damage_type = DMGT_ELECTR;
            return rand() % 4 + 2;
            break;
        case ITEM_ENCHANTMENT_OF_LIGHTNING:
            *damage_type = DMGT_ELECTR;
            return rand() % 7 + 4;
            break;
        case ITEM_ENCHANTMENT_OF_THUNDERBOLTS:
            *damage_type = DMGT_ELECTR;
            return rand() % 10 + 6;
            break;
        case ITEM_ENCHANTMENT_OF_FIRE:
            *damage_type = DMGT_FIRE;
            return GetDiceResult(1, 6);
            break;
        case ITEM_ENCHANTMENT_OF_FLAME:
            *damage_type = DMGT_FIRE;
            return GetDiceResult(2, 6);
            break;
        case ITEM_ENCHANTMENT_OF_INFERNOS:
            *damage_type = DMGT_FIRE;
            return GetDiceResult(3, 6);
            break;
        case ITEM_ENCHANTMENT_OF_POISON:
            *damage_type = DMGT_BODY;
            return 5;
            break;
        case ITEM_ENCHANTMENT_OF_VENOM:
            *damage_type = DMGT_BODY;
            return 8;
            break;
        case ITEM_ENCHANTMENT_OF_ACID:
            *damage_type = DMGT_BODY;
            return 12;
            break;
        case ITEM_ENCHANTMENT_VAMPIRIC:
        case ITEM_ENCHANTMENT_OF_DARKNESS:
            *damage_type = DMGT_DARK;
            *draintargetHP = true;
            return 0;
            break;

        case ITEM_ENCHANTMENT_OF_DRAGON:
            *damage_type = DMGT_FIRE;
            return rand() % 11 + 10;
            break;
        default:
            *damage_type = DMGT_FIRE;
            return 0;
    }
}

//----- (00402F07) --------------------------------------------------------
void ItemGen::Reset() {
    *this = ItemGen();
}

//----- (00458260) --------------------------------------------------------
void ItemGen::UpdateTempBonus(GameTime time) {
    if (this->uAttributes & ITEM_TEMP_BONUS) {
        if (time > this->uExpireTime) {
            this->uEnchantmentType = 0;
            this->special_enchantment = ITEM_ENCHANTMENT_NULL;
            this->uAttributes &= ~ITEM_TEMP_BONUS;
        }
    }
}

//----- (00456442) --------------------------------------------------------
unsigned int ItemGen::GetValue() {
    unsigned int uBaseValue;  // edi@1
    unsigned int mod, bonus;

    uBaseValue = pItemTable->pItems[this->uItemID].uValue;
    if (this->uAttributes & ITEM_TEMP_BONUS ||
        pItemTable->IsMaterialNonCommon(this))
        return uBaseValue;
    if (uEnchantmentType) return uBaseValue + 100 * m_enchantmentStrength;

    if (special_enchantment != ITEM_ENCHANTMENT_NULL) {
        mod = (pItemTable->pSpecialEnchantments[special_enchantment].iTreasureLevel & 4);
        bonus = pItemTable->pSpecialEnchantments[special_enchantment].iValue;
        if (!mod)
            return uBaseValue + bonus;
        else
            return uBaseValue * bonus;
    }
    return uBaseValue;
}

//----- (00456499) --------------------------------------------------------
std::string ItemGen::GetDisplayName() {
    if (IsIdentified()) {
        return GetIdentifiedName();
    } else {
        return pItemTable->pItems[uItemID].pUnidentifiedName;
    }
}

//----- (004564B3) --------------------------------------------------------
std::string ItemGen::GetIdentifiedName() {
    ITEM_EQUIP_TYPE equip_type = GetItemEquipType();
    if ((equip_type == EQUIP_REAGENT) || (equip_type == EQUIP_POTION) ||
        (equip_type == EQUIP_GOLD)) {
        return pItemTable->pItems[uItemID].pName;
    }

    if (uItemID == ITEM_QUEST_LICH_JAR_FULL) {  // Lich Jar
        if (uHolderPlayer > 0 && uHolderPlayer <= 4) {
            const std::string &player_name = pPlayers[uHolderPlayer]->pName;
            if (player_name.back() == 's')
                return localization->FormatString(
                    LSTR_FMT_JAR,
                    player_name.c_str());
            else
                return localization->FormatString(
                    LSTR_FMT_JAR_2,
                    player_name.c_str());
        }
    }

    if (!pItemTable->IsMaterialNonCommon(this)) {
        if (uEnchantmentType) {
            return std::string(pItemTable->pItems[uItemID].pName) + " " +
                   pItemTable->pEnchantments[uEnchantmentType - 1].pOfName;
        } else if (special_enchantment == ITEM_ENCHANTMENT_NULL) {
            return pItemTable->pItems[uItemID].pName;
        } else {
            if (special_enchantment == ITEM_ENCHANTMENT_VAMPIRIC
                || special_enchantment == ITEM_ENCHANTMENT_DEMON_SLAYING
                || special_enchantment == ITEM_ENCHANTMENT_DRAGON_SLAYING
                || special_enchantment == ITEM_ENCHANTMENT_ROGUES
                || special_enchantment == ITEM_ENCHANTMENT_WARRIORS
                || special_enchantment == ITEM_ENCHANTMENT_WIZARDS
                || special_enchantment == ITEM_ENCHANTMENT_ANTIQUE
                || special_enchantment == ITEM_ENCHANTMENT_MONKS
                || special_enchantment == ITEM_ENCHANTMENT_THIEVES
                || special_enchantment == ITEM_ENCHANTMENT_SWIFT
                || special_enchantment == ITEM_ENCHANTMENT_ELF_SLAYING
                || special_enchantment == ITEM_ENCHANTMENT_UNDEAD_SLAYING
                || special_enchantment == ITEM_ENCHANTMENT_ASSASINS
                || special_enchantment == ITEM_ENCHANTMENT_BARBARIANS
            ) {            // enchantment and name positions inverted!
                return StringPrintf(
                    "%s %s",
                    pItemTable->pSpecialEnchantments[special_enchantment].pNameAdd,
                    pItemTable->pItems[uItemID].pName
                );
            } else {
                return std::string(pItemTable->pItems[uItemID].pName) + " " +
                       pItemTable->pSpecialEnchantments[special_enchantment].pNameAdd;
            }
        }
    }

    return pItemTable->pItems[uItemID].pName;
}

//----- (004505CC) --------------------------------------------------------
bool ItemGen::GenerateArtifact() {
    signed int uNumArtifactsNotFound;  // esi@1
    std::array<ITEM_TYPE, 32> artifacts_list;

    artifacts_list.fill(ITEM_NULL);
    uNumArtifactsNotFound = 0;

    for (ITEM_TYPE i : pParty->pIsArtifactFound.indices())
        if (!pParty->pIsArtifactFound[i])
            artifacts_list[uNumArtifactsNotFound++] = i;

    Reset();
    if (uNumArtifactsNotFound) {
        uItemID = artifacts_list[rand() % uNumArtifactsNotFound];
        pItemTable->SetSpecialBonus(this);
        return true;
    } else {
        return false;
    }
}

template<class Key, class ActualKey>
static void AddToMap(std::map<Key, std::map<CHARACTER_ATTRIBUTE_TYPE, CEnchantment>> &map,
                     ActualKey key, CHARACTER_ATTRIBUTE_TYPE subkey, int bonusValue = 0, uint16_t Player::*skillPtr = NULL) {
    auto &submap = map[key];

    Assert(!submap.contains(subkey));

    submap[subkey] = CEnchantment(bonusValue, skillPtr);
}

void ItemGen::PopulateSpecialBonusMap() {
    // of Protection, +10 to all Resistances (description in txt says all 4, need to verify!)
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_PROTECTION, CHARACTER_ATTRIBUTE_RESIST_AIR, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_PROTECTION, CHARACTER_ATTRIBUTE_RESIST_BODY, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_PROTECTION, CHARACTER_ATTRIBUTE_RESIST_EARTH, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_PROTECTION, CHARACTER_ATTRIBUTE_RESIST_FIRE, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_PROTECTION, CHARACTER_ATTRIBUTE_RESIST_MIND, 10);
    //AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_PROTECTION, CHARACTER_ATTRIBUTE_RESIST_SPIRIT, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_PROTECTION, CHARACTER_ATTRIBUTE_RESIST_WATER, 10);

    // of The Gods, +10 to all Seven Statistics
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_GODS, CHARACTER_ATTRIBUTE_ACCURACY, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_GODS, CHARACTER_ATTRIBUTE_INTELLIGENCE, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_GODS, CHARACTER_ATTRIBUTE_ENDURANCE, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_GODS, CHARACTER_ATTRIBUTE_LUCK, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_GODS, CHARACTER_ATTRIBUTE_SPEED, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_GODS, CHARACTER_ATTRIBUTE_STRENGTH, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_GODS, CHARACTER_ATTRIBUTE_WILLPOWER, 10);

    // of Air Magic
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_AIR_MAGIC, CHARACTER_ATTRIBUTE_SKILL_AIR, 0, &Player::skillAir);

    // of Body Magic
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_BODY_MAGIC, CHARACTER_ATTRIBUTE_SKILL_BODY, 0, &Player::skillBody);

    // of Dark Magic
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DARK_MAGIC, CHARACTER_ATTRIBUTE_SKILL_DARK, 0, &Player::skillDark);

    // of Earth Magic
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_EARTH_MAGIC, CHARACTER_ATTRIBUTE_SKILL_EARTH, 0, &Player::skillEarth);

    // of Fire Magic
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_FIRE_MAGIC, CHARACTER_ATTRIBUTE_SKILL_FIRE, 0, &Player::skillFire);

    // of Light Magic
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_LIGHT_MAGIC, CHARACTER_ATTRIBUTE_SKILL_LIGHT, 0, &Player::skillLight);

    // of Mind Magic
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_MIND_MAGIC, CHARACTER_ATTRIBUTE_SKILL_MIND, 0, &Player::skillMind);

    // of Spirit Magic
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_SPIRIT_MAGIC, CHARACTER_ATTRIBUTE_SKILL_SPIRIT, 0, &Player::skillSpirit);

    // of Water Magic
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_WATER_MAGIC, CHARACTER_ATTRIBUTE_SKILL_WATER, 0, &Player::skillWater);

    // of Doom, +1 to Seven Stats, HP, SP, Armor, Resistances (in txt it says 4, need to check!)
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_ACCURACY, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_INTELLIGENCE, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_ENDURANCE, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_LUCK, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_SPEED, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_STRENGTH, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_WILLPOWER, 1);

    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_RESIST_AIR, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_RESIST_BODY, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_RESIST_EARTH, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_RESIST_FIRE, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_RESIST_MIND, 1);
    //AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_RESIST_SPIRIT, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_RESIST_WATER, 1);

    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_AC_BONUS, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_HEALTH, 1);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DOOM, CHARACTER_ATTRIBUTE_MANA, 1);

    // of Earth
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_EARTH, CHARACTER_ATTRIBUTE_AC_BONUS, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_EARTH, CHARACTER_ATTRIBUTE_ENDURANCE, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_EARTH, CHARACTER_ATTRIBUTE_HEALTH, 10);

    // of Life
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_LIFE, CHARACTER_ATTRIBUTE_HEALTH, 10);

    // Rogues
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_ROGUES, CHARACTER_ATTRIBUTE_ACCURACY, 5);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_ROGUES, CHARACTER_ATTRIBUTE_SPEED, 5);

    // of The Dragon
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_DRAGON, CHARACTER_ATTRIBUTE_STRENGTH, 25);

    // of The Eclipse
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_ECLIPSE, CHARACTER_ATTRIBUTE_MANA, 10);

    // of The Golem
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_GOLEM, CHARACTER_ATTRIBUTE_AC_BONUS, 5);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_GOLEM, CHARACTER_ATTRIBUTE_ENDURANCE, 15);

    // of The Moon
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_MOON, CHARACTER_ATTRIBUTE_INTELLIGENCE, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_MOON, CHARACTER_ATTRIBUTE_LUCK, 10);

    // of The Phoenix
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_PHOENIX, CHARACTER_ATTRIBUTE_RESIST_FIRE, 30);

    // of The Sky
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_SKY, CHARACTER_ATTRIBUTE_INTELLIGENCE, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_SKY, CHARACTER_ATTRIBUTE_MANA, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_SKY, CHARACTER_ATTRIBUTE_SPEED, 10);

    // of The Stars
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_STARS, CHARACTER_ATTRIBUTE_ACCURACY, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_STARS, CHARACTER_ATTRIBUTE_ENDURANCE, 10);

    // of The Sun
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_SUN, CHARACTER_ATTRIBUTE_STRENGTH, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_SUN, CHARACTER_ATTRIBUTE_WILLPOWER, 10);

    // of The Troll
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_TROLL, CHARACTER_ATTRIBUTE_ENDURANCE, 15);

    // of The Unicorn
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_UNICORN, CHARACTER_ATTRIBUTE_LUCK, 15);

    // Warriors
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_WARRIORS, CHARACTER_ATTRIBUTE_ENDURANCE, 5);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_WARRIORS, CHARACTER_ATTRIBUTE_STRENGTH, 5);

    // Wizards
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_WIZARDS, CHARACTER_ATTRIBUTE_INTELLIGENCE, 5);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_WIZARDS, CHARACTER_ATTRIBUTE_WILLPOWER, 5);

    // Monks
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_MONKS, CHARACTER_ATTRIBUTE_SKILL_DODGE, 3, &Player::skillDodge);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_MONKS, CHARACTER_ATTRIBUTE_SKILL_UNARMED, 3, &Player::skillUnarmed);

    // Thieves
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_THIEVES, CHARACTER_ATTRIBUTE_SKILL_STEALING, 3, &Player::skillStealing);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_THIEVES, CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 3, &Player::skillDisarmTrap);

    // of Identifying
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_IDENTIFYING, CHARACTER_ATTRIBUTE_SKILL_ITEM_ID, 3, &Player::skillItemId);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_IDENTIFYING, CHARACTER_ATTRIBUTE_SKILL_MONSTER_ID, 3, &Player::skillMonsterId);

    // Assassins
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_ASSASINS, CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 2, &Player::skillDisarmTrap);

    // Barbarians
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_BARBARIANS, CHARACTER_ATTRIBUTE_AC_BONUS, 5);

    // of the Storm
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_STORM, CHARACTER_ATTRIBUTE_RESIST_AIR, 20);

    // of the Ocean
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_OCEAN, CHARACTER_ATTRIBUTE_RESIST_WATER, 10);
    AddToMap(specialBonusMap, ITEM_ENCHANTMENT_OF_OCEAN, CHARACTER_ATTRIBUTE_SKILL_ALCHEMY, 2, &Player::skillAlchemy);
}

// TODO: where is it used?
void ItemGen::PopulateRegularBonusMap() {
    // of Might
    AddToMap(regularBonusMap, 1, CHARACTER_ATTRIBUTE_STRENGTH);

    // of Thought
    AddToMap(regularBonusMap, 2, CHARACTER_ATTRIBUTE_INTELLIGENCE);

    // of Charm
    AddToMap(regularBonusMap, 3, CHARACTER_ATTRIBUTE_WILLPOWER);

    // of Vigor
    AddToMap(regularBonusMap, 4, CHARACTER_ATTRIBUTE_ENDURANCE);

    // of Precision
    AddToMap(regularBonusMap, 5, CHARACTER_ATTRIBUTE_ACCURACY);

    // of Speed
    AddToMap(regularBonusMap, 6, CHARACTER_ATTRIBUTE_SPEED);

    // of Luck
    AddToMap(regularBonusMap, 7, CHARACTER_ATTRIBUTE_LUCK);

    // of Health
    AddToMap(regularBonusMap, 8, CHARACTER_ATTRIBUTE_HEALTH);

    // of Magic
    AddToMap(regularBonusMap, 9, CHARACTER_ATTRIBUTE_MANA);

    // of Defense
    AddToMap(regularBonusMap, 10, CHARACTER_ATTRIBUTE_AC_BONUS);

    // of Fire Resistance
    AddToMap(regularBonusMap, 11, CHARACTER_ATTRIBUTE_RESIST_FIRE);

    // of Air Resistance
    AddToMap(regularBonusMap, 12, CHARACTER_ATTRIBUTE_RESIST_AIR);

    // of Water Resistance
    AddToMap(regularBonusMap, 13, CHARACTER_ATTRIBUTE_RESIST_WATER);

    // of Earth Resistance
    AddToMap(regularBonusMap, 14, CHARACTER_ATTRIBUTE_RESIST_EARTH);

    // of Mind Resistance
    AddToMap(regularBonusMap, 15, CHARACTER_ATTRIBUTE_RESIST_MIND);

    // of Body Resistance
    AddToMap(regularBonusMap, 16, CHARACTER_ATTRIBUTE_RESIST_BODY);

    // of Alchemy
    AddToMap(regularBonusMap, 17, CHARACTER_ATTRIBUTE_SKILL_ALCHEMY);

    // of Stealing
    AddToMap(regularBonusMap, 18, CHARACTER_ATTRIBUTE_SKILL_STEALING);

    // of Disarming
    AddToMap(regularBonusMap, 19, CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM);

    // of Items
    AddToMap(regularBonusMap, 20, CHARACTER_ATTRIBUTE_SKILL_ITEM_ID);

    // of Monsters
    AddToMap(regularBonusMap, 21, CHARACTER_ATTRIBUTE_SKILL_MONSTER_ID);

    // of Arms
    AddToMap(regularBonusMap, 22, CHARACTER_ATTRIBUTE_SKILL_ARMSMASTER);

    // of Dodging
    AddToMap(regularBonusMap, 23, CHARACTER_ATTRIBUTE_SKILL_DODGE);

    // of the Fist
    AddToMap(regularBonusMap, 24, CHARACTER_ATTRIBUTE_SKILL_UNARMED);
}

void ItemGen::PopulateArtifactBonusMap() {
    // Puck
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_PUCK, CHARACTER_ATTRIBUTE_SPEED, 40);

    // Iron Feather
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_IRON_FEATHER, CHARACTER_ATTRIBUTE_STRENGTH, 40);

    // Wallace
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_WALLACE, CHARACTER_ATTRIBUTE_SKILL_ARMSMASTER, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_WALLACE, CHARACTER_ATTRIBUTE_WILLPOWER, 40);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_CORSAIR, CHARACTER_ATTRIBUTE_LUCK, 40);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_CORSAIR, CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 5);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_CORSAIR, CHARACTER_ATTRIBUTE_SKILL_STEALING, 5);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_GOVERNORS_ARMOR, CHARACTER_ATTRIBUTE_STRENGTH, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_GOVERNORS_ARMOR, CHARACTER_ATTRIBUTE_INTELLIGENCE, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_GOVERNORS_ARMOR, CHARACTER_ATTRIBUTE_WILLPOWER, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_GOVERNORS_ARMOR, CHARACTER_ATTRIBUTE_ENDURANCE, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_GOVERNORS_ARMOR, CHARACTER_ATTRIBUTE_ACCURACY, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_GOVERNORS_ARMOR, CHARACTER_ATTRIBUTE_SPEED, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_GOVERNORS_ARMOR, CHARACTER_ATTRIBUTE_LUCK, 10);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_YORUBA, CHARACTER_ATTRIBUTE_ENDURANCE, 25);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_SPLITTER, CHARACTER_ATTRIBUTE_RESIST_FIRE, 50);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_ULLYSES, CHARACTER_ATTRIBUTE_ACCURACY, 50);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_HANDS_OF_THE_MASTER, CHARACTER_ATTRIBUTE_SKILL_DODGE, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_HANDS_OF_THE_MASTER, CHARACTER_ATTRIBUTE_SKILL_UNARMED, 10);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_SEVEN_LEAGUE_BOOTS, CHARACTER_ATTRIBUTE_SPEED, 40);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_SEVEN_LEAGUE_BOOTS, CHARACTER_ATTRIBUTE_SKILL_WATER, 0, &Player::skillWater);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_RULERS_RING, CHARACTER_ATTRIBUTE_SKILL_MIND, 0, &Player::skillMind);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_RULERS_RING, CHARACTER_ATTRIBUTE_SKILL_DARK, 0, &Player::skillDark);

    AddToMap(artifactBonusMap, ITEM_RELIC_MASH, CHARACTER_ATTRIBUTE_STRENGTH, 150);
    AddToMap(artifactBonusMap, ITEM_RELIC_MASH, CHARACTER_ATTRIBUTE_INTELLIGENCE, -40);
    AddToMap(artifactBonusMap, ITEM_RELIC_MASH, CHARACTER_ATTRIBUTE_WILLPOWER, -40);
    AddToMap(artifactBonusMap, ITEM_RELIC_MASH, CHARACTER_ATTRIBUTE_SPEED, -40);

    AddToMap(artifactBonusMap, ITEM_RELIC_ETHRICS_STAFF, CHARACTER_ATTRIBUTE_SKILL_DARK, 0, &Player::skillDark);
    AddToMap(artifactBonusMap, ITEM_RELIC_ETHRICS_STAFF, CHARACTER_ATTRIBUTE_SKILL_MEDITATION, 15);

    AddToMap(artifactBonusMap, ITEM_RELIC_HARECKS_LEATHER, CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 5);
    AddToMap(artifactBonusMap, ITEM_RELIC_HARECKS_LEATHER, CHARACTER_ATTRIBUTE_SKILL_STEALING, 5);
    AddToMap(artifactBonusMap, ITEM_RELIC_HARECKS_LEATHER, CHARACTER_ATTRIBUTE_LUCK, 50);
    AddToMap(artifactBonusMap, ITEM_RELIC_HARECKS_LEATHER, CHARACTER_ATTRIBUTE_RESIST_FIRE, -10);
    AddToMap(artifactBonusMap, ITEM_RELIC_HARECKS_LEATHER, CHARACTER_ATTRIBUTE_RESIST_WATER, -10);
    AddToMap(artifactBonusMap, ITEM_RELIC_HARECKS_LEATHER, CHARACTER_ATTRIBUTE_RESIST_AIR, -10);
    AddToMap(artifactBonusMap, ITEM_RELIC_HARECKS_LEATHER, CHARACTER_ATTRIBUTE_RESIST_EARTH, -10);
    AddToMap(artifactBonusMap, ITEM_RELIC_HARECKS_LEATHER, CHARACTER_ATTRIBUTE_RESIST_MIND, -10);
    AddToMap(artifactBonusMap, ITEM_RELIC_HARECKS_LEATHER, CHARACTER_ATTRIBUTE_RESIST_BODY, -10);

    AddToMap(artifactBonusMap, ITEM_RELIC_OLD_NICK, CHARACTER_ATTRIBUTE_SKILL_TRAP_DISARM, 5);

    AddToMap(artifactBonusMap, ITEM_RELIC_AMUCK, CHARACTER_ATTRIBUTE_STRENGTH, 100);
    AddToMap(artifactBonusMap, ITEM_RELIC_AMUCK, CHARACTER_ATTRIBUTE_ENDURANCE, 100);
    AddToMap(artifactBonusMap, ITEM_RELIC_AMUCK, CHARACTER_ATTRIBUTE_AC_BONUS, -15);

    AddToMap(artifactBonusMap, ITEM_RELIC_GLORY_SHIELD, CHARACTER_ATTRIBUTE_SKILL_SPIRIT, 0, &Player::skillSpirit);
    AddToMap(artifactBonusMap, ITEM_RELIC_GLORY_SHIELD, CHARACTER_ATTRIBUTE_SKILL_SHIELD, 5);
    AddToMap(artifactBonusMap, ITEM_RELIC_GLORY_SHIELD, CHARACTER_ATTRIBUTE_RESIST_MIND, -10);
    AddToMap(artifactBonusMap, ITEM_RELIC_GLORY_SHIELD, CHARACTER_ATTRIBUTE_RESIST_BODY, -10);

    AddToMap(artifactBonusMap, ITEM_RELIC_KELEBRIM, CHARACTER_ATTRIBUTE_ENDURANCE, 50);
    AddToMap(artifactBonusMap, ITEM_RELIC_KELEBRIM, CHARACTER_ATTRIBUTE_RESIST_EARTH, -30);

    AddToMap(artifactBonusMap, ITEM_RELIC_TALEDONS_HELM, CHARACTER_ATTRIBUTE_SKILL_LIGHT, 0, &Player::skillLight);
    AddToMap(artifactBonusMap, ITEM_RELIC_TALEDONS_HELM, CHARACTER_ATTRIBUTE_WILLPOWER, 15);
    AddToMap(artifactBonusMap, ITEM_RELIC_TALEDONS_HELM, CHARACTER_ATTRIBUTE_STRENGTH, 15);
    AddToMap(artifactBonusMap, ITEM_RELIC_TALEDONS_HELM, CHARACTER_ATTRIBUTE_LUCK, -40);

    AddToMap(artifactBonusMap, ITEM_RELIC_SCHOLARS_CAP, CHARACTER_ATTRIBUTE_SKILL_LEARNING, +15);
    AddToMap(artifactBonusMap, ITEM_RELIC_SCHOLARS_CAP, CHARACTER_ATTRIBUTE_ENDURANCE, -50);

    AddToMap(artifactBonusMap, ITEM_RELIC_PHYNAXIAN_CROWN, CHARACTER_ATTRIBUTE_SKILL_FIRE, 0, &Player::skillFire);
    AddToMap(artifactBonusMap, ITEM_RELIC_PHYNAXIAN_CROWN, CHARACTER_ATTRIBUTE_RESIST_WATER, +50);
    AddToMap(artifactBonusMap, ITEM_RELIC_PHYNAXIAN_CROWN, CHARACTER_ATTRIBUTE_WILLPOWER, 30);
    AddToMap(artifactBonusMap, ITEM_RELIC_PHYNAXIAN_CROWN, CHARACTER_ATTRIBUTE_AC_BONUS, -20);

    AddToMap(artifactBonusMap, ITEM_RELIC_TITANS_BELT, CHARACTER_ATTRIBUTE_STRENGTH, 75);
    AddToMap(artifactBonusMap, ITEM_RELIC_TITANS_BELT, CHARACTER_ATTRIBUTE_SPEED, -40);

    AddToMap(artifactBonusMap, ITEM_RELIC_TWILIGHT, CHARACTER_ATTRIBUTE_SPEED, 50);
    AddToMap(artifactBonusMap, ITEM_RELIC_TWILIGHT, CHARACTER_ATTRIBUTE_LUCK, 50);
    AddToMap(artifactBonusMap, ITEM_RELIC_TWILIGHT, CHARACTER_ATTRIBUTE_RESIST_FIRE, -15);
    AddToMap(artifactBonusMap, ITEM_RELIC_TWILIGHT, CHARACTER_ATTRIBUTE_RESIST_WATER, -15);
    AddToMap(artifactBonusMap, ITEM_RELIC_TWILIGHT, CHARACTER_ATTRIBUTE_RESIST_AIR, -15);
    AddToMap(artifactBonusMap, ITEM_RELIC_TWILIGHT, CHARACTER_ATTRIBUTE_RESIST_EARTH, -15);
    AddToMap(artifactBonusMap, ITEM_RELIC_TWILIGHT, CHARACTER_ATTRIBUTE_RESIST_MIND, -15);
    AddToMap(artifactBonusMap, ITEM_RELIC_TWILIGHT, CHARACTER_ATTRIBUTE_RESIST_BODY, -15);

    AddToMap(artifactBonusMap, ITEM_RELIC_ANIA_SELVING, CHARACTER_ATTRIBUTE_ACCURACY, 150);
    AddToMap(artifactBonusMap, ITEM_RELIC_ANIA_SELVING, CHARACTER_ATTRIBUTE_SKILL_BOW, 5);
    AddToMap(artifactBonusMap, ITEM_RELIC_ANIA_SELVING, CHARACTER_ATTRIBUTE_AC_BONUS, -25);

    AddToMap(artifactBonusMap, ITEM_RELIC_JUSTICE, CHARACTER_ATTRIBUTE_SKILL_MIND, 0, &Player::skillMind);
    AddToMap(artifactBonusMap, ITEM_RELIC_JUSTICE, CHARACTER_ATTRIBUTE_SKILL_BODY, 0, &Player::skillBody);
    AddToMap(artifactBonusMap, ITEM_RELIC_JUSTICE, CHARACTER_ATTRIBUTE_SPEED, -40);

    AddToMap(artifactBonusMap, ITEM_RELIC_MEKORIGS_HAMMER, CHARACTER_ATTRIBUTE_SKILL_SPIRIT, 0, &Player::skillSpirit);
    AddToMap(artifactBonusMap, ITEM_RELIC_MEKORIGS_HAMMER, CHARACTER_ATTRIBUTE_STRENGTH, 75);
    AddToMap(artifactBonusMap, ITEM_RELIC_MEKORIGS_HAMMER, CHARACTER_ATTRIBUTE_RESIST_AIR, -50);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_HERMES_SANDALS, CHARACTER_ATTRIBUTE_SPEED, 100);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_HERMES_SANDALS, CHARACTER_ATTRIBUTE_ACCURACY, 50);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_HERMES_SANDALS, CHARACTER_ATTRIBUTE_RESIST_AIR, 50);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_CLOAK_OF_THE_SHEEP, CHARACTER_ATTRIBUTE_WILLPOWER, -20);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_CLOAK_OF_THE_SHEEP, CHARACTER_ATTRIBUTE_INTELLIGENCE, -20);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_MINDS_EYE, CHARACTER_ATTRIBUTE_WILLPOWER, 15);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_MINDS_EYE, CHARACTER_ATTRIBUTE_INTELLIGENCE, 15);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_ELVEN_CHAINMAIL, CHARACTER_ATTRIBUTE_SPEED, 15);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_ELVEN_CHAINMAIL, CHARACTER_ATTRIBUTE_ACCURACY, 15);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_FORGE_GAUNTLETS, CHARACTER_ATTRIBUTE_STRENGTH, 15);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_FORGE_GAUNTLETS, CHARACTER_ATTRIBUTE_ENDURANCE, 15);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_FORGE_GAUNTLETS, CHARACTER_ATTRIBUTE_RESIST_FIRE, 30);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_HEROS_BELT, CHARACTER_ATTRIBUTE_STRENGTH, 15);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_HEROS_BELT, CHARACTER_ATTRIBUTE_SKILL_ARMSMASTER, 5);

    AddToMap(artifactBonusMap, ITEM_ARTIFACT_LADYS_ESCORT, CHARACTER_ATTRIBUTE_RESIST_FIRE, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_LADYS_ESCORT, CHARACTER_ATTRIBUTE_RESIST_AIR, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_LADYS_ESCORT, CHARACTER_ATTRIBUTE_RESIST_WATER, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_LADYS_ESCORT, CHARACTER_ATTRIBUTE_RESIST_EARTH, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_LADYS_ESCORT, CHARACTER_ATTRIBUTE_RESIST_MIND, 10);
    AddToMap(artifactBonusMap, ITEM_ARTIFACT_LADYS_ESCORT, CHARACTER_ATTRIBUTE_RESIST_BODY, 10);
}

void ItemGen::GetItemBonusSpecialEnchantment(Player* owner,
                                             CHARACTER_ATTRIBUTE_TYPE attrToGet,
                                             int* additiveBonus,
                                             int* halfSkillBonus) {
    auto pos = specialBonusMap.find(this->special_enchantment);
    if (pos == specialBonusMap.end())
        return;

    auto subpos = pos->second.find(attrToGet);
    if (subpos == pos->second.end())
        return;

    const CEnchantment &currBonus = subpos->second;
    if (currBonus.statPtr != NULL) {
        if (currBonus.statBonus == 0) {
            *halfSkillBonus = owner->*currBonus.statPtr / 2;
        } else {
            if (*additiveBonus < currBonus.statBonus) {
                *additiveBonus = currBonus.statBonus;
            }
        }
    } else {
        *additiveBonus += currBonus.statBonus;
    }
}

void ItemGen::GetItemBonusArtifact(Player* owner,
                                   CHARACTER_ATTRIBUTE_TYPE attrToGet,
                                   int* bonusSum) {
    auto pos = artifactBonusMap.find(this->uItemID);
    if (pos == artifactBonusMap.end())
        return;

    auto subpos = pos->second.find(attrToGet);
    if (subpos == pos->second.end())
        return;

    const CEnchantment &currBonus = subpos->second;
    if (currBonus.statPtr != NULL) {
        *bonusSum = owner->*currBonus.statPtr / 2;
    } else {
        *bonusSum += currBonus.statBonus;
    }
}

bool ItemGen::IsRegularEnchanmentForAttribute(CHARACTER_ATTRIBUTE_TYPE attrToGet) {
    auto pos = specialBonusMap.find(this->uEnchantmentType);
    if (pos == specialBonusMap.end())
        return false;

    return pos->second.find(attrToGet) != pos->second.end();
}

ITEM_EQUIP_TYPE ItemGen::GetItemEquipType() {
    // to avoid nzi - is this safe??
    if (this->uItemID == ITEM_NULL)
        return EQUIP_NONE;
    else
        return pItemTable->pItems[this->uItemID].uEquipType;
}

PLAYER_SKILL_TYPE ItemGen::GetPlayerSkillType() {
    PLAYER_SKILL_TYPE skl = pItemTable->pItems[this->uItemID].uSkillType;
    if (skl == PLAYER_SKILL_CLUB) {
        // club skill not used but some items load it
        skl = PLAYER_SKILL_MACE;
    }
    return skl;
}

char* ItemGen::GetIconName() {
    return pItemTable->pItems[this->uItemID].pIconName;
}

uint8_t ItemGen::GetDamageDice() {
    return pItemTable->pItems[this->uItemID].uDamageDice;
}

uint8_t ItemGen::GetDamageRoll() {
    return pItemTable->pItems[this->uItemID].uDamageRoll;
}

uint8_t ItemGen::GetDamageMod() {
    return pItemTable->pItems[this->uItemID].uDamageMod;
}

//----- (0043C91D) --------------------------------------------------------
int GetItemTextureFilename(char* pOut, ITEM_TYPE item_id, int index,
                           int shoulder) {
    int result;  // eax@2
    ITEM_EQUIP_TYPE pEquipType;

    // TODO(captainurist): what are we even doing here?
    result = 0;  // BUG   fn is void
    pEquipType = pItemTable->pItems[item_id].uEquipType;
    if(item_id > ITEM_ARTIFACT_PUCK) {
        switch (item_id) {
            case ITEM_RELIC_HARECKS_LEATHER:
                if (byte_5111F6_OwnedArtifacts[2] != 0)
                    item_id = ITEM_POTION_STONESKIN;
                break;
            case ITEM_ARTIFACT_YORUBA:
                if (byte_5111F6_OwnedArtifacts[1] != 0)
                    item_id = ITEM_POTION_HARDEN_ITEM;
                break;
            case ITEM_ARTIFACT_GOVERNORS_ARMOR:
                if (byte_5111F6_OwnedArtifacts[0] != 0)
                    item_id = ITEM_POTION_WATER_BREATHING;
                break;
            case ITEM_ARTIFACT_ELVEN_CHAINMAIL:
                if (byte_5111F6_OwnedArtifacts[16] != 0)
                    item_id = ITEM_FINE_CHAIN_MAIL;
                break;
            case ITEM_ARTIFACT_SEVEN_LEAGUE_BOOTS:
                if (byte_5111F6_OwnedArtifacts[3] != 0)
                    item_id = ITEM_SCROLL_FEATHER_FALL;
                break;
            case ITEM_RELIC_TALEDONS_HELM:
                if (byte_5111F6_OwnedArtifacts[4] != 0)
                    item_id = ITEM_POTION_CURE_INSANITY;
                break;
            case ITEM_RELIC_SCHOLARS_CAP:
                if (byte_5111F6_OwnedArtifacts[5] != 0)
                    item_id = ITEM_POTION_MIGHT_BOOST;
                break;
            case ITEM_RELIC_PHYNAXIAN_CROWN:
                if (byte_5111F6_OwnedArtifacts[6] != 0)
                    item_id = ITEM_POTION_INTELLECT_BOOST;
                break;
            case ITEM_ARTIFACT_MINDS_EYE:
                if (byte_5111F6_OwnedArtifacts[7] != 0)
                    item_id = ITEM_MOGRED_HELM;
                break;
            case ITEM_RARE_SHADOWS_MASK:
                if (byte_5111F6_OwnedArtifacts[8] != 0)
                    item_id = ITEM_SCROLL_DETECT_LIFE;
                break;
            case ITEM_RELIC_TITANS_BELT:
                if (byte_5111F6_OwnedArtifacts[9] != 0)
                    item_id = ITEM_SCROLL_WATER_RESISTANCE;
                break;
            case ITEM_ARTIFACT_HEROS_BELT:
                if (byte_5111F6_OwnedArtifacts[10] != 0)
                    item_id = ITEM_GILDED_BELT;
                break;
            case ITEM_RELIC_TWILIGHT:
                if (byte_5111F6_OwnedArtifacts[11] != 0)
                    item_id = ITEM_SCROLL_ICE_BOLT;
                break;
            case ITEM_ARTIFACT_CLOAK_OF_THE_SHEEP:
                if (byte_5111F6_OwnedArtifacts[12] != 0)
                    item_id = ITEM_SCROLL_TOWN_PORTAL;
                break;
            case ITEM_RARE_SUN_CLOAK:
                if (byte_5111F6_OwnedArtifacts[13] != 0)
                    item_id = ITEM_SCROLL_TURN_UNDEAD;
                break;
            case ITEM_RARE_MOON_CLOAK:
                if (byte_5111F6_OwnedArtifacts[14] != 0)
                    item_id = ITEM_SCROLL_REMOVE_CURSE;
                break;
            case ITEM_RARE_VAMPIRES_CAPE:
                if (byte_5111F6_OwnedArtifacts[15] != 0)
                    item_id = ITEM_SCROLL_HEROISM;
                break;
            default:
                return 0;
        }
    }

    switch (pEquipType) {
        case EQUIP_ARMOUR:
            if (!shoulder)
                return sprintf(pOut, "item%3.3dv%d", item_id, index);
            else if (shoulder == 1)
                return sprintf(pOut, "item%3.3dv%da1", item_id, index);
            else if (shoulder == 2)
                return sprintf(pOut, "item%3.3dv%da2", item_id, index);
            break;
        case EQUIP_CLOAK:
            if (!shoulder)
                return sprintf(pOut, "item%3.3dv%d", item_id, index);
            else
                return sprintf(pOut, "item%3.3dv%da1", item_id, index);
        default:
            return sprintf(pOut, "item%3.3dv%d", item_id, index);
    }

    // TODO(captainurist): also makes no sense.
    result = std::to_underlying(item_id) - std::to_underlying(ITEM_ARTIFACT_GOVERNORS_ARMOR);
    return result;
}

//----- (004BDAAF) --------------------------------------------------------
bool ItemGen::MerchandiseTest(int _2da_idx) {
    bool test;

    // TODO(captainurist): move these checks into functions in ItemEnums.h?
    if ((p2DEvents[_2da_idx - 1].uType != BuildingType_AlchemistShop || !IsRecipe(this->uItemID)) &&
        (this->uItemID >= ITEM_QUEST_HEART_OF_THE_WOOD || this->uItemID >= ITEM_ARTIFACT_HERMES_SANDALS && this->uItemID <= ITEM_599) ||
        this->IsStolen())
        return false;

    switch (p2DEvents[_2da_idx - 1].uType) {
        case BuildingType_WeaponShop: {
            test = this->GetItemEquipType() <= EQUIP_BOW;
            break;
        }
        case BuildingType_ArmorShop: {
            test = this->GetItemEquipType() >= EQUIP_ARMOUR &&
                   this->GetItemEquipType() <= EQUIP_BOOTS;
            break;
        }
        case BuildingType_MagicShop: {
            // TODO(captainurist): items with EQUIP_ANY equip type don't exist (in mm7, at least)
            test = this->GetPlayerSkillType() == PLAYER_SKILL_MISC ||
                   this->GetItemEquipType() == EQIUP_ANY;
            break;
        }
        case BuildingType_AlchemistShop: {
            test = this->GetItemEquipType() == EQUIP_REAGENT ||
                   this->GetItemEquipType() == EQUIP_POTION ||
                   (this->GetItemEquipType() > EQUIP_POTION &&
                    (this->GetItemEquipType() == EQUIP_MESSAGE_SCROLL &&
                      this->uItemID >= ITEM_MESSAGE_REJUVENATION_RECIPE) &&
                    this->uItemID != ITEM_MESSAGE_BODY_RESISTANCE_RECIPE);
            // TODO(captainurist): probably should be EQUIP_REAGENT || EQUIP_POTION || (EQUIP_MESSAGE_SCROLL && IsRecipe)?
            // The last check for != ITEM_MESSAGE_BODY_RESISTANCE_RECIPE makes no sense.
            break;
        }
        default: {
            test = false;
            break;
        }
    }
    return test;
}
