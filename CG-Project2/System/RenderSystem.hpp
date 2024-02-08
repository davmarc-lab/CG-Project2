#pragma once

#include "ISystem.hpp"

#include "../World/World.hpp"
#include "../Shader/Shader.hpp"
#include "../Lib.hpp"

struct Resources {
    Shader shader;
    mat4 model;
};

class RenderSystem : ISystem{
    private:
        Resources res;
        GLuint modelLoc;

    public:
        RenderSystem(Shader shader, mat4 model) : res{ shader, model } {
            shader.use();
            this->modelLoc = glGetUniformLocation(shader.getId(), "model");            
            cout << to_string(model) << endl;
        }

        RenderSystem() {}
        
        void exec(World world) {
            glBindVertexArray(world.getId());
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        virtual void update() override {

        }
};
