#include "../include/AppGui.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <string>

#include "../include/ECS/EntityManager.hpp"
#include "../include/ECS/System.hpp"
#include "../include/PBR/PBMaterial.hpp"
#include "../include/PhysicWorld.hpp"
#include "../include/State/LoaderState.hpp"
#include "../include/State/SimulationState.hpp"

#include "../../Opengl-Core/include/Core.hpp"

#include "../../Opengl-Core/vendor/include/imgui/ImGuiFileDialog.h"

const auto em = EntityManager::instance();
const auto ed = EventManager::instance();

unsigned int npent = 0;
Shared<RenderComponent> rc = nullptr;
Shared<BufferComponent> bc = nullptr;
Shared<VertexComponent> vc = nullptr;

static unsigned int lightCompSel = 0;

void ImGuiEntityTree::onRender() {
	ImGui::Begin("Entities", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
	for (auto id : em->getEntities()) {
		if (em->entityHasComponent<HideTreeComponent>(id))
			continue;
		if (ImGui::TreeNode(systems::ecs::getEntityName(id).c_str())) {
			ImGui::PushID(&id);
			ImGui::Text("(ECS) Entity Id: %s", std::to_string(id).c_str());

			auto ivc = em->getComponentFromId<VertexComponent>(id);
			// the entity has the component
			if (ivc != nullptr) {
				if (!ivc->getNormalsCoords().empty()) {
					if (ImGui::Button("Normals##0")) {
						npent = id;
						ed->post(NORMAL_VIEW_OPEN);
						rc = em->getComponentFromId<RenderComponent>(id);
						ASSERT(rc != nullptr);
						bc = em->getComponentFromId<BufferComponent>(id);
						ASSERT(bc != nullptr);
						vc = em->getComponentFromId<VertexComponent>(id);
						ASSERT(vc != nullptr);
					}
				}
			}

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
					static char matName[256] = "";

					// Input Text for new material name
					ImGui::InputText("", matName, IM_ARRAYSIZE(matName));
					ImGui::SameLine();
					ImGuiHelpMarker("To sotre new materials first click on create, choose a name and set the parameters, then click the save button");
					if (ImGui::Button("Create##1")) {
						if (!std::string(matName).empty()) {
							auto nm = Material();
							nm.name = matName;
							systems::material::updateMaterial(id, nm);
						}
					}
					ImGui::SameLine();
					// Save current Material with the `matName` given
					if (ImGui::Button("Save##1")) {
						if (std::find(ALL(material::defaultMaterials), m) == material::defaultMaterials.end()) {
							material::defaultMaterials.push_back(m);
							material::materialTypes.push_back(material::defaultMaterials.size() - 1);
						}
					}

					// Material Name OUT
					ImGui::Text("Name: %s", m.name.c_str());
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

					if (ImGui::BeginCombo("Choose##1", "Materials")) {
						// Opens a list of default materials, and apply changes when one is clicked.
						for (int i = 0; i < material::defaultMaterials.size(); i++) {
							if (ImGui::Selectable(material::defaultMaterials[i].name.c_str(), (this->m_selectedMaterial == i))) {
								this->m_selectedMaterial = i;
								systems::material::updateMaterial(id, material::getMaterialFromPool((material::MaterialType)this->m_selectedMaterial));
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (this->m_selectedMaterial == i) {
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
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
					ed->post(CAMERA_RESET_POSITION);
				}
				if (ImGui::CollapsingHeader("Camera##4")) {
					auto camera = systems::camera::getCamera(id);
					if (ImGui::CollapsingHeader("World Info##4")) {
						auto pos = camera->getCameraPosition();
						if (ImGui::DragFloat3("Position##4", &pos.x)) {
							camera->setCameraPosition(pos);
							ed->post(CAMERA_UPDATE_DATA);
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

			auto sc = em->getComponentFromId<ShaderComponent>(id);
			if (sc != nullptr) {
				if (ImGui::CollapsingHeader("Shader##7")) {
					// shader files
					ImGui::Text("Vertex: %s", sc->vert.c_str());
					ImGui::Text("Frag: %s", sc->frag.c_str());
					if (!sc->geom.empty())
						ImGui::Text("Geom: %s", sc->geom.c_str());

					// light computation
					if (ImGui::BeginCombo("Light Comp##7", "Opts")) {
						for (int i = 0; i < light::lightCompStr.size(); i++) {
							if (ImGui::Selectable(light::lightCompStr[i].c_str(), (sc->computation == light::lightCompsEnm[i]))) {
								systems::light::setLightComputation(id, light::lightCompsEnm[i]);
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (sc->computation == light::lightCompsEnm[i]) {
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
				}
			}

			auto pm = em->getComponentFromId<PBMaterial>(id);
			if (pm != nullptr) {
				if (ImGui::CollapsingHeader("PBRMaterial##8")) {
					ImGui::SliderFloat3("Albedo: ", &pm->albedo.x, 0.f, 1.f);
					ImGui::SliderFloat("Metallic: ", &pm->metallic, 0.f, 1.f);
					ImGui::SliderFloat("Roughness: ", &pm->roughness, 0.f, 1.f);
					ImGui::DragFloat("Ao: ", &pm->ao, 1.f, 0.f);
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

ImGuiNormalView::ImGuiNormalView() {
	FBConfig fconf{};
	fconf.height = 20;
	fconf.width = 20;
	fconf.operation = GL_FRAMEBUFFER;

	RBConfig rconf{};
	rconf.height = 20;
	rconf.width = 20;
	rconf.format = GL_DEPTH24_STENCIL8;
	rconf.attachment = GL_DEPTH_STENCIL_ATTACHMENT;

	TextureParams tconf{};
	tconf.target = GL_TEXTURE_2D;
	tconf.border = 0;
	tconf.format = GL_RGB;
	tconf.internalFormat = GL_RGB;
	tconf.dataType = GL_UNSIGNED_BYTE;
	tconf.level = 0;

	this->m_fbo = CreateUnique<ogl::FrameBuffer>(fconf);
	this->m_fbo->onAttach();
	this->m_fbo->bind();

	this->m_text = ogl::Texture{tconf, 20, 20};
	this->m_text.onAttach();
	this->m_text.createTexture2D(NULL);
	this->m_text.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	this->m_text.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->m_fbo->setRenderBuffer(rconf);
	this->m_fbo->setColorTexture(this->m_text);

	auto res = this->m_fbo->createFrameBuffer();
	this->m_fbo->unbind();
	ASSERT(res);

	this->m_npcam = CreateUnique<ogl::Camera>();
	this->m_npcam->setCameraVelocity(0.05f);
	this->m_npcam->setCameraPosition(glm::vec3{0, 0, 3});
	this->m_npcam->setMouseSensitivity(0.3f);

	this->m_shader = CreateUnique<ogl::ShaderProgram>("basicVS.glsl", "basicFS.glsl");
	this->m_shader->createShaderProgram();
	this->m_nshader = CreateUnique<ShaderProgram>("normalVertShader.glsl", "normalFragShader.glsl", "normalGeomShader.glsl");
	this->m_nshader->createShaderProgram();
}

double npxpos, npypos;
bool npmfirst = true;

void ImGuiNormalView::processInput() {
	if (ImGui::IsKeyDown(ImGuiKey_W)) {
		this->m_npcam->moveCamera(this->m_npcam->getCameraFront());
	}
	if (ImGui::IsKeyDown(ImGuiKey_S)) {
		this->m_npcam->moveCamera(-this->m_npcam->getCameraFront());
	}
	if (ImGui::IsKeyDown(ImGuiKey_A)) {
		this->m_npcam->moveCamera(-this->m_npcam->getCameraRight());
	}
	if (ImGui::IsKeyDown(ImGuiKey_D)) {
		this->m_npcam->moveCamera(this->m_npcam->getCameraRight());
	}
	if (ImGui::IsKeyDown(ImGuiKey_Space)) {
		this->m_npcam->moveCamera(this->m_npcam->getCameraUp());
	}
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
		this->m_npcam->moveCamera(-this->m_npcam->getCameraUp());
	}

	auto nx = ImGui::GetMousePos().x;
	auto ny = ImGui::GetMousePos().y;

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right, false) || ImGui::IsMouseClicked(ImGuiMouseButton_Right, true)) {
		if (npmfirst) {
			npmfirst = false;
			npxpos = nx;
			npypos = ny;
		}
		auto xo = nx - npxpos;
		auto yo = ny - npypos;
		this->m_npcam->processMouseMovement(xo, yo);
	}
	npxpos = nx;
	npypos = ny;
}

ImVec2 npsize;
glm::mat4 npmodel{};
glm::vec3 npscale{0.3};
glm::vec3 nprot{};
ImGuiTableFlags tflag = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter;

const auto rd = Renderer::instance();

void ImGuiNormalView::onRender() {
	// framebuffer space
	ImGui::Begin("Normals");
	ImGui::BeginChild("Render");
	// resize the framebuffer
	npsize = ImGui::GetWindowSize();
	this->m_fbo->rescaleFrameBuffer(npsize.x, npsize.y);
	ImGui::Image((ImTextureID)this->m_text.getId(), npsize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();

	if (rc != nullptr) {
		this->m_npcam->updatePerspProjection(45.f, npsize.x / npsize.y, 0.01f, 10.f);

		npmodel = glm::translate(glm::mat4{1}, glm::vec3{0});
		npmodel = glm::scale(npmodel, npscale);

		// draw into the framebuffer
		this->m_fbo->bind();
		this->m_shader->use();
		this->m_shader->setMat4("viewProj", this->m_npcam->getViewProjMatrix());
		this->m_shader->setMat4("model", npmodel);

		bc->vao.bind();
		rd->drawElements(bc->vao.getId(), GL_LINE_STRIP, vc->getIndexCoords().size(), GL_UNSIGNED_INT);

		this->m_nshader->use();
		this->m_nshader->setMat4("view", this->m_npcam->getViewMatrix());
		this->m_nshader->setMat4("proj", this->m_npcam->getProjMatrix());
		this->m_nshader->setMat4("model", npmodel);
		rc->call();
		this->m_fbo->unbind();
	}

	ImGui::End();

	ImGui::Begin("Coords");
	if (ImGui::Button("Close")) {
		ed->post(NORMAL_VIEW_CLOSE);
		this->resetCamera();
		npmfirst = true;
	}
	ImGui::SameLine();
	ImGuiHelpMarker("This window shows all the normals of each point stored in the Buffers, also you can move the camera using WASD and while keeping the MOUSE2 button you can turn around");
	auto nv = em->getComponentFromId<VertexComponent>(npent)->getNormalsCoords();
	ImGui::Text("Normals Coords");
	ImGui::BeginTable("Normal Coords", 4, tflag);
	ImGui::TableSetupColumn("n", ImGuiTableColumnFlags_WidthFixed, 35.0f);
	ImGui::TableSetupColumn("x");
	ImGui::TableSetupColumn("y");
	ImGui::TableSetupColumn("z");
	ImGui::TableHeadersRow();
	for (auto i = 0; i < nv.size(); i++) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("%d", i + 1);
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", std::to_string(nv[i].x).c_str());
		ImGui::TableSetColumnIndex(2);
		ImGui::Text("%s", std::to_string(nv[i].y).c_str());
		ImGui::TableSetColumnIndex(3);
		ImGui::Text("%s", std::to_string(nv[i].z).c_str());
	}
	ImGui::EndTable();
	ImGui::End();
}

void ImGuiNormalView::resetCamera() {
	this->m_npcam = CreateUnique<ogl::Camera>();
	this->m_npcam->setCameraVelocity(0.05f);
	this->m_npcam->setCameraPosition(glm::vec3{0, 0, 3});
	this->m_npcam->setMouseSensitivity(0.1f);
}

void ImGuiSimulationPanel::onRender() {
	ImGui::Begin("Simulation Config");

	if (ImGui::DragFloat3("Gravity##", &this->m_config.gravity.x)) {
	}

	if (ImGui::Button("Run##")) {
		ed->post(RUN_SIMULATION);
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop##")) {
		ed->post(STOP_SIMULATION);
	}

	ImGui::Text("Collisions: %zu", systems::collision::getCollisions().size());

	if (ImGui::Button("Add")) {
		ed->post(ADD_SPHERE);
	}
	ImGui::End();
}

void ImGuiMeshLoader::onRender() {
	ImGui::Begin("Loader");

	if (ImGui::Button("Save")) {
		ed->post(SAVE_SCENE);
	};
	ImGui::SameLine();
	if (ImGui::Button("Load")) {
		ed->post(LOAD_SCENE);
	};

	ImGui::End();
}

void ImGuiFilePicker::onRender() {
	if (this->m_open) {
		IGFD::FileDialogConfig config;
		config.path = this->m_path;
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", this->m_title, this->m_extension.c_str(), config);
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

				ImGuiFileDialog::Instance()->GetSelection().clear();

				// if is using windows parse the path before using
#ifdef _WIN32
				{
					// make custom path for windows
					std::vector<std::string> words;
					std::string word;
					std::istringstream stream(filePathName);
					bool start_writing = false, add_sep = true;
					std::string res = "./";

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
					filePathName = std::string(res);
				}
#endif
				this->m_action(filePathName);
				this->close();
			}

			// close
			ImGuiFileDialog::Instance()->Close();
		}
	}
}
