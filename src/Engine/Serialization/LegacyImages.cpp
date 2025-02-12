#include "LegacyImages.h"

#include <algorithm>
#include <type_traits>
#include <string>
#include <bitset>

#include "Engine/Engine.h"
#include "Engine/Graphics/Indoor.h"
#include "Engine/Graphics/Level/Decoration.h"
#include "Engine/Graphics/Outdoor.h"
#include "Engine/Graphics/Overlays.h"
#include "Engine/Graphics/Sprites.h"
#include "Engine/Graphics/TextureFrameTable.h"
#include "Engine/Objects/Actor.h"
#include "Engine/Objects/NPC.h"
#include "Engine/Objects/ObjectList.h"
#include "Engine/Objects/SpriteObject.h"
#include "Engine/Party.h"
#include "Engine/SaveLoad.h"
#include "Engine/Tables/IconFrameTable.h"
#include "Engine/Tables/PlayerFrameTable.h"
#include "Engine/Tables/TileFrameTable.h"
#include "Engine/Time.h"

#include "Media/Audio/SoundInfo.h"

#include "GUI/GUIFont.h"

#include "Utility/Memory/MemSet.h"
#include "Utility/IndexedBitset.h"

#include "CommonImages.h"

static void serialize(const GameTime &src, int64_t *dst) {
    *dst = src.value;
}

static void deserialize(int64_t src, GameTime *dst) {
    dst->value = src;
}

void deserialize(const SpriteFrame_MM7 &src, SpriteFrame *dst) {
    deserialize(src.iconName, &dst->icon_name);
    dst->icon_name = toLower(dst->icon_name);

    deserialize(src.textureName, &dst->texture_name);
    dst->texture_name = toLower(dst->texture_name);

    for (unsigned int i = 0; i < 8; ++i)
        dst->hw_sprites[i] = nullptr;

    dst->scale = src.scale / 65536.0;
    dst->uFlags = src.flags;

    dst->uGlowRadius = src.glowRadius;
    dst->uPaletteID = src.paletteId;
    dst->ResetPaletteIndex(src.paletteIndex);
    dst->uAnimTime = src.animTime;
    dst->uAnimLength = src.animLength;
}

void deserialize(const BLVFace_MM7 &src, BLVFace *dst) {
    dst->facePlane = src.facePlane;
    dst->zCalc.init(dst->facePlane);
    dst->uAttributes = static_cast<FaceAttributes>(src.attributes);
    dst->pVertexIDs = nullptr;
    dst->pXInterceptDisplacements = nullptr;
    dst->pYInterceptDisplacements = nullptr;
    dst->pZInterceptDisplacements = nullptr;
    dst->pVertexUIDs = nullptr;
    dst->pVertexVIDs = nullptr;
    dst->uFaceExtraID = src.faceExtraId;
    dst->resource = nullptr;
    dst->uSectorID = src.sectorId;
    dst->uBackSectorID = src.backSectorId;
    dst->pBounding = src.bounding;
    dst->uPolygonType = static_cast<PolygonType>(src.polygonType);
    dst->uNumVertices = src.numVertices;
}

void deserialize(const TileDesc_MM7 &src, TileDesc *dst) {
    deserialize(src.tileName, &dst->name);
    dst->name = toLower(dst->name);

    if (istarts_with(dst->name, "wtrdr"))
        dst->name.insert(0, "h");  // mm7 uses hd water tiles with legacy names

    dst->uTileID = src.tileId;
    dst->tileset = static_cast<Tileset>(src.tileSet);
    dst->uSection = src.section;
    dst->uAttributes = src.attributes;
}

void deserialize(const TextureFrame_MM7 &src, TextureFrame *dst) {
    deserialize(src.textureName, &dst->name);
    dst->name = toLower(dst->name);

    dst->uAnimLength = src.animLength;
    dst->uAnimTime = src.animTime;
    dst->uFlags = src.flags;
}

void serialize(const Timer &src, Timer_MM7 *dst) {
    memzero(dst);

    dst->ready = src.bReady;
    dst->paused = src.bPaused;
    dst->tackGameTime = src.bTackGameTime;
    dst->startTime = src.uStartTime;
    dst->stopTime = src.uStopTime;
    dst->gameTimeStart = src.uGameTimeStart;
    dst->field_18 = src.field_18;
    dst->timeElapsed = src.uTimeElapsed;
    dst->dtFixpoint = src.dt_fixpoint;
    dst->totalGameTimeElapsed = src.uTotalTimeElapsed;
}

void deserialize(const Timer_MM7 &src, Timer *dst) {
    dst->bReady = src.ready;
    dst->bPaused = src.paused;
    dst->bTackGameTime = src.tackGameTime;
    dst->uStartTime = src.startTime;
    dst->uStopTime = src.stopTime;
    dst->uGameTimeStart = src.gameTimeStart;
    dst->field_18 = src.field_18;
    dst->uTimeElapsed = src.timeElapsed;
    dst->dt_fixpoint = src.dtFixpoint;
    dst->uTotalTimeElapsed = src.totalGameTimeElapsed;
}

void serialize(const NPCData &src, NPCData_MM7 *dst) {
    memzero(dst);

    // dst->pName = src.pName;
    dst->name = !src.pName.empty();
    dst->portraitId = src.uPortraitID;
    dst->flags = std::to_underlying(src.uFlags);
    dst->fame = src.fame;
    dst->rep = src.rep;
    dst->location2d = src.Location2D;
    dst->profession = std::to_underlying(src.profession);
    dst->greet = src.greet;
    dst->joins = src.is_joinable;
    dst->field_24 = src.field_24;
    dst->evt_A = src.dialogue_1_evt_id;
    dst->evt_B = src.dialogue_2_evt_id;
    dst->evt_C = src.dialogue_3_evt_id;
    dst->evt_D = src.dialogue_4_evt_id;
    dst->evt_E = src.dialogue_5_evt_id;
    dst->evt_F = src.dialogue_6_evt_id;
    dst->sex = src.uSex;
    dst->hasUsedAbility = src.bHasUsedTheAbility;
    dst->newsTopic = src.news_topic;
}

void deserialize(const NPCData_MM7 &src, NPCData *dst) {
    // dst->pName = src.pName;
    dst->pName = src.name ? "Dummy" : "";
    dst->uPortraitID = src.portraitId;
    dst->uFlags = NpcFlags(src.flags);
    dst->fame = src.fame;
    dst->rep = src.rep;
    dst->Location2D = src.location2d;
    dst->profession = static_cast<NPCProf>(src.profession);
    dst->greet = src.greet;
    dst->is_joinable = src.joins;
    dst->field_24 = src.field_24;
    dst->dialogue_1_evt_id = src.evt_A;
    dst->dialogue_2_evt_id = src.evt_B;
    dst->dialogue_3_evt_id = src.evt_C;
    dst->dialogue_4_evt_id = src.evt_D;
    dst->dialogue_5_evt_id = src.evt_E;
    dst->dialogue_6_evt_id = src.evt_F;
    dst->uSex = src.sex;
    dst->bHasUsedTheAbility = src.hasUsedAbility;
    dst->news_topic = src.newsTopic;
}

void serialize(const ActiveOverlay &src, ActiveOverlay_MM7 *dst) {
    memzero(dst);

    dst->field_0 = src.field_0;
    dst->indexToOverlayList = src.indexToOverlayList;
    dst->spriteFrameTime = src.spriteFrameTime;
    dst->animLength = src.animLength;
    dst->screenSpaceX = src.screenSpaceX;
    dst->screenSpaceY = src.screenSpaceY;
    dst->pid = src.pid;
    dst->projSize = src.projSize;
    dst->fpDamageMod = src.fpDamageMod;
}

void deserialize(const ActiveOverlay_MM7 &src, ActiveOverlay *dst) {
    memzero(dst);

    dst->field_0 = src.field_0;
    dst->indexToOverlayList = src.indexToOverlayList;
    dst->spriteFrameTime = src.spriteFrameTime;
    dst->animLength = src.animLength;
    dst->screenSpaceX = src.screenSpaceX;
    dst->screenSpaceY = src.screenSpaceY;
    dst->pid = src.pid;
    dst->projSize = src.projSize;
    dst->fpDamageMod = src.fpDamageMod;
}

void serialize(const ActiveOverlayList &src, ActiveOverlayList_MM7 *dst) {
    memzero(dst);

    dst->redraw = true;
    dst->field_3E8 = src.field_3E8;
    serialize(src.pOverlays, &dst->overlays);
}

void deserialize(const ActiveOverlayList_MM7 &src, ActiveOverlayList *dst) {
    dst->field_3E8 = src.field_3E8;
    deserialize(src.overlays, &dst->pOverlays);
}

void serialize(const SpellBuff &src, SpellBuff_MM7 *dst) {
    memzero(dst);

    dst->expireTime = src.expireTime.value;
    dst->power = src.power;
    dst->skillMastery = std::to_underlying(src.skillMastery);
    dst->overlayId = src.overlayID;
    dst->caster = src.caster;
    dst->flags = src.isGMBuff;
}

void deserialize(const SpellBuff_MM7 &src, SpellBuff *dst) {
    dst->expireTime.value = src.expireTime;
    dst->power = src.power;
    dst->skillMastery = static_cast<PLAYER_SKILL_MASTERY>(src.skillMastery);
    dst->overlayID = src.overlayId;
    dst->caster = src.caster;
    dst->isGMBuff = src.flags;
}

void serialize(const ItemGen &src, ItemGen_MM7 *dst) {
    memzero(dst);

    dst->itemID = std::to_underlying(src.uItemID);
    dst->enchantmentType = src.uEnchantmentType;
    dst->enchantmentStrength = src.m_enchantmentStrength;
    dst->specialEnchantment = src.special_enchantment;
    dst->numCharges = src.uNumCharges;
    dst->attributes = std::to_underlying(src.uAttributes);
    dst->bodyAnchor = std::to_underlying(src.uBodyAnchor);
    dst->maxCharges = src.uMaxCharges;
    dst->holderPlayer = src.uHolderPlayer;
    dst->placedInChest = src.placedInChest;
    dst->expireTime = src.uExpireTime.value;
}

void deserialize(const ItemGen_MM7 &src, ItemGen *dst) {
    dst->uItemID = static_cast<ITEM_TYPE>(src.itemID);
    dst->uEnchantmentType = src.enchantmentType;
    dst->m_enchantmentStrength = src.enchantmentStrength;
    dst->special_enchantment = static_cast<ITEM_ENCHANTMENT>(src.specialEnchantment);
    dst->uNumCharges = src.numCharges;
    dst->uAttributes = ItemFlags(src.attributes);
    dst->uBodyAnchor = static_cast<ITEM_SLOT>(src.bodyAnchor);
    dst->uMaxCharges = src.maxCharges;
    dst->uHolderPlayer = src.holderPlayer;
    dst->placedInChest = src.placedInChest;
    dst->uExpireTime.value = src.expireTime;
}

void serialize(const Party &src, Party_MM7 *dst) {
    memzero(dst);

    dst->field_0 = src.field_0_set25_unused;
    dst->partyHeight = src.uPartyHeight;
    dst->defaultPartyHeight = src.uDefaultPartyHeight;
    dst->eyeLevel = src.sEyelevel;
    dst->defaultEyeLevel = src.uDefaultEyelevel;
    dst->radius = src.radius;
    dst->yawGranularity = src._yawGranularity;
    dst->walkSpeed = src.uWalkSpeed;
    dst->yawRotationSpeed = src._yawRotationSpeed;
    dst->jumpStrength = src.jump_strength;
    dst->field_28 = src.field_28_set0_unused;
    dst->timePlayed = src.playing_time.value;
    dst->lastRegenerationTime = src.last_regenerated.value;

    serialize(src.PartyTimes.bountyHuntNextGenTime, &dst->partyTimes.bountyHuntingNextGenerationTime);
    dst->partyTimes.bountyHuntingNextGenerationTimeUnused.fill(0);

    // Initially was one array but was splitted in two to simplify access with first element as zero
    // because it is corresponding to invalid house ID
    dst->partyTimes.shopsNextGenerationTime0 = 0;
    serialize(src.PartyTimes.shopNextRefreshTime, &dst->partyTimes.shopsNextGenerationTime);
    serialize(src.PartyTimes.guildNextRefreshTime, &dst->partyTimes.guildsNextGenerationTime);

    dst->partyTimes.shopBanTime0 = 0;
    serialize(src.PartyTimes.shopBanTimes, &dst->partyTimes.shopBanTimes);
    serialize(src.PartyTimes.CounterEventValues, &dst->partyTimes.counterEventValues);
    serialize(src.PartyTimes.HistoryEventTimes, &dst->partyTimes.historyEventTimes);
    serialize(src.PartyTimes._s_times, &dst->partyTimes.someOtherTimes);

    dst->position.x = src.vPosition.x;
    dst->position.y = src.vPosition.y;
    dst->position.z = src.vPosition.z;
    dst->viewYaw = src._viewYaw;
    dst->viewPitch = src._viewPitch;
    dst->prevPosition.x = src.vPrevPosition.x;
    dst->prevPosition.y = src.vPrevPosition.y;
    dst->prevPosition.z = src.vPrevPosition.z;
    dst->viewPrevYaw = src._viewPrevYaw;
    dst->viewPrevPitch = src._viewPrevPitch;
    dst->prevEyeLevel = src.sPrevEyelevel;
    dst->field_6E0 = src.field_6E0_set0_unused;
    dst->field_6E4 = src.field_6E4_set0_unused;
    dst->fallSpeed = src.uFallSpeed;
    dst->field_6EC = src.field_6EC_set0_unused;
    dst->field_6F0 = src.sPartySavedFlightZ;
    dst->floorFacePid = src.floor_face_pid;
    dst->walkSoundTimer = 0; // zero walking sound timer, in OE it was removed and it is little meaning saving it
    dst->waterLavaTimer = src._6FC_water_lava_timer;
    dst->fallStartZ = src.uFallStartZ;
    dst->flying = src.bFlying;
    dst->field_708 = src.field_708_set15_unused;
    dst->hirelingScrollPosition = src.hirelingScrollPosition;
    dst->field_70A = src.cNonHireFollowers;
    dst->field_70B = src.field_70B_set0_unused;
    dst->currentYear = src.uCurrentYear;
    dst->currentMonth = src.uCurrentMonth;
    dst->currentMonthWeek = src.uCurrentMonthWeek;
    dst->currentDayOfMonth = src.uCurrentDayOfMonth;
    dst->currentHour = src.uCurrentHour;
    dst->currentMinute = src.uCurrentMinute;
    dst->currentTimeSecond = src.uCurrentTimeSecond;
    dst->numFoodRations = src.GetFood();
    dst->field_72C = src.field_72C_set0_unused;
    dst->field_730 = src.field_730_set0_unused;
    dst->numGold = src.GetGold();
    dst->numGoldInBank = src.uNumGoldInBank;
    dst->numDeaths = src.uNumDeaths;
    dst->field_740 = src.field_740_set0_unused;
    dst->numPrisonTerms = src.uNumPrisonTerms;
    dst->numBountiesCollected = src.uNumBountiesCollected;
    dst->field_74C = src.field_74C_set0_unused;

    serialize(src.monster_id_for_hunting, &dst->monsterIdForHunting);
    serialize(src.monster_for_hunting_killed, &dst->monsterForHuntingKilled, convert<bool, int16_t>());

    dst->daysPlayedWithoutRest = src.days_played_without_rest;

    serialize(src._questBits, &dst->questBits);
    serialize(src.pArcomageWins, &dst->arcomageWins);

    dst->field_7B5_in_arena_quest = src.field_7B5_in_arena_quest;
    dst->numArenaWins = src.uNumArenaWins;

    serialize(src.pIsArtifactFound, &dst->isArtifactFound);
    serialize(src.field_7d7_set0_unused, &dst->field_7d7);
    serialize(src._autonoteBits, &dst->autonoteBits);
    serialize(src.field_818_set0_unused, &dst->field_818);
    serialize(src.random_order_num_unused, &dst->field_854);

    dst->numArcomageWins = src.uNumArcomageWins;
    dst->numArcomageLoses = src.uNumArcomageLoses;
    dst->turnBasedModeOn = src.bTurnBasedModeOn;
    dst->field_880 = src.field_880_set0_unused;
    dst->flags2 = src.uFlags2;

    uint align = 0;
    if (src.alignment == PartyAlignment::PartyAlignment_Evil) align = 2;
    if (src.alignment == PartyAlignment::PartyAlignment_Neutral) align = 1;
    dst->alignment = align;

    serialize(src.pPartyBuffs, &dst->partyBuffs);
    serialize(src.pPlayers, &dst->players);
    serialize(src.pHirelings, &dst->hirelings);

    serialize(src.pPickedItem, &dst->pickedItem);

    dst->flags = src.uFlags;

    dst->standartItemsInShop0.fill({});
    serialize(src.standartItemsInShops, &dst->standartItemsInShops);
    dst->specialItemsInShop0.fill({});
    serialize(src.specialItemsInShops, &dst->specialItemsInShops);
    serialize(src.spellBooksInGuilds, &dst->spellBooksInGuilds);
    serialize(src.field_1605C_set0_unused, &dst->field_1605C);

    serialize(src.pHireling1Name, &dst->hireling1Name);
    serialize(src.pHireling2Name, &dst->hireling2Name);

    dst->armageddonTimer = src.armageddon_timer;
    dst->armageddonDamage = src.armageddonDamage;

    serialize(src.pTurnBasedPlayerRecoveryTimes, &dst->turnBasedPlayerRecoveryTimes);
    serialize(src.InTheShopFlags, &dst->inTheShopFlags);

    dst->fine = src.uFine;
    dst->torchlightColorR = src.flt_TorchlightColorR;
    dst->torchlightColorG = src.flt_TorchlightColorG;
    dst->torchlightColorB = src.flt_TorchlightColorB;
}

void deserialize(const Party_MM7 &src, Party *dst) {
    dst->field_0_set25_unused = src.field_0;
    dst->uPartyHeight = src.partyHeight;
    dst->uDefaultPartyHeight = src.defaultPartyHeight;
    dst->sEyelevel = src.eyeLevel;
    dst->uDefaultEyelevel = src.defaultEyeLevel;
    dst->radius = src.radius;
    dst->_yawGranularity = src.yawGranularity;
    dst->uWalkSpeed = src.walkSpeed;
    dst->_yawRotationSpeed = src.yawRotationSpeed;
    dst->jump_strength = src.jumpStrength;
    dst->field_28_set0_unused = src.field_28;
    dst->playing_time.value = src.timePlayed;
    dst->last_regenerated.value = src.lastRegenerationTime;

    deserialize(src.partyTimes.bountyHuntingNextGenerationTime, &dst->PartyTimes.bountyHuntNextGenTime);
    deserialize(src.partyTimes.shopsNextGenerationTime, &dst->PartyTimes.shopNextRefreshTime);
    deserialize(src.partyTimes.guildsNextGenerationTime, &dst->PartyTimes.guildNextRefreshTime);
    deserialize(src.partyTimes.shopBanTimes, &dst->PartyTimes.shopBanTimes);
    deserialize(src.partyTimes.counterEventValues, &dst->PartyTimes.CounterEventValues);
    deserialize(src.partyTimes.historyEventTimes, &dst->PartyTimes.HistoryEventTimes);
    deserialize(src.partyTimes.someOtherTimes, &dst->PartyTimes._s_times);

    dst->vPosition.x = src.position.x;
    dst->vPosition.y = src.position.y;
    dst->vPosition.z = src.position.z;
    dst->_viewYaw = src.viewYaw;
    dst->_viewPitch = src.viewPitch;
    dst->vPrevPosition.x = src.prevPosition.x;
    dst->vPrevPosition.y = src.prevPosition.y;
    dst->vPrevPosition.z = src.prevPosition.z;
    dst->_viewPrevYaw = src.viewPrevYaw;
    dst->_viewPrevPitch = src.viewPrevPitch;
    dst->sPrevEyelevel = src.prevEyeLevel;
    dst->field_6E0_set0_unused = src.field_6E0;
    dst->field_6E4_set0_unused = src.field_6E4;
    dst->uFallSpeed = src.fallSpeed;
    dst->field_6EC_set0_unused = src.field_6EC;
    dst->sPartySavedFlightZ = src.field_6F0;
    dst->floor_face_pid = src.floorFacePid;
    // Walking sound timer was removed from OE
    //dst->walk_sound_timer = src.walk_sound_timer;
    dst->_6FC_water_lava_timer = src.waterLavaTimer;
    dst->uFallStartZ = src.fallStartZ;
    dst->bFlying = src.flying;
    dst->field_708_set15_unused = src.field_708;
    dst->hirelingScrollPosition = src.hirelingScrollPosition;
    dst->cNonHireFollowers = src.field_70A;
    dst->field_70B_set0_unused = src.field_70B;
    dst->uCurrentYear = src.currentYear;
    dst->uCurrentMonth = src.currentMonth;
    dst->uCurrentMonthWeek = src.currentMonthWeek;
    dst->uCurrentDayOfMonth = src.currentDayOfMonth;
    dst->uCurrentHour = src.currentHour;
    dst->uCurrentMinute = src.currentMinute;
    dst->uCurrentTimeSecond = src.currentTimeSecond;
    dst->uNumFoodRations = src.numFoodRations;
    dst->field_72C_set0_unused = src.field_72C;
    dst->field_730_set0_unused = src.field_730;
    dst->uNumGold = src.numGold;
    dst->uNumGoldInBank = src.numGoldInBank;
    dst->uNumDeaths = src.numDeaths;
    dst->field_740_set0_unused = src.field_740;
    dst->uNumPrisonTerms = src.numPrisonTerms;
    dst->uNumBountiesCollected = src.numBountiesCollected;
    dst->field_74C_set0_unused = src.field_74C;

    deserialize(src.monsterIdForHunting, &dst->monster_id_for_hunting);
    deserialize(src.monsterForHuntingKilled, &dst->monster_for_hunting_killed, convert<int16_t, bool>());

    dst->days_played_without_rest = src.daysPlayedWithoutRest;

    deserialize(src.questBits, &dst->_questBits);
    deserialize(src.arcomageWins, &dst->pArcomageWins);

    dst->field_7B5_in_arena_quest = src.field_7B5_in_arena_quest;
    dst->uNumArenaWins = src.numArenaWins;

    deserialize(src.isArtifactFound, &dst->pIsArtifactFound);
    deserialize(src.field_7d7, &dst->field_7d7_set0_unused);
    deserialize(src.autonoteBits, &dst->_autonoteBits);
    deserialize(src.field_818, &dst->field_818_set0_unused);
    deserialize(src.field_854, &dst->random_order_num_unused);

    dst->uNumArcomageWins = src.numArcomageWins;
    dst->uNumArcomageLoses = src.numArcomageLoses;
    dst->bTurnBasedModeOn = src.turnBasedModeOn;
    dst->field_880_set0_unused = src.field_880;
    dst->uFlags2 = src.flags2;

    switch (src.alignment) {
        case 0:
            dst->alignment = PartyAlignment::PartyAlignment_Good;
            break;
        case 1:
            dst->alignment = PartyAlignment::PartyAlignment_Neutral;
            break;
        case 2:
            dst->alignment = PartyAlignment::PartyAlignment_Evil;
            break;
        default:
            Assert(false);
    }

    deserialize(src.partyBuffs, &dst->pPartyBuffs);
    deserialize(src.players, &dst->pPlayers);
    deserialize(src.hirelings, &dst->pHirelings);

    deserialize(src.pickedItem, &dst->pPickedItem);

    dst->uFlags = src.flags;

    deserialize(src.standartItemsInShops, &dst->standartItemsInShops);
    deserialize(src.specialItemsInShops, &dst->specialItemsInShops);
    deserialize(src.spellBooksInGuilds, &dst->spellBooksInGuilds);

    deserialize(src.field_1605C, &dst->field_1605C_set0_unused);

    deserialize(src.hireling1Name, &dst->pHireling1Name);
    deserialize(src.hireling2Name, &dst->pHireling2Name);

    dst->armageddon_timer = src.armageddonTimer;
    dst->armageddonDamage = src.armageddonDamage;

    deserialize(src.turnBasedPlayerRecoveryTimes, &dst->pTurnBasedPlayerRecoveryTimes);
    deserialize(src.inTheShopFlags, &dst->InTheShopFlags);

    dst->uFine = src.fine;

    // is this correct / ever used??
    dst->flt_TorchlightColorR = src.torchlightColorR;
    dst->flt_TorchlightColorG = src.torchlightColorG;
    dst->flt_TorchlightColorB = src.torchlightColorB;
}

void serialize(const Player &src, Player_MM7 *dst) {
    memzero(dst);

    for (unsigned int i = 0; i < 20; ++i)
        dst->conditions[i] = src.conditions.Get(static_cast<Condition>(i)).value;

    dst->experience = src.experience;

    serialize(src.name, &dst->name);

    dst->sex = src.uSex;
    dst->classType = src.classType;
    dst->currentFace = src.uCurrentFace;
    dst->might = src.uMight;
    dst->mightBonus = src.uMightBonus;
    dst->intelligence = src.uIntelligence;
    dst->intelligenceBonus = src.uIntelligenceBonus;
    dst->personality = src.uPersonality;
    dst->personalityBonus = src.uPersonalityBonus;
    dst->endurance = src.uEndurance;
    dst->enduranceBonus = src.uEnduranceBonus;
    dst->speed = src.uSpeed;
    dst->speedBonus = src.uSpeedBonus;
    dst->accuracy = src.uAccuracy;
    dst->accuracyBonus = src.uAccuracyBonus;
    dst->luck = src.uLuck;
    dst->luckBonus = src.uLuckBonus;
    dst->acModifier = src.sACModifier;
    dst->level = src.uLevel;
    dst->levelModifier = src.sLevelModifier;
    dst->ageModifier = src.sAgeModifier;
    dst->field_E0 = src.field_E0;
    dst->field_E4 = src.field_E4;
    dst->field_E8 = src.field_E8;
    dst->field_EC = src.field_EC;
    dst->field_F0 = src.field_F0;
    dst->field_F4 = src.field_F4;
    dst->field_F8 = src.field_F8;
    dst->field_FC = src.field_FC;
    dst->field_100 = src.field_100;
    dst->field_104 = src.field_104;

    serialize(src.pActiveSkills, &dst->activeSkills, segment<PLAYER_SKILL_FIRST_VISIBLE, PLAYER_SKILL_LAST_VISIBLE>());
    serialize(src._achievedAwardsBits, &dst->achievedAwardsBits);
    serialize(src.spellbook.bHaveSpell, &dst->spellbook.haveSpell);

    dst->pureLuckUsed = src.pure_luck_used;
    dst->pureSpeedUsed = src.pure_speed_used;
    dst->pureIntellectUsed = src.pure_intellect_used;
    dst->pureEnduranceUsed = src.pure_endurance_used;
    dst->purePersonalityUsed = src.pure_personality_used;
    dst->pureAccuracyUsed = src.pure_accuracy_used;
    dst->pureMightUsed = src.pure_might_used;

    serialize(src.pOwnItems, &dst->ownItems);
    serialize(src.pInventoryMatrix, &dst->inventoryMatrix);

    dst->resFireBase = src.sResFireBase;
    dst->resAirBase = src.sResAirBase;
    dst->resWaterBase = src.sResWaterBase;
    dst->resEarthBase = src.sResEarthBase;
    dst->resPhysicalBase = src.sResPhysicalBase;
    dst->resMagicBase = src.sResMagicBase;
    dst->resSpiritBase = src.sResSpiritBase;
    dst->resMindBase = src.sResMindBase;
    dst->resBodyBase = src.sResBodyBase;
    dst->resLightBase = src.sResLightBase;
    dst->resDarkBase = src.sResDarkBase;
    dst->resFireBonus = src.sResFireBonus;
    dst->resAirBonus = src.sResAirBonus;
    dst->resWaterBonus = src.sResWaterBonus;
    dst->resEarthBonus = src.sResEarthBonus;
    dst->resPhysicalBonus = src.sResPhysicalBonus;
    dst->resMagicBonus = src.sResMagicBonus;
    dst->resSpiritBonus = src.sResSpiritBonus;
    dst->resMindBonus = src.sResMindBonus;
    dst->resBodyBonus = src.sResBodyBonus;
    dst->resLightBonus = src.sResLightBonus;
    dst->resDarkBonus = src.sResDarkBonus;

    serialize(src.pPlayerBuffs, &dst->playerBuffs);

    dst->voiceId = src.uVoiceID;
    dst->prevVoiceId = src.uPrevVoiceID;
    dst->prevFace = src.uPrevFace;
    dst->field_192C = src.field_192C;
    dst->field_1930 = src.field_1930;
    dst->timeToRecovery = src.timeToRecovery;
    dst->field_1936 = src.field_1936;
    dst->field_1937 = src.field_1937;
    dst->skillPoints = src.uSkillPoints;
    dst->health = src.health;
    dst->mana = src.mana;
    dst->birthYear = src.uBirthYear;

    serialize(src.pEquipment.pIndices, &dst->equipment.indices);

    serialize(src.field_1988, &dst->field_1988);

    dst->field_1A4C = src.field_1A4C;
    dst->field_1A4D = src.field_1A4D;
    dst->lastOpenedSpellbookPage = src.lastOpenedSpellbookPage;
    dst->quickSpell = std::to_underlying(src.uQuickSpell);

    serialize(src._playerEventBits, &dst->playerEventBits);

    dst->someAttackBonus = src._some_attack_bonus;
    dst->field_1A91 = src.field_1A91;
    dst->meleeDmgBonus = src._melee_dmg_bonus;
    dst->field_1A93 = src.field_1A93;
    dst->rangedAttackBonus = src._ranged_atk_bonus;
    dst->field_1A95 = src.field_1A95;
    dst->rangedDmgBonus = src._ranged_dmg_bonus;
    dst->field_1A97 = src.field_1A97_set0_unused;
    dst->fullHealthBonus = src.uFullHealthBonus;
    dst->healthRelated = src._health_related;
    dst->fullManaBonus = src.uFullManaBonus;
    dst->manaRelated = src._mana_related;
    dst->expression = src.expression;
    dst->expressionTimePassed = src.uExpressionTimePassed;
    dst->expressionTimeLength = src.uExpressionTimeLength;
    dst->field_1AA2 = src.uExpressionImageIndex;
    dst->_expression21_animtime = src._expression21_animtime;
    dst->_expression21_frameset = src._expression21_frameset;

    for (unsigned int i = 0; i < 5; ++i) {
        if (i >= src.vBeacons.size()) {
            continue;
        }
        dst->installedBeacons[i].beaconTime = src.vBeacons[i].uBeaconTime.value;
        dst->installedBeacons[i].partyPosX = src.vBeacons[i].PartyPos_X;
        dst->installedBeacons[i].partyPosY = src.vBeacons[i].PartyPos_Y;
        dst->installedBeacons[i].partyPosZ = src.vBeacons[i].PartyPos_Z;
        dst->installedBeacons[i].partyViewYaw = src.vBeacons[i]._partyViewYaw;
        dst->installedBeacons[i].partyViewPitch = src.vBeacons[i]._partyViewPitch;
        dst->installedBeacons[i].saveFileId = src.vBeacons[i].SaveFileID;
    }

    dst->numDivineInterventionCasts = src.uNumDivineInterventionCastsThisDay;
    dst->numArmageddonCasts = src.uNumArmageddonCasts;
    dst->numFireSpikeCasts = src.uNumFireSpikeCasts;
    dst->field_1B3B = src.field_1B3B_set0_unused;
}

void deserialize(const Player_MM7 &src, Player *dst) {
    for (unsigned int i = 0; i < 20; ++i)
        dst->conditions.Set(static_cast<Condition>(i), GameTime(src.conditions[i]));

    dst->experience = src.experience;

    deserialize(src.name, &dst->name);

    switch (src.sex) {
    case 0:
        dst->uSex = SEX_MALE;
        break;
    case 1:
        dst->uSex = SEX_FEMALE;
        break;
    default:
        Assert(false);
    }

    switch (src.classType) {
    case 0:
        dst->classType = PLAYER_CLASS_KNIGHT;
        break;
    case 1:
        dst->classType = PLAYER_CLASS_CHEVALIER;
        break;
    case 2:
        dst->classType = PLAYER_CLASS_CHAMPION;
        break;
    case 3:
        dst->classType = PLAYER_CLASS_BLACK_KNIGHT;
        break;
    case 4:
        dst->classType = PLAYER_CLASS_THIEF;
        break;
    case 5:
        dst->classType = PLAYER_CLASS_ROGUE;
        break;
    case 6:
        dst->classType = PLAYER_CLASS_SPY;
        break;
    case 7:
        dst->classType = PLAYER_CLASS_ASSASSIN;
        break;
    case 8:
        dst->classType = PLAYER_CLASS_MONK;
        break;
    case 9:
        dst->classType = PLAYER_CLASS_INITIATE;
        break;
    case 10:
        dst->classType = PLAYER_CLASS_MASTER;
        break;
    case 11:
        dst->classType = PLAYER_CLASS_NINJA;
        break;
    case 12:
        dst->classType = PLAYER_CLASS_PALADIN;
        break;
    case 13:
        dst->classType = PLAYER_CLASS_CRUSADER;
        break;
    case 14:
        dst->classType = PLAYER_CLASS_HERO;
        break;
    case 15:
        dst->classType = PLAYER_CLASS_VILLIAN;
        break;
    case 16:
        dst->classType = PLAYER_CLASS_ARCHER;
        break;
    case 17:
        dst->classType = PLAYER_CLASS_WARRIOR_MAGE;
        break;
    case 18:
        dst->classType = PLAYER_CLASS_MASTER_ARCHER;
        break;
    case 19:
        dst->classType = PLAYER_CLASS_SNIPER;
        break;
    case 20:
        dst->classType = PLAYER_CLASS_RANGER;
        break;
    case 21:
        dst->classType = PLAYER_CLASS_HUNTER;
        break;
    case 22:
        dst->classType = PLAYER_CLASS_RANGER_LORD;
        break;
    case 23:
        dst->classType = PLAYER_CLASS_BOUNTY_HUNTER;
        break;
    case 24:
        dst->classType = PLAYER_CLASS_CLERIC;
        break;
    case 25:
        dst->classType = PLAYER_CLASS_PRIEST;
        break;
    case 26:
        dst->classType = PLAYER_CLASS_PRIEST_OF_SUN;
        break;
    case 27:
        dst->classType = PLAYER_CLASS_PRIEST_OF_MOON;
        break;
    case 28:
        dst->classType = PLAYER_CLASS_DRUID;
        break;
    case 29:
        dst->classType = PLAYER_CLASS_GREAT_DRUID;
        break;
    case 30:
        dst->classType = PLAYER_CLASS_ARCH_DRUID;
        break;
    case 31:
        dst->classType = PLAYER_CLASS_WARLOCK;
        break;
    case 32:
        dst->classType = PLAYER_CLASS_SORCERER;
        break;
    case 33:
        dst->classType = PLAYER_CLASS_WIZARD;
        break;
    case 34:
        dst->classType = PLAYER_CLASS_ARCHMAGE;
        break;
    case 35:
        dst->classType = PLAYER_CLASS_LICH;
        break;
    default:
        Assert(false);
    }

    dst->uCurrentFace = src.currentFace;
    dst->uMight = src.might;
    dst->uMightBonus = src.mightBonus;
    dst->uIntelligence = src.intelligence;
    dst->uIntelligenceBonus = src.intelligenceBonus;
    dst->uPersonality = src.personality;
    dst->uPersonalityBonus = src.personalityBonus;
    dst->uEndurance = src.endurance;
    dst->uEnduranceBonus = src.enduranceBonus;
    dst->uSpeed = src.speed;
    dst->uSpeedBonus = src.speedBonus;
    dst->uAccuracy = src.accuracy;
    dst->uAccuracyBonus = src.accuracyBonus;
    dst->uLuck = src.luck;
    dst->uLuckBonus = src.luckBonus;
    dst->sACModifier = src.acModifier;
    dst->uLevel = src.level;
    dst->sLevelModifier = src.levelModifier;
    dst->sAgeModifier = src.ageModifier;
    dst->field_E0 = src.field_E0;
    dst->field_E4 = src.field_E4;
    dst->field_E8 = src.field_E8;
    dst->field_EC = src.field_EC;
    dst->field_F0 = src.field_F0;
    dst->field_F4 = src.field_F4;
    dst->field_F8 = src.field_F8;
    dst->field_FC = src.field_FC;
    dst->field_100 = src.field_100;
    dst->field_104 = src.field_104;

    deserialize(src.activeSkills, &dst->pActiveSkills, segment<PLAYER_SKILL_FIRST_VISIBLE, PLAYER_SKILL_LAST_VISIBLE>());
    deserialize(src.achievedAwardsBits, &dst->_achievedAwardsBits);
    deserialize(src.spellbook.haveSpell, &dst->spellbook.bHaveSpell);

    dst->pure_luck_used = src.pureLuckUsed;
    dst->pure_speed_used = src.pureSpeedUsed;
    dst->pure_intellect_used = src.pureIntellectUsed;
    dst->pure_endurance_used = src.pureEnduranceUsed;
    dst->pure_personality_used = src.purePersonalityUsed;
    dst->pure_accuracy_used = src.pureAccuracyUsed;
    dst->pure_might_used = src.pureMightUsed;

    deserialize(src.ownItems, &dst->pOwnItems);
    deserialize(src.inventoryMatrix, &dst->pInventoryMatrix);

    dst->sResFireBase = src.resFireBase;
    dst->sResAirBase = src.resAirBase;
    dst->sResWaterBase = src.resWaterBase;
    dst->sResEarthBase = src.resEarthBase;
    dst->sResPhysicalBase = src.resPhysicalBase;
    dst->sResMagicBase = src.resMagicBase;
    dst->sResSpiritBase = src.resSpiritBase;
    dst->sResMindBase = src.resMindBase;
    dst->sResBodyBase = src.resBodyBase;
    dst->sResLightBase = src.resLightBase;
    dst->sResDarkBase = src.resDarkBase;
    dst->sResFireBonus = src.resFireBonus;
    dst->sResAirBonus = src.resAirBonus;
    dst->sResWaterBonus = src.resWaterBonus;
    dst->sResEarthBonus = src.resEarthBonus;
    dst->sResPhysicalBonus = src.resPhysicalBonus;
    dst->sResMagicBonus = src.resMagicBonus;
    dst->sResSpiritBonus = src.resSpiritBonus;
    dst->sResMindBonus = src.resMindBonus;
    dst->sResBodyBonus = src.resBodyBonus;
    dst->sResLightBonus = src.resLightBonus;
    dst->sResDarkBonus = src.resDarkBonus;

    deserialize(src.playerBuffs, &dst->pPlayerBuffs);

    dst->uVoiceID = src.voiceId;
    dst->uPrevVoiceID = src.prevVoiceId;
    dst->uPrevFace = src.prevFace;
    dst->field_192C = src.field_192C;
    dst->field_1930 = src.field_1930;
    dst->timeToRecovery = src.timeToRecovery;
    dst->field_1936 = src.field_1936;
    dst->field_1937 = src.field_1937;
    dst->uSkillPoints = src.skillPoints;
    dst->health = src.health;
    dst->mana = src.mana;
    dst->uBirthYear = src.birthYear;

    deserialize(src.equipment.indices, &dst->pEquipment.pIndices);

    deserialize(src.field_1988, &dst->field_1988);

    dst->field_1A4C = src.field_1A4C;
    dst->field_1A4D = src.field_1A4D;
    dst->lastOpenedSpellbookPage = src.lastOpenedSpellbookPage;
    dst->uQuickSpell = static_cast<SPELL_TYPE>(src.quickSpell);

    deserialize(src.playerEventBits, &dst->_playerEventBits);

    dst->_some_attack_bonus = src.someAttackBonus;
    dst->field_1A91 = src.field_1A91;
    dst->_melee_dmg_bonus = src.meleeDmgBonus;
    dst->field_1A93 = src.field_1A93;
    dst->_ranged_atk_bonus = src.rangedAttackBonus;
    dst->field_1A95 = src.field_1A95;
    dst->_ranged_dmg_bonus = src.rangedDmgBonus;
    dst->field_1A97_set0_unused = src.field_1A97;
    dst->uFullHealthBonus = src.fullHealthBonus;
    dst->_health_related = src.healthRelated;
    dst->uFullManaBonus = src.fullManaBonus;
    dst->_mana_related = src.manaRelated;
    dst->expression = (CHARACTER_EXPRESSION_ID)src.expression;
    dst->uExpressionTimePassed = src.expressionTimePassed;
    dst->uExpressionTimeLength = src.expressionTimeLength;
    dst->uExpressionImageIndex = src.field_1AA2;
    dst->_expression21_animtime = src._expression21_animtime;
    dst->_expression21_frameset = src._expression21_frameset;

    for (int z = 0; z < dst->vBeacons.size(); z++)
        dst->vBeacons[z].image->Release();
    dst->vBeacons.clear();

    for (unsigned int i = 0; i < 5; ++i) {
        if (src.installedBeacons[i].beaconTime != 0) {
            LloydBeacon beacon;
            beacon.uBeaconTime = GameTime(src.installedBeacons[i].beaconTime);
            beacon.PartyPos_X = src.installedBeacons[i].partyPosX;
            beacon.PartyPos_Y = src.installedBeacons[i].partyPosY;
            beacon.PartyPos_Z = src.installedBeacons[i].partyPosZ;
            beacon._partyViewYaw = src.installedBeacons[i].partyViewYaw;
            beacon._partyViewPitch = src.installedBeacons[i].partyViewPitch;
            beacon.SaveFileID = src.installedBeacons[i].saveFileId;
            dst->vBeacons.push_back(beacon);
        }
    }

    dst->uNumDivineInterventionCastsThisDay = src.numDivineInterventionCasts;
    dst->uNumArmageddonCasts = src.numArmageddonCasts;
    dst->uNumFireSpikeCasts = src.numFireSpikeCasts;
    dst->field_1B3B_set0_unused = src.field_1B3B;
}

void serialize(const Icon &src, IconFrame_MM7 *dst) {
    memzero(dst);

    serialize(src.GetAnimationName(), &dst->animationName);
    dst->animLength = src.GetAnimLength();

    serialize(src.pTextureName, &dst->textureName);
    dst->animTime = src.GetAnimTime();
    dst->flags = src.uFlags;
}

void deserialize(const IconFrame_MM7 &src, Icon *dst) {
    std::string name;
    deserialize(src.animationName, &name);
    dst->SetAnimationName(name);
    dst->SetAnimLength(8 * src.animLength);

    deserialize(src.textureName, &dst->pTextureName);
    dst->SetAnimTime(src.animTime);
    dst->uFlags = src.flags;
}

void serialize(const UIAnimation &src, UIAnimation_MM7 *dst) {
    memzero(dst);

    /* 000 */ dst->iconId = src.icon->id;
    /* 002 */ dst->field_2 = src.field_2;
    /* 004 */ dst->animTime = src.uAnimTime;
    /* 006 */ dst->animLength = src.uAnimLength;
    /* 008 */ dst->x = src.x;
    /* 00A */ dst->y = src.y;
    /* 00C */ dst->field_C = src.field_C;
}

void deserialize(const UIAnimation_MM7 &src, UIAnimation *dst) {
    dst->icon = pIconsFrameTable->GetIcon(src.iconId);
    ///* 000 */ anim->uIconID = src.uIconID;
    /* 002 */ dst->field_2 = src.field_2;
    /* 004 */ dst->uAnimTime = src.animTime;
    /* 006 */ dst->uAnimLength = src.animLength;
    /* 008 */ dst->x = src.x;
    /* 00A */ dst->y = src.y;
    /* 00C */ dst->field_C = src.field_C;
}

void deserialize(const MonsterDesc_MM6 &src, MonsterDesc *dst) {
    dst->uMonsterHeight = src.monsterHeight;
    dst->uMonsterRadius = src.monsterRadius;
    dst->uMovementSpeed = src.movementSpeed;
    dst->uToHitRadius = src.toHitRadius;
    dst->sTintColor = colorTable.White.c32();
    dst->pSoundSampleIDs = src.soundSampleIds;
    deserialize(src.monsterName, &dst->pMonsterName);
    deserialize(src.spriteNames, &dst->pSpriteNames);
}

void serialize(const MonsterDesc &src, MonsterDesc_MM7 *dst) {
    memzero(dst);

    dst->monsterHeight = src.uMonsterHeight;
    dst->monsterRadius = src.uMonsterRadius;
    dst->movementSpeed = src.uMovementSpeed;
    dst->toHitRadius = src.uToHitRadius;
    dst->tintColor = src.sTintColor;
    dst->soundSampleIds = src.pSoundSampleIDs;
    serialize(src.pMonsterName, &dst->monsterName);
    serialize(src.pSpriteNames, &dst->spriteNames);
    dst->spriteNamesUnused[0].fill('\0');
    dst->spriteNamesUnused[1].fill('\0');
}

void deserialize(const MonsterDesc_MM7 &src, MonsterDesc *dst) {
    dst->uMonsterHeight = src.monsterHeight;
    dst->uMonsterRadius = src.monsterRadius;
    dst->uMovementSpeed = src.movementSpeed;
    dst->uToHitRadius = src.toHitRadius;
    dst->sTintColor = src.tintColor;
    dst->pSoundSampleIDs = src.soundSampleIds;
    deserialize(src.monsterName, &dst->pMonsterName);
    deserialize(src.spriteNames, &dst->pSpriteNames);
}

void serialize(const ActorJob &src, ActorJob_MM7 *dst) {
    memzero(dst);
    dst->pos = src.vPos;
    dst->attributes = src.uAttributes;
    dst->action = src.uAction;
    dst->hour = src.uHour;
    dst->day = src.uDay;
    dst->month = src.uMonth;
}

void deserialize(const ActorJob_MM7 &src, ActorJob *dst) {
    dst->vPos = src.pos;
    dst->uAttributes = src.attributes;
    dst->uAction = src.action;
    dst->uHour = src.hour;
    dst->uDay = src.day;
    dst->uMonth = src.month;
}

void serialize(const Actor &src, Actor_MM7 *dst) {
    memzero(dst);

    serialize(src.pActorName, &dst->pActorName);

    dst->sNPC_ID = src.sNPC_ID;
    dst->field_22 = src.field_22;
    dst->uAttributes = std::to_underlying(src.uAttributes);
    dst->sCurrentHP = src.sCurrentHP;

    dst->pMonsterInfo.level = src.pMonsterInfo.uLevel;
    dst->pMonsterInfo.treasureDropChance = src.pMonsterInfo.uTreasureDropChance;
    dst->pMonsterInfo.treasureDiceRolls = src.pMonsterInfo.uTreasureDiceRolls;
    dst->pMonsterInfo.treasureDiceSides = src.pMonsterInfo.uTreasureDiceSides;
    dst->pMonsterInfo.treasureLevel = std::to_underlying(src.pMonsterInfo.uTreasureLevel);
    dst->pMonsterInfo.treasureType = src.pMonsterInfo.uTreasureType;
    dst->pMonsterInfo.flying = src.pMonsterInfo.uFlying;
    dst->pMonsterInfo.movementType = src.pMonsterInfo.uMovementType;
    dst->pMonsterInfo.aiType = src.pMonsterInfo.uAIType;
    dst->pMonsterInfo.hostilityType = (uint8_t)src.pMonsterInfo.uHostilityType;
    dst->pMonsterInfo.field_12 = src.pMonsterInfo.field_12;
    dst->pMonsterInfo.specialAttackType = src.pMonsterInfo.uSpecialAttackType;
    dst->pMonsterInfo.specialAttackLevel = src.pMonsterInfo.uSpecialAttackLevel;
    dst->pMonsterInfo.attack1Type = src.pMonsterInfo.uAttack1Type;
    dst->pMonsterInfo.attack1DamageDiceRolls = src.pMonsterInfo.uAttack1DamageDiceRolls;
    dst->pMonsterInfo.attack1DamageDiceSides = src.pMonsterInfo.uAttack1DamageDiceSides;
    dst->pMonsterInfo.attack1DamageBonus = src.pMonsterInfo.uAttack1DamageBonus;
    dst->pMonsterInfo.missileAttack1Type = src.pMonsterInfo.uMissleAttack1Type;
    dst->pMonsterInfo.attack2Chance = src.pMonsterInfo.uAttack2Chance;
    dst->pMonsterInfo.attack2Type = src.pMonsterInfo.uAttack2Type;
    dst->pMonsterInfo.attack2DamageDiceRolls = src.pMonsterInfo.uAttack2DamageDiceRolls;
    dst->pMonsterInfo.attack2DamageDiceSides = src.pMonsterInfo.uAttack2DamageDiceSides;
    dst->pMonsterInfo.attack2DamageBonus = src.pMonsterInfo.uAttack2DamageBonus;
    dst->pMonsterInfo.missileAttack2Type = src.pMonsterInfo.uMissleAttack2Type;
    dst->pMonsterInfo.spell1UseChance = src.pMonsterInfo.uSpell1UseChance;
    dst->pMonsterInfo.spell1Id = std::to_underlying(src.pMonsterInfo.uSpell1ID);
    dst->pMonsterInfo.spell2UseChance = src.pMonsterInfo.uSpell2UseChance;
    dst->pMonsterInfo.spell2Id = std::to_underlying(src.pMonsterInfo.uSpell2ID);
    dst->pMonsterInfo.resFire = src.pMonsterInfo.uResFire;
    dst->pMonsterInfo.resAir = src.pMonsterInfo.uResAir;
    dst->pMonsterInfo.resWater = src.pMonsterInfo.uResWater;
    dst->pMonsterInfo.resEarth = src.pMonsterInfo.uResEarth;
    dst->pMonsterInfo.resMind = src.pMonsterInfo.uResMind;
    dst->pMonsterInfo.resSpirit = src.pMonsterInfo.uResSpirit;
    dst->pMonsterInfo.resBody = src.pMonsterInfo.uResBody;
    dst->pMonsterInfo.resLight = src.pMonsterInfo.uResLight;
    dst->pMonsterInfo.resDark = src.pMonsterInfo.uResDark;
    dst->pMonsterInfo.resPhysical = src.pMonsterInfo.uResPhysical;
    dst->pMonsterInfo.specialAbilityType = src.pMonsterInfo.uSpecialAbilityType;
    dst->pMonsterInfo.specialAbilityDamageDiceRolls = src.pMonsterInfo.uSpecialAbilityDamageDiceRolls;
    dst->pMonsterInfo.specialAbilityDamageDiceSides = src.pMonsterInfo.uSpecialAbilityDamageDiceSides;
    dst->pMonsterInfo.specialAbilityDamageDiceBonus = src.pMonsterInfo.uSpecialAbilityDamageDiceBonus;
    dst->pMonsterInfo.numCharactersAttackedPerSpecialAbility = src.pMonsterInfo.uNumCharactersAttackedPerSpecialAbility;
    dst->pMonsterInfo.field_33 = src.pMonsterInfo.field_33;
    dst->pMonsterInfo.id = src.pMonsterInfo.uID;
    dst->pMonsterInfo.bloodSplatOnDeath = src.pMonsterInfo.bBloodSplatOnDeath;
    dst->pMonsterInfo.spellSkillAndMastery1 = src.pMonsterInfo.uSpellSkillAndMastery1;
    dst->pMonsterInfo.spellSkillAndMastery2 = src.pMonsterInfo.uSpellSkillAndMastery2;
    dst->pMonsterInfo.field_3C_some_special_attack = src.pMonsterInfo.field_3C_some_special_attack;
    dst->pMonsterInfo.field_3E = src.pMonsterInfo.field_3E;
    dst->pMonsterInfo.hp = src.pMonsterInfo.uHP;
    dst->pMonsterInfo.ac = src.pMonsterInfo.uAC;
    dst->pMonsterInfo.exp = src.pMonsterInfo.uExp;
    dst->pMonsterInfo.baseSpeed = src.pMonsterInfo.uBaseSpeed;
    dst->pMonsterInfo.recoveryTime = src.pMonsterInfo.uRecoveryTime;
    dst->pMonsterInfo.attackPreference = src.pMonsterInfo.uAttackPreference;
    dst->word_000084_range_attack = src.word_000084_range_attack;
    dst->word_000086_some_monster_id = src.word_000086_some_monster_id;  // base monster class monsterlist id
    dst->uActorRadius = src.uActorRadius;
    dst->uActorHeight = src.uActorHeight;
    dst->uMovementSpeed = src.uMovementSpeed;
    dst->vPosition = src.vPosition;
    dst->vVelocity = src.vVelocity;
    dst->uYawAngle = src.uYawAngle;
    dst->uPitchAngle = src.uPitchAngle;
    dst->uSectorID = src.uSectorID;
    dst->uCurrentActionLength = src.uCurrentActionLength;
    dst->vInitialPosition = src.vInitialPosition;
    dst->vGuardingPosition = src.vGuardingPosition;
    dst->uTetherDistance = src.uTetherDistance;
    dst->uAIState = std::to_underlying(src.uAIState);
    dst->uCurrentActionAnimation = std::to_underlying(src.uCurrentActionAnimation);
    dst->uCarriedItemID = std::to_underlying(src.uCarriedItemID);
    dst->field_B6 = src.field_B6;
    dst->field_B7 = src.field_B7;
    dst->uCurrentActionTime = src.uCurrentActionTime;

    serialize(src.pSpriteIDs, &dst->pSpriteIDs);
    serialize(src.pSoundSampleIDs, &dst->pSoundSampleIDs);
    serialize(src.pActorBuffs, &dst->pActorBuffs);
    serialize(src.ActorHasItems, &dst->ActorHasItems);

    dst->uGroup = src.uGroup;
    dst->uAlly = src.uAlly;

    serialize(src.pScheduledJobs, &dst->pScheduledJobs);

    dst->uSummonerID = src.uSummonerID;
    dst->uLastCharacterIDToHit = src.uLastCharacterIDToHit;
    dst->dword_000334_unique_name = src.dword_000334_unique_name;

    serialize(src.field_338, &dst->field_338);
}

void deserialize(const Actor_MM7 &src, Actor *dst) {
    deserialize(src.pActorName, &dst->pActorName);
    dst->sNPC_ID = src.sNPC_ID;
    dst->field_22 = src.field_22;
    dst->uAttributes = ActorAttributes(src.uAttributes);
    dst->sCurrentHP = src.sCurrentHP;

    dst->pMonsterInfo.uLevel = src.pMonsterInfo.level;
    dst->pMonsterInfo.uTreasureDropChance = src.pMonsterInfo.treasureDropChance;
    dst->pMonsterInfo.uTreasureDiceRolls = src.pMonsterInfo.treasureDiceRolls;
    dst->pMonsterInfo.uTreasureDiceSides = src.pMonsterInfo.treasureDiceSides;
    dst->pMonsterInfo.uTreasureLevel = static_cast<ITEM_TREASURE_LEVEL>(src.pMonsterInfo.treasureLevel);
    dst->pMonsterInfo.uTreasureType = src.pMonsterInfo.treasureType;
    dst->pMonsterInfo.uFlying = src.pMonsterInfo.flying;
    dst->pMonsterInfo.uMovementType = src.pMonsterInfo.movementType;
    dst->pMonsterInfo.uAIType = src.pMonsterInfo.aiType;
    dst->pMonsterInfo.uHostilityType = static_cast<MonsterInfo::HostilityRadius>(src.pMonsterInfo.hostilityType);
    dst->pMonsterInfo.field_12 = src.pMonsterInfo.field_12;
    dst->pMonsterInfo.uSpecialAttackType = static_cast<SPECIAL_ATTACK_TYPE>(src.pMonsterInfo.specialAttackType);
    dst->pMonsterInfo.uSpecialAttackLevel = src.pMonsterInfo.specialAttackLevel;
    dst->pMonsterInfo.uAttack1Type = src.pMonsterInfo.attack1Type;
    dst->pMonsterInfo.uAttack1DamageDiceRolls = src.pMonsterInfo.attack1DamageDiceRolls;
    dst->pMonsterInfo.uAttack1DamageDiceSides = src.pMonsterInfo.attack1DamageDiceSides;
    dst->pMonsterInfo.uAttack1DamageBonus = src.pMonsterInfo.attack1DamageBonus;
    dst->pMonsterInfo.uMissleAttack1Type = src.pMonsterInfo.missileAttack1Type;
    dst->pMonsterInfo.uAttack2Chance = src.pMonsterInfo.attack2Chance;
    dst->pMonsterInfo.uAttack2Type = src.pMonsterInfo.attack2Type;
    dst->pMonsterInfo.uAttack2DamageDiceRolls = src.pMonsterInfo.attack2DamageDiceRolls;
    dst->pMonsterInfo.uAttack2DamageDiceSides = src.pMonsterInfo.attack2DamageDiceSides;
    dst->pMonsterInfo.uAttack2DamageBonus = src.pMonsterInfo.attack2DamageBonus;
    dst->pMonsterInfo.uMissleAttack2Type = src.pMonsterInfo.missileAttack2Type;
    dst->pMonsterInfo.uSpell1UseChance = src.pMonsterInfo.spell1UseChance;
    dst->pMonsterInfo.uSpell1ID = static_cast<SPELL_TYPE>(src.pMonsterInfo.spell1Id);
    dst->pMonsterInfo.uSpell2UseChance = src.pMonsterInfo.spell2UseChance;
    dst->pMonsterInfo.uSpell2ID = static_cast<SPELL_TYPE>(src.pMonsterInfo.spell2Id);
    dst->pMonsterInfo.uResFire = src.pMonsterInfo.resFire;
    dst->pMonsterInfo.uResAir = src.pMonsterInfo.resAir;
    dst->pMonsterInfo.uResWater = src.pMonsterInfo.resWater;
    dst->pMonsterInfo.uResEarth = src.pMonsterInfo.resEarth;
    dst->pMonsterInfo.uResMind = src.pMonsterInfo.resMind;
    dst->pMonsterInfo.uResSpirit = src.pMonsterInfo.resSpirit;
    dst->pMonsterInfo.uResBody = src.pMonsterInfo.resBody;
    dst->pMonsterInfo.uResLight = src.pMonsterInfo.resLight;
    dst->pMonsterInfo.uResDark = src.pMonsterInfo.resDark;
    dst->pMonsterInfo.uResPhysical = src.pMonsterInfo.resPhysical;
    dst->pMonsterInfo.uSpecialAbilityType = src.pMonsterInfo.specialAbilityType;
    dst->pMonsterInfo.uSpecialAbilityDamageDiceRolls = src.pMonsterInfo.specialAbilityDamageDiceRolls;
    dst->pMonsterInfo.uSpecialAbilityDamageDiceSides = src.pMonsterInfo.specialAbilityDamageDiceSides;
    dst->pMonsterInfo.uSpecialAbilityDamageDiceBonus = src.pMonsterInfo.specialAbilityDamageDiceBonus;
    dst->pMonsterInfo.uNumCharactersAttackedPerSpecialAbility = src.pMonsterInfo.numCharactersAttackedPerSpecialAbility;
    dst->pMonsterInfo.field_33 = src.pMonsterInfo.field_33;
    dst->pMonsterInfo.uID = src.pMonsterInfo.id;
    dst->pMonsterInfo.bBloodSplatOnDeath = src.pMonsterInfo.bloodSplatOnDeath;
    dst->pMonsterInfo.uSpellSkillAndMastery1 = src.pMonsterInfo.spellSkillAndMastery1;
    dst->pMonsterInfo.uSpellSkillAndMastery2 = src.pMonsterInfo.spellSkillAndMastery2;
    dst->pMonsterInfo.field_3C_some_special_attack = src.pMonsterInfo.field_3C_some_special_attack;
    dst->pMonsterInfo.field_3E = src.pMonsterInfo.field_3E;
    dst->pMonsterInfo.uHP = src.pMonsterInfo.hp;
    dst->pMonsterInfo.uAC = src.pMonsterInfo.ac;
    dst->pMonsterInfo.uExp = src.pMonsterInfo.exp;
    dst->pMonsterInfo.uBaseSpeed = src.pMonsterInfo.baseSpeed;
    dst->pMonsterInfo.uRecoveryTime = src.pMonsterInfo.recoveryTime;
    dst->pMonsterInfo.uAttackPreference = src.pMonsterInfo.attackPreference;
    dst->word_000084_range_attack = src.word_000084_range_attack;
    dst->word_000086_some_monster_id = src.word_000086_some_monster_id;  // base monster class monsterlist id
    dst->uActorRadius = src.uActorRadius;
    dst->uActorHeight = src.uActorHeight;
    dst->uMovementSpeed = src.uMovementSpeed;
    dst->vPosition = src.vPosition;
    dst->vVelocity = src.vVelocity;
    dst->uYawAngle = src.uYawAngle;
    dst->uPitchAngle = src.uPitchAngle;
    dst->uSectorID = src.uSectorID;
    dst->uCurrentActionLength = src.uCurrentActionLength;
    dst->vInitialPosition = src.vInitialPosition;
    dst->vGuardingPosition = src.vGuardingPosition;
    dst->uTetherDistance = src.uTetherDistance;
    dst->uAIState = static_cast<AIState>(src.uAIState);
    dst->uCurrentActionAnimation = static_cast<ActorAnimation>(src.uCurrentActionAnimation);
    dst->uCarriedItemID = ITEM_TYPE(src.uCarriedItemID);
    dst->field_B6 = src.field_B6;
    dst->field_B7 = src.field_B7;
    dst->uCurrentActionTime = src.uCurrentActionTime;

    deserialize(src.pSpriteIDs, &dst->pSpriteIDs);
    deserialize(src.pSoundSampleIDs, &dst->pSoundSampleIDs);
    deserialize(src.pActorBuffs, &dst->pActorBuffs);
    deserialize(src.ActorHasItems, &dst->ActorHasItems);

    dst->uGroup = src.uGroup;
    dst->uAlly = src.uAlly;

    deserialize(src.pScheduledJobs, &dst->pScheduledJobs);

    dst->uSummonerID = src.uSummonerID;
    dst->uLastCharacterIDToHit = src.uLastCharacterIDToHit;
    dst->dword_000334_unique_name = src.dword_000334_unique_name;

    deserialize(src.field_338, &dst->field_338);
}

void serialize(const BLVDoor &src, BLVDoor_MM7 *dst) {
    memzero(dst);

    dst->uAttributes = std::to_underlying(src.uAttributes);
    dst->uDoorID = src.uDoorID;
    dst->uTimeSinceTriggered = src.uTimeSinceTriggered;
    dst->vDirection = src.vDirection;
    dst->uMoveLength = src.uMoveLength;
    dst->uOpenSpeed = src.uOpenSpeed;
    dst->uCloseSpeed = src.uCloseSpeed;
    dst->uNumVertices = src.uNumVertices;
    dst->uNumFaces = src.uNumFaces;
    dst->uNumSectors = src.uNumSectors;
    dst->uNumOffsets = src.uNumOffsets;
    dst->uState = std::to_underlying(src.uState);
    dst->field_4E = src.field_4E;
}

void deserialize(const BLVDoor_MM7 &src, BLVDoor *dst) {
    dst->uAttributes = static_cast<DoorAttributes>(src.uAttributes);
    dst->uDoorID = src.uDoorID;
    dst->uTimeSinceTriggered = src.uTimeSinceTriggered;
    dst->vDirection = src.vDirection;
    dst->uMoveLength = src.uMoveLength;
    dst->uOpenSpeed = src.uOpenSpeed;
    dst->uCloseSpeed = src.uCloseSpeed;
    dst->uNumVertices = src.uNumVertices;
    dst->uNumFaces = src.uNumFaces;
    dst->uNumSectors = src.uNumSectors;
    dst->uNumOffsets = src.uNumOffsets;
    dst->uState = static_cast<BLVDoor::State>(src.uState);
    dst->field_4E = src.field_4E;
}

void serialize(const BLVSector &src, BLVSector_MM7 *dst) {
    memzero(dst);

    dst->field_0 = src.field_0;
    dst->uNumFloors = src.uNumFloors;
    dst->field_6 = src.field_6;
    dst->uNumWalls = src.uNumWalls;
    dst->field_E = src.field_E;
    dst->uNumCeilings = src.uNumCeilings;
    dst->field_16 = src.field_16;
    dst->uNumFluids = src.uNumFluids;
    dst->field_1E = src.field_1E;
    dst->uNumPortals = src.uNumPortals;
    dst->field_26 = src.field_26;
    dst->uNumFaces = src.uNumFaces;
    dst->uNumNonBSPFaces = src.uNumNonBSPFaces;
    dst->uNumCylinderFaces = src.uNumCylinderFaces;
    dst->field_36 = src.field_36;
    dst->pCylinderFaces = src.pCylinderFaces;
    dst->uNumCogs = src.uNumCogs;
    dst->field_3E = src.field_3E;
    dst->uNumDecorations = src.uNumDecorations;
    dst->field_46 = src.field_46;
    dst->uNumMarkers = src.uNumMarkers;
    dst->field_4E = src.field_4E;
    dst->uNumLights = src.uNumLights;
    dst->field_56 = src.field_56;
    dst->uWaterLevel = src.uWaterLevel;
    dst->uMistLevel = src.uMistLevel;
    dst->uLightDistanceMultiplier = src.uLightDistanceMultiplier;
    dst->uMinAmbientLightLevel = src.uMinAmbientLightLevel;
    dst->uFirstBSPNode = src.uFirstBSPNode;
    dst->exit_tag = src.exit_tag;
    dst->pBounding = src.pBounding;
}

void deserialize(const BLVSector_MM7 &src, BLVSector *dst) {
    dst->field_0 = src.field_0;
    dst->uNumFloors = src.uNumFloors;
    dst->field_6 = src.field_6;
    dst->uNumWalls = src.uNumWalls;
    dst->field_E = src.field_E;
    dst->uNumCeilings = src.uNumCeilings;
    dst->field_16 = src.field_16;
    dst->uNumFluids = src.uNumFluids;
    dst->field_1E = src.field_1E;
    dst->uNumPortals = src.uNumPortals;
    dst->field_26 = src.field_26;
    dst->uNumFaces = src.uNumFaces;
    dst->uNumNonBSPFaces = src.uNumNonBSPFaces;
    dst->uNumCylinderFaces = src.uNumCylinderFaces;
    dst->field_36 = src.field_36;
    dst->pCylinderFaces = src.pCylinderFaces;
    dst->uNumCogs = src.uNumCogs;
    dst->field_3E = src.field_3E;
    dst->uNumDecorations = src.uNumDecorations;
    dst->field_46 = src.field_46;
    dst->uNumMarkers = src.uNumMarkers;
    dst->field_4E = src.field_4E;
    dst->uNumLights = src.uNumLights;
    dst->field_56 = src.field_56;
    dst->uWaterLevel = src.uWaterLevel;
    dst->uMistLevel = src.uMistLevel;
    dst->uLightDistanceMultiplier = src.uLightDistanceMultiplier;
    dst->uMinAmbientLightLevel = src.uMinAmbientLightLevel;
    dst->uFirstBSPNode = src.uFirstBSPNode;
    dst->exit_tag = src.exit_tag;
    dst->pBounding = src.pBounding;
}

void serialize(const GUICharMetric &src, GUICharMetric_MM7 *dst) {
    memzero(dst);

    dst->uLeftSpacing = src.uLeftSpacing;
    dst->uWidth = src.uWidth;
    dst->uRightSpacing = src.uRightSpacing;
}

void deserialize(const GUICharMetric_MM7 &src, GUICharMetric *dst) {
    dst->uLeftSpacing = src.uLeftSpacing;
    dst->uWidth = src.uWidth;
    dst->uRightSpacing = src.uRightSpacing;
}

void serialize(const FontData &src, FontData_MM7 *dst) {
    memzero(dst);

    dst->cFirstChar = src.cFirstChar;
    dst->cLastChar = src.cLastChar;
    dst->field_2 = src.field_2;
    dst->field_3 = src.field_3;
    dst->field_4 = src.field_4;
    dst->uFontHeight = src.uFontHeight;
    dst->field_7 = src.field_7;
    dst->palletes_count = src.palletes_count;

    serialize(src.pMetrics, &dst->pMetrics);
    serialize(src.font_pixels_offset, &dst->font_pixels_offset);

    std::copy(src.pFontData.begin(), src.pFontData.end(), dst->pFontData);
}

void deserialize(const FontData_MM7 &src, size_t size, FontData *dst) {
    dst->cFirstChar = src.cFirstChar;
    dst->cLastChar = src.cLastChar;
    dst->field_2 = src.field_2;
    dst->field_3 = src.field_3;
    dst->field_4 = src.field_4;
    dst->uFontHeight = src.uFontHeight;
    dst->field_7 = src.field_7;
    dst->palletes_count = src.palletes_count;

    deserialize(src.pMetrics, &dst->pMetrics);
    deserialize(src.font_pixels_offset, &dst->font_pixels_offset);

    dst->pFontData.assign(src.pFontData, &src.pFontData[size - 4128]);
}

void deserialize(const ODMFace_MM7 &src, ODMFace *dst) {
    dst->facePlane.normal.x = src.facePlane.normal.x / 65536.0;
    dst->facePlane.normal.y = src.facePlane.normal.y / 65536.0;
    dst->facePlane.normal.z = src.facePlane.normal.z / 65536.0;
    dst->facePlane.dist = src.facePlane.dist / 65536.0;

    dst->zCalc.init(dst->facePlane);
    dst->uAttributes = FaceAttributes(src.attributes);
    dst->pVertexIDs = src.pVertexIDs;
    dst->pTextureUIDs = src.pTextureUIDs;
    dst->pTextureVIDs = src.pTextureVIDs;
    dst->pXInterceptDisplacements = src.pXInterceptDisplacements;
    dst->pYInterceptDisplacements = src.pYInterceptDisplacements;
    dst->pZInterceptDisplacements = src.pZInterceptDisplacements;
    dst->resource = nullptr;
    dst->sTextureDeltaU = src.sTextureDeltaU;
    dst->sTextureDeltaV = src.sTextureDeltaV;
    dst->pBoundingBox = src.pBoundingBox;
    dst->sCogNumber = src.sCogNumber;
    dst->sCogTriggeredID = src.sCogTriggeredID;
    dst->sCogTriggerType = src.sCogTriggerType;
    dst->field_128 = src.field_128;
    dst->field_129 = src.field_129;
    dst->uGradientVertex1 = src.uGradientVertex1;
    dst->uGradientVertex2 = src.uGradientVertex2;
    dst->uGradientVertex3 = src.uGradientVertex3;
    dst->uGradientVertex4 = src.uGradientVertex4;
    dst->uNumVertices = src.uNumVertices;
    dst->uPolygonType = static_cast<PolygonType>(src.uPolygonType);
    dst->uShadeType = src.uShadeType;
    dst->bVisible = src.bVisible;
    dst->field_132 = src.field_132;
    dst->field_133 = src.field_133;
}

void deserialize(const SpawnPoint_MM7 &src, SpawnPoint *dst) {
    dst->vPosition = src.vPosition;
    dst->uRadius = src.uRadius;
    dst->uKind = static_cast<ObjectType>(src.uKind);
    if (dst->uKind == OBJECT_Actor) {
        dst->uItemIndex = ITEM_TREASURE_LEVEL_INVALID;
        dst->uMonsterIndex = src.uIndex;
    } else {
        Assert(dst->uKind == OBJECT_Item);
        dst->uItemIndex = static_cast<ITEM_TREASURE_LEVEL>(src.uIndex);
        dst->uMonsterIndex = 0;
    }
    dst->uAttributes = src.uAttributes;
    dst->uGroup = src.uGroup;
}

void serialize(const SpriteObject &src, SpriteObject_MM7 *dst) {
    memzero(dst);

    dst->uType = src.uType;
    dst->uObjectDescID = src.uObjectDescID;
    dst->vPosition = src.vPosition;
    dst->vVelocity = src.vVelocity;
    dst->uFacing = src.uFacing;
    dst->uSoundID = src.uSoundID;
    dst->uAttributes = std::to_underlying(src.uAttributes);
    dst->uSectorID = src.uSectorID;
    dst->uSpriteFrameID = src.uSpriteFrameID;
    dst->tempLifetime = src.tempLifetime;
    dst->field_22_glow_radius_multiplier = src.field_22_glow_radius_multiplier;
    serialize(src.containing_item, &dst->containing_item);
    dst->uSpellID = std::to_underlying(src.uSpellID);
    dst->spell_level = src.spell_level;
    dst->spell_skill = std::to_underlying(src.spell_skill);
    dst->field_54 = src.field_54;
    dst->spell_caster_pid = src.spell_caster_pid;
    dst->spell_target_pid = src.spell_target_pid;
    dst->field_60_distance_related_prolly_lod = src.field_60_distance_related_prolly_lod;
    dst->field_61 = std::to_underlying(src.field_61);
    dst->field_62[0] = src.field_62[0];
    dst->field_62[1] = src.field_62[1];
    dst->initialPosition = src.initialPosition;
}

void deserialize(const SpriteObject_MM7 &src, SpriteObject *dst) {
    dst->uType = static_cast<SPRITE_OBJECT_TYPE>(src.uType);
    dst->uObjectDescID = src.uObjectDescID;
    dst->vPosition = src.vPosition;
    dst->vVelocity = src.vVelocity;
    dst->uFacing = src.uFacing;
    dst->uSoundID = src.uSoundID;
    dst->uAttributes = SpriteAttributes(src.uAttributes);
    dst->uSectorID = src.uSectorID;
    dst->uSpriteFrameID = src.uSpriteFrameID;
    dst->tempLifetime = src.tempLifetime;
    dst->field_22_glow_radius_multiplier = src.field_22_glow_radius_multiplier;
    deserialize(src.containing_item, &dst->containing_item);
    dst->uSpellID = static_cast<SPELL_TYPE>(src.uSpellID);
    dst->spell_level = src.spell_level;
    dst->spell_skill = static_cast<PLAYER_SKILL_MASTERY>(src.spell_skill);
    dst->field_54 = src.field_54;
    dst->spell_caster_pid = src.spell_caster_pid;
    dst->spell_target_pid = src.spell_target_pid;
    dst->field_60_distance_related_prolly_lod = src.field_60_distance_related_prolly_lod;
    dst->field_61 = static_cast<ABILITY_INDEX>(src.field_61);
    dst->field_62[0] = src.field_62[0];
    dst->field_62[1] = src.field_62[1];
    dst->initialPosition = src.initialPosition;
}

void deserialize(const ChestDesc_MM7 &src, ChestDesc *dst) {
    deserialize(src.pName, &dst->sName);
    dst->uWidth = src.uWidth;
    dst->uHeight = src.uHeight;
    dst->uTextureID = src.uTextureID;
}

void deserialize(const DecorationDesc_MM6 &src, DecorationDesc *dst) {
    deserialize(src.pName, &dst->pName);
    deserialize(src.field_20, &dst->field_20);
    dst->uType = src.uType;
    dst->uDecorationHeight = src.uDecorationHeight;
    dst->uRadius = src.uRadius;
    dst->uLightRadius = src.uLightRadius;
    dst->uSpriteID = src.uSpriteID;
    dst->uFlags = DecorationDescFlags(src.uFlags);
    dst->uSoundID = src.uSoundID;

    dst->uColoredLightRed = 255;
    dst->uColoredLightGreen = 255;
    dst->uColoredLightBlue = 255;
}

void deserialize(const DecorationDesc_MM7 &src, DecorationDesc *dst) {
    deserialize(static_cast<const DecorationDesc_MM6 &>(src), dst);

    dst->uColoredLightRed = src.uColoredLightRed;
    dst->uColoredLightGreen = src.uColoredLightGreen;
    dst->uColoredLightBlue = src.uColoredLightBlue;
}

void serialize(const Chest &src, Chest_MM7 *dst) {
    memzero(dst);

    dst->uChestBitmapID = src.uChestBitmapID;
    dst->uFlags = std::to_underlying(src.uFlags);
    serialize(src.igChestItems, &dst->igChestItems);
    serialize(src.pInventoryIndices, &dst->pInventoryIndices);
}

void deserialize(const Chest_MM7 &src, Chest *dst) {
    dst->uChestBitmapID = src.uChestBitmapID;
    dst->uFlags = ChestFlags(src.uFlags);
    deserialize(src.igChestItems, &dst->igChestItems);
    deserialize(src.pInventoryIndices, &dst->pInventoryIndices);
}

void deserialize(const BLVLight_MM7 &src, BLVLight *dst) {
    dst->vPosition = src.vPosition;
    dst->uRadius = src.uRadius;
    dst->uRed = src.uRed;
    dst->uGreen = src.uGreen;
    dst->uBlue = src.uBlue;
    dst->uType = src.uType;
    dst->uAtributes = src.uAtributes;
    dst->uBrightness = src.uBrightness;
}

void deserialize(const OverlayDesc_MM7 &src, OverlayDesc *dst) {
    dst->uOverlayID = src.uOverlayID;
    dst->uOverlayType = src.uOverlayType;
    dst->uSpriteFramesetID = src.uSpriteFramesetID;
    dst->spriteFramesetGroup = src.spriteFramesetGroup;
}

void deserialize(const PlayerFrame_MM7 &src, PlayerFrame *dst) {
    dst->expression = static_cast<CHARACTER_EXPRESSION_ID>(src.expression);
    dst->uTextureID = src.uTextureID;
    dst->uAnimTime = src.uAnimTime;
    dst->uAnimLength = src.uAnimLength;
    dst->uFlags = src.uFlags;
}

void deserialize(const LevelDecoration_MM7 &src, LevelDecoration *dst) {
    dst->uDecorationDescID = src.uDecorationDescID;
    dst->uFlags = LevelDecorationFlags(src.uFlags);
    dst->vPosition = src.vPosition;
    dst->_yawAngle = src._yawAngle;
    dst->uCog = src.uCog;
    dst->uEventID = src.uEventID;
    dst->uTriggerRange = src.uTriggerRange;
    dst->field_1A = src.field_1A;
    dst->eventVarId = src.eventVarId - 75; // Was changed because all current usages are without this 75 shift
    dst->field_1E = src.field_1E;
}

void deserialize(const BLVFaceExtra_MM7 &src, BLVFaceExtra *dst) {
    dst->field_0 = src.field_0;
    dst->field_2 = src.field_2;
    dst->field_4 = src.field_4;
    dst->field_6 = src.field_6;
    dst->field_8 = src.field_8;
    dst->field_A = src.field_A;
    dst->face_id = src.face_id;
    dst->uAdditionalBitmapID = src.uAdditionalBitmapID;
    dst->field_10 = src.field_10;
    dst->field_12 = src.field_12;
    dst->sTextureDeltaU = src.sTextureDeltaU;
    dst->sTextureDeltaV = src.sTextureDeltaV;
    dst->sCogNumber = src.sCogNumber;
    dst->uEventID = src.uEventID;
    dst->field_1C = src.field_1C;
    dst->field_1E = src.field_1E;
    dst->field_20 = src.field_20;
    dst->field_22 = src.field_22;
}

void deserialize(const BSPNode_MM7 &src, BSPNode *dst) {
    dst->uFront = src.uFront;
    dst->uBack = src.uBack;
    dst->uBSPFaceIDOffset = src.uBSPFaceIDOffset;
    dst->uNumBSPFaces = src.uNumBSPFaces;
}

void deserialize(const BLVMapOutline_MM7 &src, BLVMapOutline *dst) {
    dst->uVertex1ID = src.uVertex1ID;
    dst->uVertex2ID = src.uVertex2ID;
    dst->uFace1ID = src.uFace1ID;
    dst->uFace2ID = src.uFace2ID;
    dst->sZ = src.sZ;
    dst->uFlags = src.uFlags;
}

void deserialize(const ObjectDesc_MM6 &src, ObjectDesc *dst) {
    dst->field_0 =  src.field_0;
    dst->uObjectID = src.uObjectID;
    dst->uRadius = src.uRadius;
    dst->uHeight = src.uHeight;
    dst->uFlags = ObjectDescFlags(src.uFlags);
    dst->uSpriteID = src.uSpriteID;
    dst->uLifetime = src.uLifetime;
    dst->uParticleTrailColor = src.uParticleTrailColor;
    dst->uSpeed = src.uSpeed;
    dst->uParticleTrailColorR = src.uParticleTrailColorR;
    dst->uParticleTrailColorG = src.uParticleTrailColorG;
    dst->uParticleTrailColorB = src.uParticleTrailColorB;
}

void deserialize(const ObjectDesc_MM7 &src, ObjectDesc *dst) {
    dst->field_0 = src.field_0;
    dst->uObjectID = src.uObjectID;
    dst->uRadius = src.uRadius;
    dst->uHeight = src.uHeight;
    dst->uFlags = ObjectDescFlags(src.uFlags);
    dst->uSpriteID = src.uSpriteID;
    dst->uLifetime = src.uLifetime;
    dst->uParticleTrailColor = src.uParticleTrailColor;
    dst->uSpeed = src.uSpeed;
    dst->uParticleTrailColorR = src.uParticleTrailColorR;
    dst->uParticleTrailColorG = src.uParticleTrailColorG;
    dst->uParticleTrailColorB = src.uParticleTrailColorB;
}

void serialize(const LocationTime &src, LocationTime_MM7 *dst) {
    memzero(dst);

    serialize(src.last_visit, &dst->last_visit);
    serialize(src.sky_texture_name, &dst->sky_texture_name);
    dst->day_attrib = src.day_attrib;
    dst->day_fogrange_1 = src.day_fogrange_1;
    dst->day_fogrange_2 = src.day_fogrange_2;
}

void deserialize(const LocationTime_MM7 &src, LocationTime *dst) {
    deserialize(src.last_visit, &dst->last_visit);
    deserialize(src.sky_texture_name, &dst->sky_texture_name);
    dst->day_attrib = src.day_attrib;
    dst->day_fogrange_1 = src.day_fogrange_1;
    dst->day_fogrange_2 = src.day_fogrange_2;
}

void deserialize(const SoundInfo_MM6 &src, SoundInfo *dst) {
    deserialize(src.pSoundName, &dst->sName);
    dst->uSoundID = src.uSoundID;
    dst->eType = static_cast<SOUND_TYPE>(src.eType);
    dst->uFlags = src.uFlags;
}

void deserialize(const SoundInfo_MM7 &src, SoundInfo *dst) {
    deserialize(static_cast<const SoundInfo_MM6 &>(src), dst);
}

void serialize(const LocationInfo &src, LocationInfo_MM7 *dst) {
    dst->respawnCount = src.respawnCount;
    dst->lastRespawnDay = src.lastRespawnDay;
    dst->reputation = src.reputation;
    dst->alertStatus = src.alertStatus;
}

void deserialize(const LocationInfo_MM7 &src, LocationInfo *dst) {
    dst->respawnCount = src.respawnCount;
    dst->lastRespawnDay = src.lastRespawnDay;
    dst->reputation = src.reputation;
    dst->alertStatus = src.alertStatus;
}

void serialize(const PersistentVariables &src, MapEventVariables_MM7 *dst) {
    memzero(dst);

    dst->mapVars = src.mapVars;
    dst->decorVars = src.decorVars;
}

void deserialize(const MapEventVariables_MM7 &src, PersistentVariables *dst) {
    dst->mapVars = src.mapVars;
    dst->decorVars = src.decorVars;
}

void deserialize(const OutdoorLocationTileType_MM7 &src, OutdoorLocationTileType *dst) {
    dst->tileset = static_cast<Tileset>(src.tileset);
    dst->uTileID = src.tileId;
}

void serialize(const SaveGameHeader &src, SaveGameHeader_MM7 *dst) {
    memzero(dst);

    serialize(src.name, &dst->name);
    serialize(src.locationName, &dst->locationName);
    serialize(src.playingTime, &dst->playingTime);
}

void deserialize(const SaveGameHeader_MM7 &src, SaveGameHeader *dst) {
    deserialize(src.name, &dst->name);
    deserialize(src.locationName, &dst->locationName);
    deserialize(src.playingTime, &dst->playingTime);
    // field_30 is ignored.
}
