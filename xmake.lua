set_languages("cxx20")
set_arch("x64")

add_rules("mode.debug", "mode.release")
add_requires("glfw", "glew", "imgui")

add_defines("UNICODE")

target("example")
    set_kind("binary")
    add_files("example/src/**.cpp")
    add_headerfiles("src/**.h")
    add_packages("glfw", "glew", "imgui")
    add_includedirs("include")

    on_package(function(target)
      os.rm("$(buildir)/windows/x64/release/locales")
      os.cp("example/locales", "$(buildir)/windows/x64/release/locales")
    end)