#include "Sprite.h"
#include "Renderer.h"
#include <iostream>

Sprite::Sprite(const Image* i, float x1, float x2, float y1, float y2) :
    _mySourceImage(i), sx1(x1), sx2(x2), sy1(y1), sy2(y2)
{
    w = sx2 - sx1; h = sy2 - sy1;
}


void Sprite::draw(float tx1, float ty1, float w, float h) const
{
    Renderer::Instance()->drawImage(_mySourceImage, sx1, sy1, sx2, sy2,
        tx1, ty1, w + tx1, h + ty1);
}

Sprite Sprite::fromRegularSpriteSheet(const Image* i, int numFil, int numCol, int fila, int col)
{
    float spriteWidth = i->width() / numCol; float spriteHeight = i->height() / numFil;
    float sx1 = col * spriteWidth; float sx2 = sx1 + spriteWidth; float sy1 = fila * spriteHeight; float sy2 = sy1 + spriteHeight;
    return Sprite(i, sx1, sx2, sy1, sy2);
}


