#pragma once
#include "Texture.h"

class FrameBufferTexture 
{
public:
    GLuint FramebufferName;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
    
    FrameBufferTexture(int width, int height);
    void Bind();
};
