#pragma once

#include <string>
#include <vector>

#include "GUI/GUIWindow.h"
#include "Utility/IndexedArray.h"
#include "UIHouseEnums.h"

// Right hand side dialogue writing constants
constexpr int SIDE_TEXT_BOX_WIDTH = 143;
constexpr int SIDE_TEXT_BOX_POS_X = 483;
constexpr int SIDE_TEXT_BOX_POS_Z = 334;
constexpr int SIDE_TEXT_BOX_POS_Y = 113;
constexpr int SIDE_TEXT_BOX_BODY_TEXT_HEIGHT = 174;
constexpr int SIDE_TEXT_BOX_BODY_TEXT_OFFSET = 138;
constexpr int SIDE_TEXT_BOX_MAX_SPACING = 32;

bool HouseUI_CheckIfPlayerCanInteract();
void PlayHouseSound(unsigned int uHouseID, HouseSoundID sound);  // idb
void SimpleHouseDialog();
void OnSelectShopDialogueOption(DIALOGUE_TYPE option);
void PrepareHouse(HOUSE_ID house);  // idb

void createHouseUI(HOUSE_ID houseId);

/**
 * @offset 0x44622E
 */
bool enterHouse(HOUSE_ID uHouseID);
void BackToHouseMenu();

int HouseDialogPressCloseBtn();

void GetHouseGoodbyeSpeech();

class GUIWindow_House : public GUIWindow {
 public:
    explicit GUIWindow_House(HOUSE_ID houseId);
    virtual ~GUIWindow_House() {}

    virtual void Update();
    virtual void Release();

    HOUSE_ID houseId() const {
        return static_cast<HOUSE_ID>(wData.val); // TODO(captainurist): drop all direct accesses to wData.val.
    }

    void houseDialogManager();
    void initializeDialog();
    void learnSelectedSkill(PLAYER_SKILL_TYPE skill);

    virtual void houseDialogueOptionSelected(DIALOGUE_TYPE option);
    // TODO(Nik-RE-dev): add DIALOGUE_TYPE argument?
    virtual void houseSpecificDialogue();
    virtual std::vector<DIALOGUE_TYPE> listDialogueOptions(DIALOGUE_TYPE option);
};

// Originally was a packed struct.
struct HouseAnimDescr {
    const char *video_name;
    int field_4;
    int house_npc_id;
    BuildingType uBuildingType; // Originally was 1 byte.
    uint8_t uRoomSoundId;
    uint16_t padding_e;
};

extern int uHouse_ExitPic;
extern int dword_591080;
extern BuildingType in_current_building_type;  // 00F8B198
extern DIALOGUE_TYPE dialog_menu_id;     // 00F8B19C

extern class GraphicsImage *_591428_endcap;

extern std::array<const HouseAnimDescr, 196> pAnimatedRooms;

extern IndexedArray<int, BuildingType_WeaponShop, BuildingType_DarkGuild> itemAmountInShop;
