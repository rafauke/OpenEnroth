#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Utility/Memory/Blob.h"

class Texture;

// TODO(captainurist): where is this used?
enum TEXTURE_FRAME_TABLE_FLAGS {
    TEXTURE_FRAME_TABLE_MORE_FRAMES = 0x1,
    TEXTURE_FRAME_TABLE_FIRST = 0x2,
};

class TextureFrame {
 public:
    inline TextureFrame() : tex(nullptr) {}

    std::string name = "null";
    int16_t uAnimTime = 0;
    int16_t uAnimLength = 0;
    int16_t uFlags = 0;  // 1 for anim

    Texture *GetTexture();

 protected:
    Texture *tex;
};

struct TextureFrameTable {
    void FromFile(const Blob &data_mm6, const Blob &data_mm7, const Blob &data_mm8);
    void LoadAnimationSequenceAndPalettes(int uIconID);
    Texture *GetFrameTexture(int frameId, int time);
    /**
    * @param   frameID        TextureFrameTable index
    * @return                 Total length of texture animation
    */
    int textureFrameAnimLength(int frameID);
    /**
    * @param   frameID        TextureFrameTable index
    * @return                 Dwell time of this texture
    */
    int textureFrameAnimTime(int frameID);
    int64_t FindTextureByName(const std::string &Str2);

    std::vector<TextureFrame> textures;
};

extern TextureFrameTable *pTextureFrameTable;
