set_project("Minecraft")
set_version("0.0.1")

set_languages("c++23")

add_rules("mode.debug", "mode.release")

add_requires("glfw", "fmt", "glad", "glm", "enet")


if is_mode("debug") then 
    add_defines("MC_DEBUG")
    --set_warnings("allextra", "error")
    set_warnings("allextra")
end

target("MinecraftLib")
    set_kind("static")

    add_defines("MC_LIB_BUILD")
    --add_defines("GLM_ENABLE_EXPERIMENTAL")

    add_includedirs("Lib/include/",  { public = true })
    add_headerfiles("Lib/include/(**/*.h)", "Lib/include/(**/*.inl)")

    add_files("Lib/**/*.cpp")

    add_packages("glm", "fmt")

target("MinecraftClient")
    set_kind("binary")

    -- Use to don't care about glfw and glad include order
    add_defines("GLFW_INCLUDE_NONE")

    add_includedirs("Client/include/",  { public = true })

    add_files("Client/**/*.cpp")

    if is_plat("macosx") then
        add_defines("MACOSX")
    end

    add_deps("MinecraftLib")
    add_packages("glfw", "fmt", "glad", "glm", "enet")

target("MinecraftServer")
    set_kind("binary")

    add_includedirs("Server/include/",  { public = true })

    add_files("Server/**/*.cpp")

    add_deps("MinecraftLib")
    add_packages("fmt", "enet")