#pragma once
#include <string>

void DrawPopupWindow(unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight);  // idb

class GraphicsImage;
class Texture;
extern Texture *parchment;
extern GraphicsImage *messagebox_corner_x;       // 5076AC
extern GraphicsImage *messagebox_corner_y;       // 5076B4
extern GraphicsImage *messagebox_corner_z;       // 5076A8
extern GraphicsImage *messagebox_corner_w;       // 5076B0
extern GraphicsImage *messagebox_border_top;     // 507698
extern GraphicsImage *messagebox_border_bottom;  // 5076A4
extern GraphicsImage *messagebox_border_left;    // 50769C
extern GraphicsImage *messagebox_border_right;   // 5076A0

extern bool holdingMouseRightButton;
extern bool rightClickItemActionPerformed;
extern bool identifyOrRepairReactionPlayed;
