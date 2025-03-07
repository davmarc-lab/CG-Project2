workspace "opengl-wrapper"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Opengl-Sandbox"

    OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

    libdirs = {
        "bin/" .. OutputDir .. "/",
    }

    group "App"
        include "App/BuildApp.lua"

    group "Opengl-Core"
        include "Opengl-Core/BuildCore.lua"

