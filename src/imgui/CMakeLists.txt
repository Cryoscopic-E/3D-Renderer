set(_target "imgui")

set(source_list
        imconfig.h
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_impl_glfw.cpp
        imgui_impl_glfw.h
        imgui_impl_opengl3.cpp
        imgui_impl_opengl3.h
        imgui_impl_opengl3_loader.h
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
)
add_library(${_target} ${source_list})
target_include_directories(${_target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${_target} PUBLIC glfw glm::glm)
