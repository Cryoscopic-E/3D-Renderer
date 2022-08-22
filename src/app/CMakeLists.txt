file(GLOB all_SRCS
        #####
        # ADD SOURCE FILES HERE
        #####
        graphics/Shader.hpp
        graphics/Shader.cpp
        graphics/Light.hpp
        graphics/stb_image.h
        graphics/Texture.hpp
        graphics/Texture.cpp
        graphics/Material.hpp
        graphics/Material.cpp
        graphics/Mesh.hpp
        graphics/Mesh.cpp
        graphics/Renderer.hpp
        graphics/Renderer.cpp
        graphics/Camera.hpp
        graphics/Camera.cpp
        
        loaders/OBJLoader.hpp
        loaders/OBJLoader.cpp

        core/Object.hpp
        core/Object.cpp
        core/Transform.hpp
        core/Transform.cpp
        core/Model.hpp
        core/Model.cpp
        
        main.cpp
        )

add_executable(${PROJECT_NAME} ${all_SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL glad glm imgui)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL glad glm)