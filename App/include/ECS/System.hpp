#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "../../../Opengl-Core/include/Core.hpp"
#include "Component.hpp"

namespace systems {
	namespace ecs {
		bool removeEntityFromManager(const unsigned int &id);
		void removeEntityFromScene(const unsigned int &id);
		bool removeEntityFromAll(const unsigned int &id);

		std::string getEntityName(const unsigned int &id);
		void updateEntityName(const unsigned int &id, const std::string &name);
	} // namespace ecs

	namespace transform {
		void updatePosition(const unsigned int &id, const glm::vec3 &position);
		void updateScale(const unsigned int &id, const glm::vec3 &scale);
		void updateRotation(const unsigned int &id, const glm::vec3 &rotation);

		void addPosition(const unsigned int &id, const glm::vec3 &offset);
		void addScale(const unsigned int &id, const glm::vec3 &offset);
		void addRotation(const unsigned int &id, const glm::vec3 &offset);

		glm::vec3 getPosition(const unsigned int &id);
		glm::vec3 getScale(const unsigned int &id);
		glm::vec3 getRotation(const unsigned int &id);
		glm::mat4 getModelMatrix(const unsigned int &id);

		void updateModelMatrix(const unsigned int &id);

		void updateAllModelMatrix();

	} // namespace transform

	namespace collision {
		void updateParentCollider(const unsigned int &id);
		void updateCollider(const unsigned int &id);
		void updateAllColliders();

        ColliderType getColliderType(const unsigned int& id);
        void updateColliderType(const unsigned int& id, const ColliderType& type);

		void compressBoundingBox();

		void resolveCollisions();

		float getEnemyLastHit(const unsigned int &id);
		float getPlayerLastHit(const unsigned int &id);

		Pair<glm::vec3> getCollider(const unsigned int &id);

		void updateEnemyLastHit(const unsigned int &id, const float &time);
		void updatePlayerLastHit(const unsigned int &id, const float &time);

		std::vector<Pair<unsigned int>> getCollisions();
	} // namespace collision

	namespace camera {
		Shared<ogl::Camera> getCamera(const unsigned int &id);

		void updateCameraCollider(const unsigned int &id, const glm::vec3 &position, const glm::vec3 &size);
	} // namespace camera

	namespace parent {
		void addChild(const unsigned int &parent, const unsigned int &child);
		void removeChild(const unsigned int &parent, const unsigned int &child);
	} // namespace parent

	namespace input {
		std::vector<std::pair<unsigned int, std::function<void()>>> getKeysCallback(const unsigned int &id);

		void setKeyCallback(const unsigned int &id, const unsigned int &key, std::function<void()> func);
	} // namespace input

	namespace animation {
		void executeNextFrame(const float &currentTime);
		void updateDistanceAnimation();
	} // namespace animation

	namespace texture {
		ogl::Texture getTexture(const unsigned int &id);

		void setTexture(const unsigned int &id, const ogl::Texture &texture);

		std::string getTexturePath(const unsigned int &id);
		void updateTexturePath(const unsigned int &id, const std::string &path);

		bool isMeshReflective(const unsigned int &id);
		void setMeshReflective(const unsigned int &id, const bool &val);
	} // namespace texture

	namespace material {
		Material getMaterial(const unsigned int &id);
		void updateAmbient(const unsigned int &id, const glm::vec3 &val);
		void updateDiffuse(const unsigned int &id, const glm::vec3 &val);
		void updateSpecular(const unsigned int &id, const glm::vec3 &val);
		void updateShininess(const unsigned int &id, const float &val);
		void updateMaterial(const unsigned int &id, const Material &material);
	} // namespace material

	namespace light {
		LightShaderBlock getLightInfo(const unsigned int &id);
		void updateColor(const unsigned int &id, const glm::vec3 &color);
		void updateIntensity(const unsigned int &id, const float &intensity);
		void updateAmbient(const unsigned int &id, const glm::vec3 &ambient);
		void updateDiffuse(const unsigned int &id, const glm::vec3 &diffuse);
		void updateSpecular(const unsigned int &id, const glm::vec3 &specular);
		void updateDirection(const unsigned int &id, const glm::vec3 &direction);
		void updatePosition(const unsigned int &id, const glm::vec3 &position);
		void updateConstant(const unsigned int &id, const float &constant);
		void updateLinear(const unsigned int &id, const float &linear);
		void updateQuadratic(const unsigned int &id, const float &quadratic);
		void updateCutoff(const unsigned int &id, const float &cutoff);
		void updateOuterCutoff(const unsigned int &id, const float &outerCutoff);

		LightComputation getLightComputation(const unsigned int &id);
	} // namespace light

	namespace render {
		void initStencilShader();

		void renderSkybox(const unsigned int &id, const Shared<ogl::ShaderProgram> &shader);
		void renderAllMeshes();

		void renderBoundingBox();
	} // namespace render
} // namespace systems
