set_project("MachinaCubicaCatalyst")
set_version("0.0.1")

set_languages("c++23")

add_rules("mode.debug", "mode.release")

add_repositories("devalexxx-repo https://github.com/devalexxx/xmake-repo.git")
add_requires("hexis", { debug = is_mode("debug"), configs = { shared = false } })

add_requires("glfw", "fmt >= 11", "glad", "glm", "enet", "flecs", "cereal", "perlinnoise", "zlib")

add_requires("flecs", { debug = is_mode("debug") })

if is_host("macos") or is_host("linux") then
    add_defines("MCC_POSIX")
end

if is_host("windows") then
    add_cxxflags("cl::/Zc:preprocessor")
end

if is_mode("debug") then 
    add_defines("MCC_DEBUG")
    --set_warnings("allextra", "error")
    set_warnings("allextra")
end

add_defines("GLM_ENABLE_EXPERIMENTAL")
--add_defines("MCC_LOG_FULL")

target("MCCCommon")
    set_kind("static")

    add_defines("MCC_LIB_BUILD")

    add_includedirs("Common/include/",  { public = true })
    add_headerfiles("Common/include/(*.h)", "Common/include/(*.inl)")
    add_headerfiles("Common/include/(**/*.h)", "Common/include/(**/*.inl)")

    add_files("Common/**/*.cpp")

    add_packages("glm", "fmt", "flecs", "enet", "cereal", "zlib")
    add_packages("hexis", { public = true })

target("MCCClient")
    set_kind("binary")

    -- Use to don't care about glfw and glad include order
    add_defines("GLFW_INCLUDE_NONE")

    add_includedirs("Client/include/",  { public = true })
    add_headerfiles("Client/include/(**/*.h)", "Client/include/(**/*.inl)")

    add_files("Client/**/*.cpp")

    if is_plat("macosx") then
        add_defines("MACOSX")
    end

    add_deps("MCCCommon")
    add_packages("glfw", "fmt", "glad", "glm", "enet", "flecs", "cereal", "hexis", "zlib")

target("MCCServer")
    set_kind("binary")

    add_includedirs("Server/include/",  { public = true })
    add_headerfiles("Server/include/(**/*.h)", "Server/include/(**/*.inl)")

    add_files("Server/**/*.cpp")

    add_deps("MCCCommon")
    add_packages("fmt", "enet", "flecs", "cereal", "glm", "perlinnoise")