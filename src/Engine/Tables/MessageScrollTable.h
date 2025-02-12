#pragma once

#include <string>

#include "Engine/Objects/ItemEnums.h"
#include "Utility/IndexedArray.h"

/**
 * @offset 0x4764C2
 */
void initializeMessageScrolls();

extern IndexedArray<std::string, ITEM_FIRST_MESSAGE_SCROLL, ITEM_LAST_MESSAGE_SCROLL> pMessageScrolls;
