#include "GUI/UI/Houses/Bank.h"

#include "GUI/UI/UIStatusBar.h"
#include "GUI/GUIFont.h"
#include "GUI/GUIButton.h"

#include "Engine/Localization.h"

#include "Io/KeyboardActionMapping.h"

using Io::TextInputType;

void GUIWindow_Bank::mainDialogue() {
    GUIWindow bank_window = *this;
    bank_window.uFrameX = SIDE_TEXT_BOX_POS_X;
    bank_window.uFrameWidth = SIDE_TEXT_BOX_WIDTH;
    bank_window.uFrameZ = SIDE_TEXT_BOX_POS_Z;
    bank_window.DrawTitleText(pFontArrus, 0, 220, colorTable.PaleCanary,
                              fmt::format("{}: {}", localization->GetString(LSTR_BALANCE), pParty->uNumGoldInBank), 3);

    Color pColorText = colorTable.PaleCanary;
    if (pDialogueWindow->pCurrentPosActiveItem != 2) {
        pColorText = colorTable.White;
    }
    bank_window.DrawTitleText(pFontArrus, 0, 146, pColorText, localization->GetString(LSTR_DEPOSIT), 3);
    pColorText = colorTable.PaleCanary;
    if (pDialogueWindow->pCurrentPosActiveItem != 3) {
        pColorText = colorTable.White;
    }
    bank_window.DrawTitleText(pFontArrus, 0, 176, pColorText, localization->GetString(LSTR_WITHDRAW), 3);
}

void GUIWindow_Bank::putGoldDialogue() {
    GUIWindow bank_window = *this;
    bank_window.uFrameX = SIDE_TEXT_BOX_POS_X;
    bank_window.uFrameWidth = SIDE_TEXT_BOX_WIDTH;
    bank_window.uFrameZ = SIDE_TEXT_BOX_POS_Z;
    bank_window.DrawTitleText(pFontArrus, 0, 220, colorTable.PaleCanary,
                              fmt::format("{}: {}", localization->GetString(LSTR_BALANCE), pParty->uNumGoldInBank), 3);

    if (keyboard_input_status == WINDOW_INPUT_IN_PROGRESS) {
        bank_window.DrawTitleText(pFontArrus, 0, 146, colorTable.PaleCanary,
                                  fmt::format("{}\n{}", localization->GetString(LSTR_DEPOSIT), localization->GetString(LSTR_HOW_MUCH)), 3);
        bank_window.DrawTitleText(pFontArrus, 0, 186, colorTable.White, keyboardInputHandler->GetTextInput(), 3);
        bank_window.DrawFlashingInputCursor(pFontArrus->GetLineWidth(keyboardInputHandler->GetTextInput()) / 2 + 80, 185, pFontArrus);
        return;
    }
    if (keyboard_input_status == WINDOW_INPUT_CONFIRMED) {
        int sum = atoi(keyboardInputHandler->GetTextInput().c_str());
        if (sum <= 0) {
            pCurrentFrameMessageQueue->AddGUIMessage(UIMSG_Escape, 1, 0);
            return;
        }

        int party_gold = pParty->GetGold();
        if (sum > party_gold) {
            PlayHouseSound(wData.val, HouseSound_NotEnoughMoney);
            sum = party_gold;
        }

        if (sum > 0) {
            pParty->TakeGold(sum);
            pParty->AddBankGold(sum);
            if (pParty->hasActiveCharacter()) {
                pParty->activeCharacter().playReaction(SPEECH_BankDeposit);
            }
        }
    }
    keyboard_input_status = WINDOW_INPUT_NONE;
    pCurrentFrameMessageQueue->AddGUIMessage(UIMSG_Escape, 1, 0);
}

void GUIWindow_Bank::getGoldDialogue() {
    GUIWindow bank_window = *this;
    bank_window.uFrameX = SIDE_TEXT_BOX_POS_X;
    bank_window.uFrameWidth = SIDE_TEXT_BOX_WIDTH;
    bank_window.uFrameZ = SIDE_TEXT_BOX_POS_Z;
    bank_window.DrawTitleText(pFontArrus, 0, 220, colorTable.PaleCanary,
                              fmt::format("{}: {}", localization->GetString(LSTR_BALANCE), pParty->uNumGoldInBank), 3);

    if (keyboard_input_status == WINDOW_INPUT_IN_PROGRESS) {
        bank_window.DrawTitleText(pFontArrus, 0, 146, colorTable.PaleCanary,
                                  fmt::format("{}\n{}", localization->GetString(LSTR_WITHDRAW), localization->GetString(LSTR_HOW_MUCH)), 3);
        bank_window.DrawTitleText(pFontArrus, 0, 186, colorTable.White, keyboardInputHandler->GetTextInput(), 3);
        bank_window.DrawFlashingInputCursor(pFontArrus->GetLineWidth(keyboardInputHandler->GetTextInput()) / 2 + 80, 185, pFontArrus);
        return;
    } else if (keyboard_input_status == WINDOW_INPUT_CONFIRMED) {
        keyboard_input_status = WINDOW_INPUT_NONE;
        int sum = atoi(keyboardInputHandler->GetTextInput().c_str());
        if (sum <= 0) {
            pCurrentFrameMessageQueue->AddGUIMessage(UIMSG_Escape, 1, 0);
            return;
        }

        int bank_gold = pParty->GetBankGold();
        if (sum > bank_gold) {
            PlayHouseSound(wData.val, HouseSound_NotEnoughMoney);
            sum = bank_gold;
        }

        if (sum > 0) {
            pParty->TakeBankGold(sum);
            pParty->AddGold(sum);
        }
    }
    keyboard_input_status = WINDOW_INPUT_NONE;
    pCurrentFrameMessageQueue->AddGUIMessage(UIMSG_Escape, 1, 0);
}

void GUIWindow_Bank::houseDialogueOptionSelected(DIALOGUE_TYPE option) {
    if (option == DIALOGUE_BANK_PUT_GOLD || option == DIALOGUE_BANK_GET_GOLD) {
        keyboardInputHandler->StartTextInput(TextInputType::Number, 10, this);
    }
}

void GUIWindow_Bank::houseSpecificDialogue() {
    switch (dialog_menu_id) {
      case DIALOGUE_MAIN:
        mainDialogue();
        break;
      case DIALOGUE_BANK_PUT_GOLD:
        putGoldDialogue();
        break;
      case DIALOGUE_BANK_GET_GOLD:
        getGoldDialogue();
        break;
      default:
        break;
    }
}

std::vector<DIALOGUE_TYPE> GUIWindow_Bank::listDialogueOptions(DIALOGUE_TYPE option) {
    switch (option) {
      case DIALOGUE_MAIN:
        return {DIALOGUE_BANK_PUT_GOLD, DIALOGUE_BANK_GET_GOLD};
      default:
        return {};
    }
}
