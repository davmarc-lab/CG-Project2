#include "../include/AppGui.hpp"
#include <glm/trigonometric.hpp>
#include <string>

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"

const auto em = EntityManager::instance();

void ImGuiEntityTree::onRender() {
	ImGui::Begin("Entities", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
	for (auto id : em->getEntitiesFromComponent<Transform>()) {
		if (ImGui::TreeNode(systems::ecs::getEntityName(id).c_str())) {
			auto comp = em->getComponentFromId<Transform>(id);

			ImGui::PushID(&id);
			ImGui::Text("Ett: %s", std::to_string(id).c_str());

			if (comp != nullptr) {
				auto pos = systems::transform::getPosition(id);
				if (ImGui::DragFloat3("Position", &pos.x, 0.2f)) {
					systems::transform::updatePosition(id, pos);
				}

				auto scale = systems::transform::getScale(id);
				if (ImGui::DragFloat3("Scale", &scale.x, 0.2f)) {
					systems::transform::updateScale(id, scale);
				}

				auto rot = glm::degrees(systems::transform::getRotation(id));
				if (ImGui::DragFloat3("Rotation", &rot.x)) {
					systems::transform::updateRotation(id, rot);
				}
			}

			ImGui::PopID();
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void ImGuiEntityModel::onRender() {
	if (this->m_ett >= 0) {
		ImGui::Begin("Model", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
		ImGui::PushID(&this->m_ett);
		ImGui::Text("Ett: %s", std::to_string(this->m_ett).c_str());
		auto pos = systems::transform::getPosition(this->m_ett);
		if (ImGui::DragFloat3("Position", &pos.x, 0.2f)) {
			systems::transform::updatePosition(this->m_ett, pos);
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
