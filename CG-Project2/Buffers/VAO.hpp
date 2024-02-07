#pragma once

#include "../Lib.hpp"

class VAO {
    private:
        GLuint id;

    public:
        /* Desctructor. */
        ~VAO() {}

        /* Empty constructor. */
        VAO() { glGenVertexArrays(1, &this->id); }

        /*
         * This method bind the buffer and retrives its id.
         */
        GLuint bind() {
            glBindVertexArray(this->id);
            return this->id;
        }

        /*
         * This method unbind the current buffer.
         */
        void unbind() { glBindVertexArray(0); }

        /*
         * This method destrory the current buffer.
         */
        void destroy() { glDeleteVertexArrays(1, &this->id); }
};
