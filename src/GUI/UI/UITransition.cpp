#include <stdlib.h>

#include "Engine/Engine.h"
#include "Engine/Tables/BuildingTable.h"
#include "Engine/Graphics/Outdoor.h"
#include "Engine/Graphics/Indoor.h"
#include "Engine/Graphics/IRender.h"
#include "Engine/LOD.h"
#include "Engine/Localization.h"
#include "Engine/MapInfo.h"
#include "Engine/Party.h"
#include "Engine/Time.h"
#include "Engine/Tables/TransitionTable.h"

#include "GUI/GUIButton.h"
#include "GUI/UI/UITransition.h"
#include "GUI/GUIFont.h"
#include "GUI/UI/UIGame.h"
#include "GUI/UI/UIHouses.h"
#include "GUI/UI/UIDialogue.h"

#include "Media/Audio/AudioPlayer.h"
#include "Media/MediaPlayer.h"

GraphicsImage *transition_ui_icon = nullptr;

std::string transition_button_label;

void GUIWindow_Travel::Release() {
    // -----------------------------------------
    // 0041C26A void GUIWindow::Release --- part
    if (transition_ui_icon) {
        transition_ui_icon->Release();
        transition_ui_icon = nullptr;
    }

    if (game_ui_dialogue_background) {
        game_ui_dialogue_background->Release();
        game_ui_dialogue_background = nullptr;
    }

    current_screen_type = prev_screen_type;

    GUIWindow::Release();
}

void GUIWindow_Transition::Release() {
    // -----------------------------------------
    // 0041C26A void GUIWindow::Release --- part
    // pVideoPlayer->Unload();
    if (transition_ui_icon) {
        transition_ui_icon->Release();
        transition_ui_icon = nullptr;
    }

    if (game_ui_dialogue_background) {
        game_ui_dialogue_background->Release();
        game_ui_dialogue_background = nullptr;
    }

    current_screen_type = prev_screen_type;

    GUIWindow::Release();
}

//----- (00444839) --------------------------------------------------------
GUIWindow_Transition::GUIWindow_Transition(uint anim_id, uint exit_pic_id,
                                           int x, int y, int z, int directiony,
                                           int directionx, int a8,
                                           const std::string &locationName)
    : GUIWindow(WINDOW_Transition, {0, 0}, render->GetRenderDimensions(), 0) {
    Party_Teleport_X_Pos = x;
    Party_Teleport_Y_Pos = y;
    Party_Teleport_Z_Pos = z;
    Party_Teleport_Cam_Yaw = directiony;
    Party_Teleport_Cam_Pitch = directionx;
    Party_Teleport_Z_Speed = a8;
    Party_Teleport_Map_Name = locationName;
    uCurrentHouse_Animation = anim_id;
    pEventTimer->Pause();
    current_screen_type = CURRENT_SCREEN::SCREEN_CHANGE_LOCATION;

    mapid = pMapStats->GetMapInfo(pCurrentMapName);
    _mapName = locationName;

    game_ui_dialogue_background = assets->getImage_Solid(DialogueBackgroundResourceByAlignment[pParty->alignment]);

    transition_ui_icon = assets->getImage_Solid(pHouse_ExitPictures[exit_pic_id]);

    // animation or special transfer message
    if (anim_id || IndoorLocation::GetLocationIndex(locationName)) {
        if (!IndoorLocation::GetLocationIndex(locationName))
            pMediaPlayer->OpenHouseMovie(pAnimatedRooms[buildingTable[anim_id - 1].uAnimationID].video_name, 1);

        std::string v15 = locationName;
        if (locationName[0] == '0') {
            v15 = pCurrentMapName;
        }
        if (pMapStats->GetMapInfo(v15)) {
            transition_button_label = localization->FormatString(LSTR_FMT_ENTER_S, pMapStats->pInfos[pMapStats->GetMapInfo(v15)].pName.c_str());
            if (uCurrentlyLoadedLevelType == LEVEL_INDOOR && pParty->hasActiveCharacter() && pParty->GetRedOrYellowAlert())
                pParty->activeCharacter().playReaction(SPEECH_LeaveDungeon);
            if (IndoorLocation::GetLocationIndex(locationName))
                uCurrentHouse_Animation = IndoorLocation::GetLocationIndex(locationName);
        } else {
            transition_button_label = localization->FormatString(LSTR_FMT_ENTER_S, pMapStats->pInfos[pMapStats->GetMapInfo(v15)].pName.c_str());
            if (pAnimatedRooms[buildingTable[anim_id].uAnimationID].uRoomSoundId)
                PlayHouseSound(anim_id, HouseSound_Greeting);
            if (uCurrentlyLoadedLevelType == LEVEL_INDOOR && pParty->hasActiveCharacter() && pParty->GetRedOrYellowAlert())
                pParty->activeCharacter().playReaction(SPEECH_LeaveDungeon);
            if (IndoorLocation::GetLocationIndex(locationName))
                uCurrentHouse_Animation = IndoorLocation::GetLocationIndex(locationName);
        }
    } else if (!IndoorLocation::GetLocationIndex(locationName)) { // transfer to outdoors - no special message
        if (pMapStats->GetMapInfo(pCurrentMapName)) {
            transition_button_label = localization->FormatString(LSTR_FMT_LEAVE_S, pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)].pName.c_str());
            if (pAnimatedRooms[buildingTable[anim_id].uAnimationID].uRoomSoundId)
                PlayHouseSound(anim_id, HouseSound_Greeting);
            if (uCurrentlyLoadedLevelType == LEVEL_INDOOR && pParty->hasActiveCharacter() && pParty->GetRedOrYellowAlert())
                pParty->activeCharacter().playReaction(SPEECH_LeaveDungeon);
            if (IndoorLocation::GetLocationIndex(locationName))
                uCurrentHouse_Animation = IndoorLocation::GetLocationIndex(locationName);
        } else {
            transition_button_label = localization->GetString(LSTR_DIALOGUE_EXIT);
            if ( pAnimatedRooms[buildingTable[anim_id].uAnimationID].uRoomSoundId)
                PlayHouseSound(anim_id, HouseSound_Greeting);
            if (uCurrentlyLoadedLevelType == LEVEL_INDOOR && pParty->hasActiveCharacter() && pParty->GetRedOrYellowAlert())
                pParty->activeCharacter().playReaction(SPEECH_LeaveDungeon);
            if (IndoorLocation::GetLocationIndex(locationName))
                uCurrentHouse_Animation = IndoorLocation::GetLocationIndex(locationName);
        }
    }

    std::string hint = this->sHint = transition_button_label;

    prev_screen_type = current_screen_type;
    current_screen_type = CURRENT_SCREEN::SCREEN_INPUT_BLV;
    pBtn_ExitCancel = CreateButton({556, 0x1BDu}, {0x4Bu, 0x21u}, 1, 0, UIMSG_TransitionWindowCloseBtn, 0,
                                   InputAction::No, localization->GetString(LSTR_CANCEL), {ui_buttdesc2});
    pBtn_YES = CreateButton({476, 0x1BDu}, {0x4Bu, 0x21u}, 1, 0, UIMSG_TransitionUI_Confirm, 0, InputAction::Yes, hint, {ui_buttyes2});
    CreateButton({pNPCPortraits_x[0][0], pNPCPortraits_y[0][0]}, {0x3Fu, 0x49u}, 1, 0, UIMSG_TransitionUI_Confirm, 1, InputAction::EventTrigger, hint);
    CreateButton({8, 8}, {0x1CCu, 0x158u}, 1, 0, UIMSG_TransitionUI_Confirm, 1u, InputAction::Invalid, hint);
}

GUIWindow_Travel::GUIWindow_Travel() : GUIWindow(WINDOW_ChangeLocation, {0, 0}, render->GetRenderDimensions(), 0) {
    pEventTimer->Pause();

    game_ui_dialogue_background = assets->getImage_Solid(DialogueBackgroundResourceByAlignment[pParty->alignment]);

    transition_ui_icon = assets->getImage_Solid("outside");
    if (pMapStats->GetMapInfo(pCurrentMapName)) {
        transition_button_label = localization->FormatString( LSTR_FMT_LEAVE_S, pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)].pName.c_str());
    } else {
        transition_button_label = localization->GetString(LSTR_DIALOGUE_EXIT);
    }

    std::string hint = this->sHint = transition_button_label;

    prev_screen_type = current_screen_type;
    current_screen_type = CURRENT_SCREEN::SCREEN_CHANGE_LOCATION;
    pBtn_ExitCancel = CreateButton({566, 445}, {75, 33}, 1, 0, UIMSG_CHANGE_LOCATION_ClickCancelBtn, 0, InputAction::No,
                                   localization->GetString(LSTR_STAY_IN_THIS_AREA), {ui_buttdesc2});
    pBtn_YES = CreateButton({486, 445}, {75, 33}, 1, 0, UIMSG_OnTravelByFoot, 0, InputAction::Yes, hint, {ui_buttyes2});
    CreateButton({pNPCPortraits_x[0][0], pNPCPortraits_y[0][0]}, {63, 73}, 1, 0, UIMSG_OnTravelByFoot, 1, InputAction::EventTrigger, hint);
    CreateButton({8, 8}, {460, 344}, 1, 0, UIMSG_OnTravelByFoot, 1, InputAction::Invalid, hint);
}

void GUIWindow_Travel::Update() {
    std::string pDestinationMapName;

    pOutdoor->GetTravelDestination(pParty->vPosition.x, pParty->vPosition.y, &pDestinationMapName);
    render->DrawTextureNew(477 / 640.0f, 0, game_ui_dialogue_background);
    render->DrawTextureNew(468 / 640.0f, 0, game_ui_right_panel_frame);
    render->DrawTextureNew(pNPCPortraits_x[0][0] / 640.0f, pNPCPortraits_y[0][0] / 480.0f, transition_ui_icon);
    render->DrawTextureNew(556 / 640.0f, 451 / 480.0f, dialogue_ui_x_x_u);
    render->DrawTextureNew(476 / 640.0f, 451 / 480.0f, dialogue_ui_x_ok_u);
    if (pMapStats->GetMapInfo(pDestinationMapName)) {
        GUIWindow travel_window = *pPrimaryWindow;
        travel_window.uFrameX = 493;
        travel_window.uFrameWidth = 126;
        travel_window.uFrameZ = 366;
        travel_window.DrawTitleText(pFontCreate, 0, 4, Color(), pMapStats->pInfos[pMapStats->GetMapInfo(pDestinationMapName)].pName, 3);
        travel_window.uFrameX = SIDE_TEXT_BOX_POS_X;
        travel_window.uFrameWidth = SIDE_TEXT_BOX_WIDTH;
        travel_window.uFrameZ = SIDE_TEXT_BOX_POS_Z;

        std::string str;
        if (getTravelTime() == 1) {
            str = localization->FormatString(LSTR_FMT_IT_TAKES_D_DAY_TO_S, 1, pMapStats->pInfos[pMapStats->GetMapInfo(pDestinationMapName)].pName.c_str());
        } else {
            str = localization->FormatString(LSTR_FMT_IT_TAKES_D_DAYS_TO_S, getTravelTime(), pMapStats->pInfos[pMapStats->GetMapInfo(pDestinationMapName)].pName.c_str());
        }
        str += "\n \n";
        str += localization->FormatString(LSTR_FMT_DO_YOU_WISH_TO_LEAVE_S, pMapStats->pInfos[pMapStats->GetMapInfo(pCurrentMapName)].pName.c_str());

        travel_window.DrawTitleText(pFontCreate, 0, (212 - pFontCreate->CalcTextHeight(str, travel_window.uFrameWidth, 0)) / 2 + 101, Color(), str, 3);
    }
}

void GUIWindow_Transition::Update() {
    render->DrawTextureNew(477 / 640.0f, 0, game_ui_dialogue_background);
    render->DrawTextureNew((pNPCPortraits_x[0][0] - 4) / 640.0f, (pNPCPortraits_y[0][0] - 4) / 480.0f, game_ui_evtnpc);
    render->DrawTextureNew(pNPCPortraits_x[0][0] / 640.0f, pNPCPortraits_y[0][0] / 480.0f, transition_ui_icon);
    render->DrawTextureNew(468 / 640.0f, 0, game_ui_right_panel_frame);
    render->DrawTextureNew(556 / 640.0f, 451 / 480.0f, dialogue_ui_x_x_u);
    render->DrawTextureNew(476 / 640.0f, 451 / 480.0f, dialogue_ui_x_ok_u);

    unsigned int map_id = mapid;
    if ((pMovie_Track || IndoorLocation::GetLocationIndex(_mapName)) && Party_Teleport_Map_Name[0] != ' ') {
        map_id = pMapStats->GetMapInfo(_mapName);
    }

    GUIWindow transition_window = *pPrimaryWindow;
    transition_window.uFrameX = 493;
    transition_window.uFrameWidth = 126;
    transition_window.uFrameZ = 366;
    transition_window.DrawTitleText(pFontCreate, 0, 5, Color(), pMapStats->pInfos[map_id].pName, 3);
    transition_window.uFrameX = SIDE_TEXT_BOX_POS_X;
    transition_window.uFrameWidth = SIDE_TEXT_BOX_WIDTH;
    transition_window.uFrameZ = SIDE_TEXT_BOX_POS_Z;

    if (uCurrentHouse_Animation) {
        unsigned int vertMargin = (212 - pFontCreate->CalcTextHeight(pTransitionStrings[uCurrentHouse_Animation], transition_window.uFrameWidth, 0)) / 2 + 101;
        transition_window.DrawTitleText(pFontCreate, 0, vertMargin, Color(), pTransitionStrings[uCurrentHouse_Animation], 3);
    } else if (map_id) {
        std::string str = localization->FormatString(LSTR_FMT_DO_YOU_WISH_TO_LEAVE_S_2, pMapStats->pInfos[map_id].pName.c_str());
        unsigned int vertMargin = (212 - pFontCreate->CalcTextHeight(str, transition_window.uFrameWidth, 0)) / 2 + 101;
        transition_window.DrawTitleText(pFontCreate, 0, vertMargin, Color(), str, 3);
    } else {
        Error("Troubles in da house");
    }
}
