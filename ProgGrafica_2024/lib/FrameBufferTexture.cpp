#include "FrameBufferTexture.h"
#include "Common.h"

FrameBufferTexture::FrameBufferTexture(int width, int height)
{
    this->FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    glGenTextures(1, &this->GLId);

    glBindTexture(GL_TEXTURE_2D, this->GLId);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA16F, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &this->depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthrenderbuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->GLId, 0);

    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Error creating framebuffer" << std::endl;
}

void FrameBufferTexture::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glViewport(0,0,1024,768);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
}
