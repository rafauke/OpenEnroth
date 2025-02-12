#pragma once

#include <vector>

#include "GUI/UI/UIHouses.h"
#include "GUI/UI/UIHouseEnums.h"
#include "Engine/Objects/Player.h"

class GUIWindow_Temple : public GUIWindow_House {
 public:
    explicit GUIWindow_Temple(HOUSE_ID houseId);
    virtual ~GUIWindow_Temple() {}

    virtual void houseDialogueOptionSelected(DIALOGUE_TYPE option) override;
    virtual void houseSpecificDialogue() override;
    virtual std::vector<DIALOGUE_TYPE> listDialogueOptions(DIALOGUE_TYPE option) override;

 protected:
    void mainDialogue();
    void healDialogue();
    void donateDialogue();
    void learnSkillsDialogue();

    /**
     * @offset 0x4B6FF9
     */
    bool isPlayerHealableByTemple(const Player &player) const;

 private:
    std::vector<int> _templeSpellCounter;
};
