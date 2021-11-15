﻿#pragma once
#include "Engine/Strings.h"
#include "Engine/Objects/NPCProf.h"


#define LSTR_AC                          0   // "AC"
#define LSTR_ACCURACY                    1   // "Accuracy"
#define LSTR_AFRAID                      4   // "Afraid"
#define LSTR_AGE                         5   // "Age"
#define LSTR_AIR                         6   // "Air"
#define LSTR_OBELISK_NOTES               8   // "Obelisk Notes"
#define LSTR_CONGRATULATIONS             9   // "Congratulations!"
#define LSTR_TOWN_PORTAL                10   // "Town Portal"
#define LSTR_ARMOR                      11   // "Armor"
#define LSTR_ARMOR_CLASS                12   // "Armor Class"
#define LSTR_AUTOSAVE                   16   // "Autosave"
#define LSTR_EXP                        17   // "Exp."
#define LSTR_ATTACK                     18   // "Attack"
#define LSTR_AVAILABLE                  19   // "Available"
#define LSTR_AVAILABLE_SKILLS           20   // "Available Skills"
#define LSTR_AWARDS                     22   // "Awards"
#define LSTR_AWARDS_FOR                 23   // "Awards for"
#define LSTR_BALANCE                    25   // "Balance"
#define LSTR_BODY                       29   // "Body"
#define LSTR_BONUS                      30   // "Bonus"
#define LSTR_BROKEN_ITEM                32   // "Broken Item"
#define LSTR_CANCEL                     34   // "Cancel"
#define LSTR_TOTAL_TIME                 37   // "Total Time:"
#define LSTR_CAST_SPELL                 38   // "Cast Spell"
#define LSTR_CHOOSE_TARGET              39   // "Choose target"
#define LSTR_CLASS                      41   // "Class"
#define LSTR_COND                       45   // "Cond"
#define LSTR_CONDITION                  47   // "Condition"
#define LSTR_CREATE_PARTY_FANCY         51   // "C R E A T E   P A R T Y"
#define LSTR_DAMAGE                     53   // "Damage"
#define LSTR_DAWN                       55   // "Dawn"
#define LSTR_DAY                        56   // "Day"
#define LSTR_DAYS                       57   // "Days"
#define LSTR_DEPOSIT                    60   // "Deposit"
#define LSTR_PRESS_ESCAPE               61   // "Press Escape"
#define LSTR_PLEASE_REINSTALL           63   // "Might and Magic VII is having trouble loading files. Please re-install to fix this problem. Note: Re-installing will not destroy your save games."
#define LSTR_DETAIL_TOGGLE              64   // "Detail Toggle"
#define LSTR_DMG                        66   // "Dmg"
#define LSTR_DONATE                     68   // "Donate"
#define LSTR_EARTH                      70   // "Earth"
#define LSTR_EMPTY_SAVESLOT             72   // "Empty"
#define LSTR_ENTER                      73   // "Enter"
#define LSTR_END_CONVERSATION           74   // "End Conversation"
#define LSTR_ENDURANCE                  75   // "Endurance"
#define LSTR_CALENDAR                   78   // "Calendar"
#define LSTR_DIALOGUE_EXIT              79   // "Exit"
#define LSTR_EXIT_BUILDING              80   // "Exit Building"
#define LSTR_EXIT_REST                  81   // "Exit Rest"
#define LSTR_EXIT_GAME_PROMPT           82   // "Are you sure?  Click again to quit"
#define LSTR_EXPERIENCE                 83   // "Experience"
#define LSTR_FAME                       84   // "Fame"
#define LSTR_POTION_NOTES               85   // "Potion Notes"
#define LSTR_FIRE                       87   // "Fire"
#define LSTR_GAME_OPTIONS               93   // "Game Options"
#define LSTR_GRAND                      96   // "Grand"
#define LSTR_GOLD                       97   // "Gold"
#define LSTR_RACE_HUMAN                 99   // "Human"
#define LSTR_RACE_ELF                   101  // "Elf"
#define LSTR_RACE_DWARF                 103  // "Dwarf"
#define LSTR_HEAL                       104  // "Heal"
#define LSTR_RACE_GOBLIN                106  // "Goblin"
#define LSTR_HP                         107  // "HP"
#define LSTR_HIT_POINTS                 108  // "Hit Points"
#define LSTR_HOUR                       109  // "Hour"
#define LSTR_HOURS                      110  // "Hours"
#define LSTR_HOW_MUCH                   112  // "How Much?"
#define LSTR_IDENTIFY                   113  // "Identify"
#define LSTR_INTELLECT                  116  // "Intellect"
#define LSTR_WE_HOPE_YOU_ENJOYED_MM7    118  // "We hope that you've enjoyed playing Might and Magic VII as much as we did making it. We have saved this screen as MM7_WIN.PCX in your MM7 directory. You can print it out as proof of your accomplishment."
#define LSTR_INVENTORY                  120  // "Inventory"
#define LSTR_PERMANENT                  121  // "Permanent"
#define LSTR_JOIN                       122  // "Join"
#define LSTR_MISC_NOTES                 123  // "Miscellaneous Notes"
#define LSTR_NO_TEXT                    127  // "No Text!"
#define LSTR_LEVEL                      131  // "Level"
#define LSTR_YEARS                      132  // "Years"
#define LSTR_STANDARD                   134  // "Standard"
#define LSTR_LOADING                    135  // "Loading"
#define LSTR_LUCK                       136  // "Luck"
#define LSTR_FOUNTAIN_NOTES             137  // "Fountain Notes"
#define LSTR_MAGIC                      138  // "Magic"
#define LSTR_MAPS                       139  // "Maps"
#define LSTR_RATIONS_FULL               140  // "Your packs are already full!"
#define LSTR_SEER_NOTES                 141  // "Seer Notes"
#define LSTR_MIND                       142  // "Mind"
#define LSTR_MISC                       143  // "Misc"
#define LSTR_MIGHT                      144  // "Might"
#define LSTR_MONTH                      146  // "Month"
#define LSTR_MONTHS                     148  // "Months"
#define LSTR_ELIGIBLE_TO_LEVELUP        147  // "You are eligible to train to %u."
#define LSTR_NAME                       149  // "Name"
#define LSTR_CONGRATULATIONS_ADVENTURER 151  // "Congratulations Adventurer."
#define LSTR_SPECIAL                    152  // "Special"
#define LSTR_NONE                       153  // "None"
#define LSTR_AUTONOTES                  154  // "Autonotes"
#define LSTR_NOT_ENOUGH_GOLD            155  // "You don't have enough gold"
#define LSTR_STAY_IN_THIS_AREA          156  // "Stay in this Area"
#define LSTR_3DO_COPYRIGHT              157  // ""© 1999 The 3DO Company.All Rights
                                             // Reserved.Might and Magic, Bloodand Honor, New World Computing, 3DO, and their
                                             // respective logos, are trademarksand /or service marks of The 3DO Company in the
                                             // U.S. and other countries.All other trademarks belong to their respective
                                             // owners.New World Computing is a division of The 3DO Company.""
#define LSTR_DISPLAY                    159  // "Display"
#define LSTR_LEARN_SKILLS               160  // "Learn Skills"
#define LSTR_PARALYZED                  162  // "Paralyzed"
#define LSTR_PERSONALITY                163  // "Personality"
#define LSTR_PLEASE_WAIT                165  // "Please Wait"
#define LSTR_THE_MM7_DEV_TEAM           167  // "- The Might and Magic VII Development Team."
#define LSTR_POINTS                     168  // "Points"
#define LSTR_QSPELL                     170  // "QSpell"
#define LSTR_QUICK_SPELL                172  // "Quick Spell"
#define LSTR_QUICK_REFERENCE            173  // "Quick Reference"
#define LSTR_CURRENT_QUESTS             174  // "Current Quests"
#define LSTR_REPAIR                     179  // "Repair"
#define LSTR_REPUTATION                 180  // "Reputation"
#define LSTR_REST                       182  // "Rest"
#define LSTR_REINSTALL_NECESSARY        184  // "Reinstall Necessary"
#define LSTR_TIME_IN_ERATHIA            186  // "Time in Erathia"
#define LSTR_STEAL_ITEM_FMT             181  // "Steal %24"
#define LSTR_REST_AND_HEAL_8_HOURS      183  // "Rest & Heal 8 Hours"
#define LSTR_STEAL_ITEM                 185  // "Steal item"
#define LSTR_STOLEN                     187  // "Stolen"
#define LSTR_BANNED_FROM_SHOP           191  // "You've been banned from this shop!"
#define LSTR_SCROLL_UP                  192  // "Scroll Up"
#define LSTR_SCROLL_DOWN                193  // "Scroll Down"
#define LSTR_SELECT_ITEM_TO_BUY         195  // "Select the Item to Buy"
#define LSTR_SELECT_ITEM_TO_IDENTIFY    197  // "Select the Item to Identify"
#define LSTR_SELECT_ITEM_TO_REPAIR      198  // "Select the Item to Repair"
#define LSTR_SELECT_ITEM_TO_SELL        199  // "Select the Item to Sell"
#define LSTR_SELL                       200  // "Sell"
#define LSTR_START_NEW_GAME_PROMPT      201  // "Are you sure?  Click again to start a New Game"
#define LSTR_SHOOT                      203  // "Shoot"
#define LSTR_SKILLS                     205  // "Skills"
#define LSTR_SKILLS_FOR                 206  // "Skills for"
#define LSTR_SKILL_POINTS               207  // "Skill Points"
#define LSTR_SP                         209  // "SP"
#define LSTR_SPECIAL                    210  // "Special"
#define LSTR_SPEED                      211  // "Speed"
#define LSTR_SPELL_POINTS               212  // "Spell Points"
#define LSTR_STATS                      216  // "Stats"
#define LSTR_STONED                     220  // "Stoned"
#define LSTR_FATE                       221  // "Fate"
#define LSTR_GRANDMASTER                225  // "Grandmaster"
#define LSTR_GAMMA_DESCRIPTION          226  // ""Gamma controls the relative ""brightness"" of the game.May vary depending on your monitor.""
#define LSTR_HAMMERHANDS                228  // "Hammerhands"
#define LSTR_PAIN_REFLECTION            229  // "Pain Reflection"
#define LSTR_NOT_IDENTIFIED             232  // "Not Identified"
#define LSTR_WAIT_WITHOUT_HEALING       236  // "Wait without healing"
#define LSTR_WAIT_UNTIL_DAWN            237  // "Wait until Dawn"
#define LSTR_WAIT_5_MINUTES             238  // "Wait 5 Minutes"
#define LSTR_WAIT_1_HOUR                239  // "Wait 1 Hour"
#define LSTR_WATER                      240  // "Water"
#define LSTR_WEAPONS                    242  // "Weapons"
#define LSTR_WITHDRAW                   244  // "Withdraw"
#define LSTR_YEAR                       245  // "Year"
#define LSTR_ZOOM_IN                    251  // "Zoom In"
#define LSTR_ZOOM_OUT                   252  // "Zoom Out"
#define LSTR_ARCOMAGE_CARD_DISCARD      266  // "DISCARD A CARD"
#define LSTR_SHIELD                     279  // "Shield"
#define LSTR_SET_BEACON                 375  // "Set Beacon"
#define LSTR_REPUTATION_HATED           379  // "Hated"
#define LSTR_HIS                        383  // "his"
#define LSTR_HER                        384  // "her"
#define LSTR_SIR_LOWERCASE              385  // "sir"
#define LSTR_SIR                        386  // "Sir"
#define LSTR_LADY_LOWERCASE             387  // "lady"
#define LSTR_LORD                       388  // "Lord"
#define LSTR_LADY                       389  // "Lady"
#define LSTR_BROTHER                    390  // "brother"
#define LSTR_SISTER                     391  // "sister"
#define LSTR_REPUTATION_UNFRIENDLY      392  // "Unfriendly"
#define LSTR_DAUGHTER                   393  // "daughter"
#define LSTR_UNKNOWN                    394  // "Unknown"
#define LSTR_MORNING                    395  // "morning"
#define LSTR_DAY                        396  // "day"
#define LSTR_EVENING                    397  // "evening"
#define LSTR_REPUTATION_NEUTRAL         399  // "Neutral"
#define LSTR_BUY_SPELLS                 400  // "Buy Spells"
#define LSTR_REPUTATION_FRIENDLY        402  // "Friendly"
#define LSTR_HIRE                       406  // "Hire"
#define LSTR_MORE_INFORMATION           407  // "More Information"
#define LSTR_HIRE_RELEASE               408  // "Release %s"
#define LSTR_ENTER_FMT                  411  // "Enter %s"
#define LSTR_PARTY_UNASSIGNED_POINTS    412  // "Create Party cannot be completed unless you have assigned all characters 2 extra skills and have spent all of your bonus points."
#define LSTR_PARTY_TOO_MUCH_POINTS      413  // "You can't spend more than 50 points."
#define LSTR_SPELL_FAILED               428  // "Spell failed"
#define LSTR_NORMAL                     431  // "Normal"
#define LSTR_MASTER                     432  // "Master"
#define LSTR_EXPERT                     433  // "Expert"
#define LSTR_REPUTATION_RESPECTED       434  // "Respected"
#define LSTR_CONVERSE_WITH_FMT          435  // "Converse with %s"
#define LSTR_MINUTES                    436  // "Minutes"
#define LSTR_MINUTE                     437  // "Minute"
#define LSTR_SECONDS                    438  // "Seconds"
#define LSTR_SECOND                     439  // "Second"
#define LSTR_HEROISM                    440  // "Heroism"
#define LSTR_HASTE                      441  // "Haste"
#define LSTR_STONESKIN                  442  // "Stoneskin"
#define LSTR_BLESS                      443  // "Bless"
#define LSTR_OOPS                       444  // "Ooops!"
#define LSTR_IDENTIFY_FAILED            446  // "Identify Failed"
#define LSTR_REPAIR_FAILED              448  // "Repair Failed"
#define LSTR_POWER                      449  // "Power"
#define LSTR_ACTIVE_PARTY_SPELLS        451  // "Active Party Spells"
#define LSTR_CHARGES                    464  // "Charges"
#define LSTR_VALUE                      465  // "Value"
#define LSTR_ALREADY_RESTING            477  // "You are already resting!"
#define LSTR_CANT_REST_IN_TURN_BASED    478  // "You can't rest in turn-based mode!"
#define LSTR_CANT_REST_HERE             479  // "You can't rest here!"
#define LSTR_HOSTILE_ENEMIES_NEARBY     480  // "There are hostile enemies near!"
#define LSTR_ENCOUNTER                  481  // "Encounter!"
#define LSTR_NOT_ENOUGH_FOOD            482  // "You don't have enough food to rest"
#define LSTR_CLICK_TO_SET_QUICKSPELL    484  // "Select a spell then click here to set a QuickSpell"
#define LSTR_SKILL_ALREADY_MASTERED     487  // "You have already mastered this skill!"
#define LSTR_NOT_ENOUGH_SKILL_POINTS    488  // "You don't have enough skill points!"
#define LSTR_CANT_METEOR_SHOWER_INDOORS 491  // "Can't cast Meteor Shower indoors!"
#define LSTR_CANT_INFERNO_OUTDOORS      492  // "Can't cast Inferno outdoors!"
#define LSTR_CANT_JUMP_AIRBORNE         493  // "Can't cast Jump while airborne!"
#define LSTR_CANT_FLY_INDOORS           494  // "Can't fly indoors"
#define LSTR_CANT_STARBURST_INDOORS     495  // "Can't cast Starburst indoors!"
#define LSTR_NO_VALID_SPELL_TARGET      496  // "No valid target exists!"
#define LSTR_CANT_PRISMATIC_OUTDOORS    497  // "Can't cast Prismatic Light outdoors!"
#define LSTR_CANT_ARMAGEDDON_INDOORS    499  // "Can't cast Armageddon indoors!"
#define LSTR_READING                    505  // "Reading..."
#define LSTR_PC_NAME_RODERIC            506  // "Roderic"
#define LSTR_PC_NAME_ALEXIS             507  // "Alexis"
#define LSTR_PC_NAME_SERENA             508  // "Serena"
#define LSTR_PC_NAME_ZOLTAN             509  // "Zoltan"
#define LSTR_NOTHING_HERE               521  // "Nothing here"
#define LSTR_SP_COST                    522  // "SP Cost"
#define LSTR_RECALL_BEACON              523  // "Recall Beacon"
#define LSTR_CHEATED_THE_DEATH          524  // "Once again you've cheated death! ..."
#define LSTR_TIME                       526  // "Time"
#define LSTR_THANK_YOU                  527  // "Thank You!"
#define LSTR_NO_FURTHER_OFFERS          528  // "I can offer you nothing further."
#define LSTR_CANT_TRAIN_FURTHER         529  // ""Sorry, but we are unable to train you.""
#define LSTR_MOON                       530  // "Moon"
#define LSTR_LOCATION                   531  // "Location"
#define LSTR_PLEASE_TRY_BACK_IN         532  // "Please try back in "
#define LSTR_HIRE_NO_ROOM               533  // "I cannot join you, you're party is full"
#define LSTR_LEARN                      535  // "Learn"
#define LSTR_TEACHER_LEVEL_TOO_LOW      536  // ""With your skills, you should be working here as a teacher.""
#define LSTR_XP_UNTIL_NEXT_LEVEL        538  // "You need %d more experience to train to level %d"
#define LSTR_IDENTIFY_ITEMS             541  // "Identify Items"
#define LSTR_COME_BACK_ANOTHER_DAY      561  // ""Sorry, come back another day""
#define LSTR_DO_ANYTHING                562  // "do anythig"
#define LSTR_INVENTORY_IS_FULL          563  // "Pack is Full!"
#define LSTR_DUSK                       566  // "Dusk"
#define LSTR_NIGHT                      567  // "Night"
#define LSTR_DONE                       569  // "Done!"
#define LSTR_GOOD_AS_NEW                570  // "Good as New!"
#define LSTR_SCROLL_LEFT                572  // "Scroll Left"
#define LSTR_SCROLL_RIGHT               573  // "Scroll Right"
#define LSTR_ARENA_WELCOME              574  // "Welcome to the Arena of Life and Death.  Remember,
                                             // you are only allowed one arena combat per visit.  To
                                             // fight an arena battle, select the option that best
                                             // describes your abilities and return to me- if you
                                             // survive"
#define LSTR_PLEASE_WAIT_WHILE_I_SUMMON 575  // "Please wait while I summon the monsters.  Good luck."
#define LSTR_ARENA_REWARD               576  // "Congratulations on your win. Here's your stuff: %u gold."
#define LSTR_ARENA_PREMATURE_EXIT       577  // "Get back in there you wimps"
#define LSTR_ARENA_DIFFICULTY_PAGE      578  // "Page"
#define LSTR_ARENA_DIFFICULTY_SQUIRE    579  // "Squire"
#define LSTR_ARENA_DIFFICULTY_KNIGHT    580  // "Knight"
#define LSTR_ARENA_DIFFICULTY_LORD      581  // "Lord"
#define LSTR_ARENA_AREADY_WON           582  // "You already won this trip to the Arena
#define LSTR_NO_SAVING_IN_ARENA         583  // "No saving in the Arena"
#define LSTR_CLICK_TO_REMOVE_QUICKSPELL 584  // "Click here to remove your Quick Spell"
#define LSTR_ITEM_TOO_LAME              585  // "Item is not of high enough quality"
#define LSTR_NOT_ENOUGH_SPELLPOINTS     586  // "Not enough spell points"
#define LSTR_ATTACK_BONUS               587  // "Attack Bonus"
#define LSTR_ATTACK_DAMAGE              588  // "Attack Damage"
#define LSTR_SHOOT_BONUS                589  // "Shoot Bonus"
#define LSTR_SHOOT_DAMAGE               590  // "Shoot Damage"
#define LSTR_CHARMED                    591  // "Charmed"
#define LSTR_SHRUNK                     592  // "Shrunk"
#define LSTR_SLOWED                     593  // "Slowed"
#define LSTR_WAND                       595  // "Wand"
#define LSTR_HISTORY                    602  // "History"
#define LSTR_PAY_FINE                   603  // "Pay Fine"
#define LSTR_BOUNTY_HUNT                604  // "Bounty Hunt"
#define LSTR_CURRENT_FINE               605  // "Current Fine"
#define LSTR_PAY                        606  // "Pay"
#define LSTR_ENSLAVED                   607  // "Enslaved"
#define LSTR_BERSERK                    608  // "Berserk"
#define LSTR_HOUR_OF_POWER              609  // "Hour of Power"
#define LSTR_DAY_OF_PROTECTION          610  // "Day of Protection"
#define LSTR_PLAY_ARCOMAGE              611  // "Play ArcoMage"
#define LSTR_AUTOSAVE_MM7               613  // "AutoSave.MM7"
#define LSTR_NEW_GAME                   614  // "New Game"
#define LSTR_SAVE_GAME                  615  // "Save Game"
#define LSTR_LOAD_GAME                  616  // "Load Game"
#define LSTR_OPTIONS                    617  // ""Sound, Keyboard, Game Options...""
#define LSTR_QUIT                       618  // "Quit"
#define LSTR_RETURN_TO_GAME             619  // "Return to Game"
#define LSTR_RULES                      620  // "Rules"
#define LSTR_PLAY                       621  // "Play"
#define LSTR_VICTORY_CONDITIONS         622  // "Victory Conditions"
#define LSTR_BONUS_2                    623  // "Bonus"
#define LSTR_PHYSICAL                   624  // "Physical"
#define LSTR_IMMUNE                     625  // "Immune"
#define LSTR_RESISTANCES                626  // "Resistances"
#define LSTR_RESISTANT                  627  // "Resistant"
#define LSTR_SPELL                      628  // "Spell"
#define LSTR_SPELLS                     629  // "Spells"
#define LSTR_UNKNOWN_VALUE              630  // "?"
#define LSTR_EFFECTS                    631  // "Effects"
#define LSTR_HOSTILE_CREATURES_NEARBY   638  // "There are hostile creatures nearby!"
#define LSTR_A_TIE                      639  // "A tie!"
#define LSTR_YOU_WON                    640  // "You won!"
#define LSTR_YOU_LOST                   641  // "You lost!"
#define LSTR_SUMMONS_LIMIT_REACHED      648  // "This character can't summon any more monsters!"
#define LSTR_SUMMONED                   649  // "Summoned"
#define LSTR_CURRENT_HIT_POINTS         650  // "Current Hit Points"
#define LSTR_HARDENED                   651  // "Hardened"
#define LSTR_CANT_DO_UNDERWATER         652  // "You can not do that while you are underwater!"
#define LSTR_FOOD                       653  // "Food"
#define LSTR_GAME_SAVED                 656  // "Game Saved!"
#define LSTR_COLLECT_PRIZE              658  // "Collect Prize"
#define LSTR_YOURE_DROWNING             660  // "You're drowning!"
#define LSTR_ON_FIRE                    661  // "On fire!"
#define LSTR_INSTRUCTORS                662  // "Instructors"
#define LSTR_ACCEPT_PARTY_HINT          664  // "Click here to accept this party and continue to the game."
#define LSTR_OK_BUTTON                  665  // "Ok Button"
#define LSTR_RESET_HINT                 666  // "Clears all party stats and skills."
#define LSTR_CLEAR_BUTTON               667  // "Clear Button"
#define LSTR_SUBTRACT                   668  // "Subtract"
#define LSTR_SKILL_DECREASE_HINT        669  // ""Subtracts a point from the highlighted skill, returning it to the bonus pool""
#define LSTR_ADD                        670  // "Add"
#define LSTR_SKILL_INCREASE_HINT        671  // ""Adds a point from the highlighted skill, taking it from the bonus pool""
#define LSTR_ONE_YEAR_SENTENCE          672  // ""For your numerous crimes and evil deeds, you have been sentenced to one year in prison.""
#define LSTR_GOOD_ENDING                675  // "Splendid job!  With the activation of the Gate, a thousand
                                             // worlds lie at your feet.  Perhaps on one of them you will
                                             // find the Ancients themselves, and return with the fruits
                                             // their great civilization has to offer your world and your
                                             // kingdom."
#define LSTR_EVIL_ENDING                676  // "Brilliant!  The completion of the Heavenly Forge has
                                             // provided enough Ancient weapons to crush all resistance to
                                             // your plans.  Soon the world will bow to your every whim!
                                             // Still, you can't help but wonder what was beyond the Gate
                                             // the other side was trying so hard to build."

class Localization {
 public:
    bool Initialize();

    const char *GetString(unsigned int index) const;
    String FormatString(unsigned int index, ...) const;

    const char *GetDayName(unsigned int index) const {
        return this->day_names[index];
    }

    const char *GetMonthName(unsigned int index) const {
        return this->month_names[index];
    }

    const char *GetMoonPhaseName(unsigned int index) const {
        return this->moon_phase_names[index];
    }

    const char *GetSpellSchoolName(unsigned int index) const {
        return this->spell_school_names[index];
    }

    const char *GetSpellName(unsigned int index) const {
        return this->spell_names[index];
    }

    const char *GetClassName(unsigned int index) const {
        return this->class_names[index];
    }

    const char *GetClassDescription(unsigned int index) const {
        return this->class_desciptions[index];
    }

    const char *GetAttirubteName(unsigned int index) const {
        return this->attribute_names[index];
    }

    const char *GetAttributeDescription(unsigned int index) const {
        return this->attribute_descriptions[index];
    }

    const char *GetSkillName(unsigned int index) const {
        return this->skill_names[index];
    }

    const char *GetSkillDescription(unsigned int index) const {
        return this->skill_descriptions[index];
    }

    const char *GetSkillDescriptionNormal(unsigned int index) const {
        return this->skill_descriptions_normal[index];
    }

    const char *GetSkillDescriptionExpert(unsigned int index) const {
        return this->skill_descriptions_expert[index];
    }

    const char *GetSkillDescriptionMaster(unsigned int index) const {
        return this->skill_descriptions_master[index];
    }

    const char *GetSkillDescriptionGrand(unsigned int index) const {
        return this->skill_descriptions_grand[index];
    }

    const char *GetCharacterConditionName(unsigned int index) const {
        return this->character_conditions[index];
    }

    const char *GetAmPm(unsigned int index) const {
        return this->GetString(472 + index);
    }

    const char *GetNpcProfessionName(NPCProf prof) const {
        Assert((int)prof >= 0 && (int)prof < 59);
        return this->npc_profession_names[prof];
    }

 public:
    Localization()
        : localization_raw(nullptr),
          localization_strings(nullptr),
          class_desc_raw(nullptr),
          attribute_desc_raw(nullptr),
          skill_desc_raw(nullptr) {}

 private:
    void InitializeMm6ItemCategories();

    void InitializeMonthNames();
    void InitializeDayNames();
    void InitializeMoonPhaseNames();

    void InitializeSpellSchoolNames();
    void InitializeSpellNames();

    void InitializeClassNames();
    void InitializeAttributeNames();
    void InitializeSkillNames();
    void InitializeCharacterConditionNames();

    void InitializeNpcProfessionNames();

 private:
    char *localization_raw;
    const char **localization_strings;
    char *class_desc_raw;
    char *attribute_desc_raw;
    char *skill_desc_raw;

    const char* mm6_item_categories[14]{};
    const char* month_names[12]{};
    const char* day_names[7]{};
    const char* moon_phase_names[5]{};
    const char* spell_school_names[9]{};
    const char* spell_names[44]{};
    const char* class_names[36]{};
    const char* class_desciptions[36]{};
    const char* attribute_names[7]{};
    const char* attribute_descriptions[7]{};
    const char* skill_names[38]{};
    const char* skill_descriptions[38]{};
    const char* skill_descriptions_normal[38]{};
    const char* skill_descriptions_expert[38]{};
    const char* skill_descriptions_master[38]{};
    const char* skill_descriptions_grand[38]{};
    const char* character_conditions[19]{};
    const char* npc_profession_names[59]{};
};

extern Localization *localization;
