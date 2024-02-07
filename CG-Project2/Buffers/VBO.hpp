#pragma once

#include "../Lib.hpp"

class VBO {
    private:
        GLuint id;
        GLenum bufferType;
        GLenum bufferUsage;

    public:
        ~VBO() { this->destrory(); }

        VBO() {}

        VBO(GLenum bufferType, GLenum bufferUsage) : bufferType(bufferType), bufferUsage(bufferUsage) {
            glGenBuffers(1, &this->id);
        }

        GLuint bind() {
            glBindBuffer(this->bufferType, this->id);
            return this->id;
        }

        void unbind() { glBindBuffer(this->bufferType, 0); }

        void bindAttribPosition(const int pos, const int start, const int end, const int stride, const void *offset) {
            glVertexAttribPointer(start, end, GL_FLOAT, GL_FALSE, stride, offset);
            glEnableVertexAttribArray(pos);
        }

        void bufferData(unsigned int size, void *data) {
            this->bind();
            glBufferData(this->bufferType, size, data, this->bufferUsage);
        }

        void destrory() { glDeleteBuffers(1, &this->id); }
};
