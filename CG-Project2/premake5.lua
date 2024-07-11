workspace("HelloWorld")
configurations({ "Debug", "Release" })

project("HelloWorld")
kind("ConsoleApp")
language("C++")
targetdir("bin/%{cfg.buildcfg}")

files({ "**.hpp", "**.h", "**.cpp", "**.c" })

links({ "glm", "GL", "glfw", "X11", "freetype", "assimp", "Xrandr", "Xi", "dl" })

filter("configurations:Debug")
defines({ "DEBUG" })
symbols("On")

filter("configurations:Release")
defines({ "NDEBUG" })
optimize("On")
