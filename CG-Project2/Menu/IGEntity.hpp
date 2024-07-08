#pragma once

#include "../Entity/Entity.hpp"

#include "../LibCustomGui.hpp"

#include "IGMenu.hpp"

#include <vector>
#include <sstream>

class IGEntity : public IGMenu {

private:
	Entity* observer;

	enum AXIS {
		X, Y, Z, NONE
	};

	inline vec3 updateOtherValues(vec3 vec, AXIS axis) {
		float val = 0.f;

		switch (axis) {
		case AXIS::X: {
			val = vec.x;
			break;
		}
		case AXIS::Y: {
			val = vec.y;
			break;
		}
		case AXIS::Z: {
			val = vec.z;
			break;
		}
		default: {
			return vec3(vec);
		}
		}

		return vec = vec3(val);
	}

	inline AXIS findAxis(vector<AXIS> scaleAxis) {
		for (const auto elem : scaleAxis) {
			if (elem != AXIS::NONE)
				return elem;
		}
		return AXIS::NONE;
	}

public:
	IGEntity(Entity* entity) : observer(entity) {}

	inline virtual void render() override {
		ImGui::Begin("Entity");
		auto pos = this->observer->getPosition();
		auto scale = this->observer->getScale();
		auto rot = this->observer->getRotation();

		static bool lockFlag = false;
		vector<AXIS> scaleAxis;

		if (ImGui::CollapsingHeader("Transform")) {
			ImGui::Text("Position:");
			if (ImGui::DragFloat("x", &pos.x, 0.005, 0.f)) {
				this->observer->setMotionTime(0);
			}
			if (ImGui::DragFloat("y", &pos.y, 0.005)) {
				this->observer->setMotionTime(0);
			}
			if (ImGui::DragFloat("z", &pos.z, 0.005)) {
				this->observer->setMotionTime(0);
			}
			if (ImGui::Button("Reset")) {
				pos = vec3(0);
			}
			this->observer->setPosition(vec3(pos));

			ImGui::Text("Scale:");
			AXIS click;

			click = ImGui::DragFloat("x##2", &scale.x, 0.005) ? AXIS::X : AXIS::NONE;
			scaleAxis.push_back(click);
			click = ImGui::DragFloat("y##2", &scale.y, 0.005) ? AXIS::Y : AXIS::NONE;
			scaleAxis.push_back(click);
			click = ImGui::DragFloat("z##2", &scale.z, 0.005) ? AXIS::Z : AXIS::NONE;
			scaleAxis.push_back(click);

			if (ImGui::Button("Reset##2")) {
				scale = vec3(1);
			}

			ImGui::Checkbox("Lock All", &lockFlag);
			if (lockFlag) {
				scale = updateOtherValues(scale, findAxis(scaleAxis));
			}

			this->observer->setScale(vec3(scale));

			ImGui::Text("Rotation:");
			ImGui::DragFloat("x##3", &rot.x, 0.005);
			ImGui::DragFloat("y##3", &rot.y, 0.005);
			ImGui::DragFloat("z##3", &rot.z, 0.005);
			if (ImGui::Button("Reset##3")) {
				rot = vec3(0);
			}
			this->observer->setRotation(vec3(rot));
		}

		if (ImGui::CollapsingHeader("Shader")) {

		}

		if (ImGui::CollapsingHeader("Texture")) {
			ImGui::Text("Texture File:");
			ImGui::Text("%s", this->observer->getTexture().getPath());
			if (ImGui::Button("Choose Texture")) {
				IGFD::FileDialogConfig config;
				config.path = "./resources/textures/";
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", config);
			}
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
				if (ImGuiFileDialog::Instance()->IsOk()) {
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

					// action
#ifdef _WIN32
					{
						// make custom path for windows
						std::vector<std::string> words;
						std::string word;
						std::istringstream stream(filePathName);
						bool start_writing = false, add_sep = true;
						string res = "./";

						while (std::getline(stream, word, '\\')) {
							std::istringstream subStream(word);
							if (!start_writing && word == "resources") {
								start_writing = true;
							}

							if (start_writing) {
								res.append(word);
								res.append("/");
							}
						}

						if (!res.empty()) {
							res.pop_back();
						}
						filePathName = string(res);
					}
#endif
					this->observer->changeTexture(filePathName);
				}
				// close
				ImGuiFileDialog::Instance()->Close();
			}
		}

		/*auto vel = this->observer->getVelocity();
		auto force = this->observer->getForce();
		auto mass = this->observer->getMass();

		if (ImGui::CollapsingHeader("Physics")) {
			ImGui::Text("Mass:");
			if (ImGui::DragFloat("mass", &mass, 0, 200)) {
				this->observer->setMass(mass);
			}

			ImGui::Text("Velocity:");
			ImGui::DragFloat("x##4", &vel.x, 0.005);
			ImGui::DragFloat("y##4", &vel.y, 0.005);
			ImGui::DragFloat("z##4", &vel.z, 0.005);
			this->observer->setVelocity(vel);

			ImGui::Text("Force:");
			ImGui::DragFloat("x##5", &force.x, 0.005);
			ImGui::DragFloat("y##5", &force.y, 0.005);
			ImGui::DragFloat("z##5", &force.z, 0.005);
		}*/


		ImGui::End();
	}

	~IGEntity() = default;
};
