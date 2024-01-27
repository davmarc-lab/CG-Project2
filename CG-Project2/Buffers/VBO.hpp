#pragma once

#include "../Lib.hpp"

class VBO
{
    private:
        GLuint id;

    public:
        ~VBO()
        {
            this->destrory();
        }

        VBO(const void* data, const unsigned int size)
        {
            this->bind();
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        }

        VBO() {}

        GLuint bind()
        {
            glBindBuffer(GL_ARRAY_BUFFER, this->id);

            return this->id;
        }

        void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

        void bindAttribPosition(const GLuint pos, const int start, const int end, const int stride, const void* offset)
        {
            glVertexAttribPointer(start, end, GL_FLOAT, GL_FALSE, stride, offset);
            glEnableVertexAttribArray(pos);
        }

        void destrory() { glDeleteBuffers(1, &this->id); }
};
