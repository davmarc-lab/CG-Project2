#include "PointLight.hpp"

#include "../Menu/IGDebug.hpp"

PointLight::PointLight(vec3 position, float constant, float linear, float quadratic) : PointLight() {
    this->position = position; this->caster->setPosition(position);
    this->info.constant = constant;
    this->info.linear = linear;
    this->info.quadratic = quadratic;
}

void PointLight::sendDataToShader(Shader shader, int index) {
    shader.use();

    shader.setInt("lights[" + to_string(index) + "].lightType", this->type);
    shader.setFloat("lights[" + to_string(index) + "].intensity", this->intensity);
    shader.setInt("max_num_lights", max_lights);

    shader.setVec3("lights[" + to_string(index) + "].color", this->color);
    shader.setVec3("lights[" + to_string(index) + "].position", this->caster->getPosition());

    shader.setFloat("lights[" + to_string(index) + "].constant", this->info.constant);
    shader.setFloat("lights[" + to_string(index) + "].linear", this->info.linear);
    shader.setFloat("lights[" + to_string(index) + "].quadratic", this->info.quadratic);

    shader.setVec3("lights[" + to_string(index) + "].ambient", this->vectors.ambient);
    shader.setVec3("lights[" + to_string(index) + "].diffuse", this->vectors.diffuse);
    shader.setVec3("lights[" + to_string(index) + "].specular", this->vectors.specular);

    shader.setMat4("projection", projection);
}

void PointLight::drawCaster() {
    if (this->caster == nullptr) {
        IGDebug::instance()->addLog(LogType::MISSING_CASTER, "No Caster Attached");
        return;
    }
    this->casterShader.use();
    this->casterShader.setVec3("light_color", this->getColor());
    this->casterShader.setMat4("projection", projection);
    this->casterShader.setMat4("view", camera.getViewMatrix());
    this->casterShader.setMat4("model", this->caster->getModelMatrix());
    this->caster->draw(this->casterShader);
}
