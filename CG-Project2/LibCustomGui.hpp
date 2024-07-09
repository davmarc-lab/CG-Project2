#pragma once

#ifdef _WIN32
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui-file-dialog/ImGuiFileDialog.h>
#else
#include "dependencies/include/imgui/imgui.h"
#include "dependencies/include/imgui/imgui_impl_glfw.h"
#include "dependencies/include/imgui/imgui_impl_opengl3.h"
#include "dependencies/include/imgui-file-dialog/ImGuiFileDialog.h"
#endif

