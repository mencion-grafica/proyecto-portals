#pragma once
#include "Texture.h"

class FrameBufferTexture : public Texture
{
public:
    GLuint FramebufferName;
    
    GLuint depthrenderbuffer;
    
    FrameBufferTexture(int width, int height);
    void Bind();
};
