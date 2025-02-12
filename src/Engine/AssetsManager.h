#pragma once

#include <string>
#include <unordered_map>

#include "Library/Color/ColorTable.h"

class GraphicsImage;
class Texture;

class AssetsManager {
 public:
    AssetsManager() {}

    void releaseAllTextures();

    bool releaseImage(const std::string &name);
    bool releaseSprite(const std::string &name);
    bool releaseBitmap(const std::string &name);

    Texture *getImage_ColorKey(const std::string &name, Color colorkey = colorTable.TealMask);
    Texture *getImage_Paletted(const std::string &name);
    Texture *getImage_Solid(const std::string &name);
    Texture *getImage_Alpha(const std::string &name);

    Texture *getImage_PCXFromFile(const std::string &name);
    Texture *getImage_PCXFromIconsLOD(const std::string &name);
    Texture *getImage_PCXFromNewLOD(const std::string &name);

    Texture *getBitmap(const std::string &name);
    Texture *getSprite(const std::string &name, unsigned int palette_id,
                       unsigned int lod_sprite_id);

    // TODO(pskelton): Contain better
    // TODO(pskelton): Manager should have a ref to all loose textures created throuh CreateTexture_Blank also
    Texture *winnerCert{ nullptr };

 protected:
    std::unordered_map<std::string, Texture *> bitmaps;
    std::unordered_map<std::string, Texture *> sprites;
    std::unordered_map<std::string, Texture *> images;
};

extern AssetsManager *assets;
