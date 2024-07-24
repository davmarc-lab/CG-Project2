#pragma once

#include "IGMenu.hpp"

#include "../Lib.hpp"
#include "Logger/Log.hpp"
#include "Logger/LogManager.hpp"
#include <cstdint>

class IGViewport : public IGMenu {
  private:
    float width;
    float height;
    GLuint fbo;
    unsigned int texture;
    GLuint rbo;

  public:
    IGViewport() : IGViewport(800, 600) {}

    IGViewport(int width, int height) : width(width), height(height) {}

    inline bool init() {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenRenderbuffers(1, &this->rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LogManager::instance()->addLog(logs::ERROR, "Error during frame buffer init");
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
    }

    inline void rescaleRenderBuffer(int width, int height) {
        this->width = width;
        this->height = height;
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
    }

    inline void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    inline virtual void render() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        ImGui::Begin("Viewport");
        ImGui::BeginChild("Render");

        this->width = ImGui::GetContentRegionAvail().x;
        this->height = ImGui::GetContentRegionAvail().y;

        ImGui::Image((void *)(intptr_t)texture, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();
        ImGui::End();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~IGViewport() {
        glDeleteFramebuffers(1, &this->fbo);
        glDeleteTextures(1, &this->texture);
        glDeleteRenderbuffers(1, &this->rbo);
    }
};
