#include "../include/AppGui.hpp"
#include <glm/trigonometric.hpp>
#include <string>

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"
#include "../include/PhysicWorld.hpp"

#include "../../Opengl-Core/include/Core.hpp"

const auto em = EntityManager::instance();

void ImGuiEntityTree::onRender() {
	ImGui::Begin("Entities", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
	for (auto id : em->getEntities()) {
		if (em->entityHasComponent<HideTreeComponent>(id))
			continue;
		if (ImGui::TreeNode(systems::ecs::getEntityName(id).c_str())) {
			ImGui::PushID(&id);
			ImGui::Text("(ECS) Entity Id: %s", std::to_string(id).c_str());

			if (em->entityHasComponent<Transform>(id)) {
				if (ImGui::CollapsingHeader("Transform##0")) {
					auto pos = systems::transform::getPosition(id);
					if (ImGui::DragFloat3("Position##0", &pos.x, 0.2f)) {
						systems::transform::updatePosition(id, pos);
						if (em->entityHasComponent<PhysicComponent>(id))
							systems::physic::resetGravitySolver(id);
					}

					auto scale = systems::transform::getScale(id);
					if (ImGui::DragFloat3("Scale##0", &scale.x, 0.2f)) {
						systems::transform::updateScale(id, scale);
					}

					auto rot = glm::degrees(systems::transform::getRotation(id));
					if (ImGui::DragFloat3("Rotation##0", &rot.x)) {
						systems::transform::updateRotation(id, rot);
					}
				}
			}

			if (em->entityHasComponent<PhysicComponent>(id)) {
				if (ImGui::CollapsingHeader("Physics##0")) {
					auto vel = systems::physic::getVelocity(id);
					if (ImGui::DragFloat3("Velocity##0", &vel.x)) {
						systems::physic::updateVelocity(id, vel);
					}
					auto acc = systems::physic::getAcceleration(id);
					if (ImGui::DragFloat3("Acceleration##0", &acc.x)) {
						systems::physic::updateAcceleration(id, acc);
					}
					auto force = systems::physic::getForce(id);
					if (ImGui::DragFloat3("Force##0", &force.x)) {
						systems::physic::updateForce(id, force);
					}
					auto mass = systems::physic::getMass(id);
					if (ImGui::DragFloat("Mass##0", &mass, 0.05f)) {
						if (mass > 0.f)
							systems::physic::updateMass(id, mass);
					}
				}
			}

			if (em->entityHasComponent<MaterialComponent>(id)) {
				if (ImGui::CollapsingHeader("Material##1")) {
					auto m = systems::material::getMaterial(id);
					if (ImGui::SliderFloat3("Ambient##1", &m.ambient.x, 0.f, 1.f)) {
						systems::material::updateAmbient(id, m.ambient);
					}
					if (ImGui::SliderFloat3("Diffuse##1", &m.diffuse.x, 0.f, 1.f)) {
						systems::material::updateDiffuse(id, m.diffuse);
					}
					if (ImGui::SliderFloat3("Specular##1", &m.specular.x, 0.f, 1.f)) {
						systems::material::updateSpecular(id, m.specular);
					}
					if (ImGui::DragFloat("Shininess##1", &m.shininess, 0.5f)) {
						systems::material::updateShininess(id, m.shininess);
					}
				}
			}

			if (em->entityHasComponent<LightComponent>(id)) {
				if (ImGui::CollapsingHeader("Light##2")) {
					auto data = systems::light::getLightInfo(id);
					ImGui::SeparatorText("General Info##2");
					if (ImGui::ColorEdit3("Color##2", &data.color.x)) {
						systems::light::updateColor(id, data.color);
					}
					if (ImGui::SliderFloat("Intensity##2", &data.intensity, 0.f, 1.f)) {
						systems::light::updateIntensity(id, data.intensity);
					}
					if (ImGui::SliderFloat3("Ambient##2", &data.ambient.x, 0.f, 1.f)) {
						systems::light::updateAmbient(id, data.ambient);
					}
					if (ImGui::SliderFloat3("Diffuse##2", &data.diffuse.x, 0.f, 1.f)) {
						systems::light::updateDiffuse(id, data.diffuse);
					}
					if (ImGui::SliderFloat3("Specular##2", &data.specular.x, 0.f, 1.f)) {
						systems::light::updateSpecular(id, data.specular);
					}
					ImGui::SeparatorText("Specific Info##2");
					switch (data.type) {
						case LightType::LIGHT_DIRECTIONAL: {
							ImGui::TextColored({1, 1, 0, 1}, "Directional Light");
							if (ImGui::SliderFloat3("Direction##2", &data.direction.x, -1.f, 1.f)) {
								systems::light::updateDirection(id, data.direction);
							}
							break;
						}
						case LightType::LIGHT_POINT: {
							ImGui::TextColored({1, 1, 0, 1}, "Point Light");
							if (ImGui::DragFloat3("Position##2", &data.position.x, 0.2f)) {
								systems::light::updatePosition(id, data.position);
							}
							if (ImGui::SliderFloat("Constant##2", &data.constant, 0.f, 1.f)) {
								systems::light::updateConstant(id, data.constant);
							}
							if (ImGui::SliderFloat("Linear##2", &data.linear, 0.f, 1.f)) {
								systems::light::updateLinear(id, data.linear);
							}
							if (ImGui::SliderFloat("Quadratic##2", &data.quadratic, 0.f, 1.f)) {
								systems::light::updateQuadratic(id, data.quadratic);
							}
							break;
						}
						case LightType::LIGHT_SPOT: {
							ImGui::TextColored({1, 1, 0, 1}, "Spot Light");
							if (ImGui::DragFloat3("Position##2", &data.position.x, 0.2f)) {
								systems::light::updatePosition(id, data.position);
							}
							if (ImGui::SliderFloat3("Direction##2", &data.direction.x, -1.f, 1.f)) {
								systems::light::updateDirection(id, data.direction);
							}
							if (ImGui::SliderFloat("Constant##2", &data.constant, 0.f, 1.f)) {
								systems::light::updateConstant(id, data.constant);
							}
							if (ImGui::SliderFloat("Linear##2", &data.linear, 0.f, 1.f)) {
								systems::light::updateLinear(id, data.linear);
							}
							if (ImGui::SliderFloat("Quadratic##2", &data.quadratic, 0.f, 1.f)) {
								systems::light::updateQuadratic(id, data.quadratic);
							}
							if (ImGui::DragFloat("Cut Off##2", &data.cutoff, 0.2f, 0.f)) {
								systems::light::updateCutoff(id, data.cutoff);
							}
							if (ImGui::DragFloat("Outer Cut Off##2", &data.outerCutoff, 0.2f, 0.1f)) {
								systems::light::updateOuterCutoff(id, data.outerCutoff);
							}

							break;
						}
						default:
							break;
					}
				}
			}

			if (em->entityHasComponent<TextureComponent>(id)) {
				if (ImGui::CollapsingHeader("Texture##3")) {
					ImGui::Text("%s", systems::texture::getTexturePath(id).c_str());
					ImGui::SameLine();
					if (ImGui::Button("Change")) {
						std::cout << "CHANGE TEXTURE\n";
					}
				}
			}

			if (em->entityHasComponent<CameraComponent>(id)) {
				if (ImGui::Button("Reset##4")) {
					EventManager::instance()->post(CAMERA_RESET_POSITION);
				}
				if (ImGui::CollapsingHeader("Camera##4")) {
					auto camera = systems::camera::getCamera(id);
					if (ImGui::CollapsingHeader("World Info##4")) {
						auto pos = camera->getCameraPosition();
						if (ImGui::DragFloat3("Position##4", &pos.x)) {
							camera->setCameraPosition(pos);
							EventManager::instance()->post(CAMERA_UPDATE_DATA);
						}
					}
					if (ImGui::CollapsingHeader("Camera Info##4")) {
						auto speed = camera->getCameraVelocity();
						if (ImGui::SliderFloat("Velocity##4", &speed, 0.01f, 1.f)) {
							camera->setCameraVelocity(speed);
						}
						auto sens = camera->getMouseSensitivity();
						if (ImGui::SliderFloat("Sensitivity##4", &sens, 0.01f, 1.f)) {
							camera->setMouseSensitivity(sens);
						}
						auto zoom = camera->getCameraZoom();
						if (ImGui::SliderFloat("Zoom##4", &zoom, 20.f, 100.f)) {
							camera->setCameraZoom(zoom);
						}
					}

					if (ImGui::CollapsingHeader("Camera Rotation##4")) {
						auto rot = camera->getCameraRotation();
						if (ImGui::DragFloat("Yaw##4", &rot.yaw)) {
							camera->setCameraYaw(rot.yaw);
						}
						if (ImGui::DragFloat("Pitch##4", &rot.pitch)) {
							camera->setCameraPitch(rot.pitch);
						}
					}
				}
			}

			if (em->entityHasComponent<ColliderComponent>(id)) {
				if (ImGui::CollapsingHeader("Collider##5")) {
					auto cc = em->getComponentFromId<ColliderComponent>(id);
					auto bot = cc->botLeft;
					ImGui::DragFloat3("Bot Left##5", &bot.x);
					auto top = cc->topRight;
					ImGui::DragFloat3("Top Right##5", &top.x);
					auto pos = cc->position;
					ImGui::DragFloat3("Position##5", &pos.x);
				}
			}

			if (em->entityHasComponent<ParentComponent>(id)) {
				if (ImGui::CollapsingHeader("Parent##6")) {
					ImGui::Text("%zu", em->getComponentFromId<ParentComponent>(id)->children.size());
				}
			}

			ImGui::PopID();
			ImGui::TreePop();
		}
	}

	ImGui::Separator();
	// ImGui::Text("Collisions -> %zu", systems::collision::getCollisions().size());
	ImGui::Text("FPS -> %f", ImGui::GetIO().Framerate);
	ImGui::End();
}

void ImGuiEntityModel::onRender() {
	if (this->m_ett >= 0) {
		ImGui::Begin("Model", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
		ImGui::PushID(&this->m_ett);
		ImGui::Text("(ECS) Entity Id: %s", std::to_string(this->m_ett).c_str());
		auto pos = systems::transform::getPosition(this->m_ett);
		if (ImGui::DragFloat3("Position", &pos.x, 0.2f)) {
			systems::transform::updatePosition(this->m_ett, pos);
			if (em->entityHasComponent<PhysicComponent>(this->m_ett))
				systems::physic::resetGravitySolver(this->m_ett);
		}

		auto scale = systems::transform::getScale(this->m_ett);
		if (ImGui::DragFloat3("Scale", &scale.x, 0.2f)) {
			systems::transform::updateScale(this->m_ett, scale);
		}

		auto rot = glm::degrees(systems::transform::getRotation(this->m_ett));
		if (ImGui::DragFloat3("Rotation", &rot.x)) {
			systems::transform::updateRotation(this->m_ett, rot);
		}
		ImGui::PopID();

		ImGui::End();
	}
}
