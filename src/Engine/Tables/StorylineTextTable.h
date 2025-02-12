#pragma once

#include <cstdint>
#include <string>

struct StorylineRecord {
    std::string pText;
    std::string pPageTitle;
    uint8_t uTime;
    char f_9;
    char f_A;
    char f_B;
};

struct StorylineText {
    void Initialize();
    StorylineRecord StoreLine[29];
    int field_15C;
    // int field_0;
    // int field_4[87];
};

extern struct StorylineText *pStorylineText;
