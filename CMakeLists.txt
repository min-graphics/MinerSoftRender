cmake_minimum_required(VERSION 3.14)

project(MinerSoftRender)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CONFIGURATION_TYPES "Release")#只生成release版本的，要不然debug版本的也会生成。

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -GL -fp:fast")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -ffast-math")
endif()

set(HEADERS
        core/camera.h
        core/pch.h
        core/renderbuffer.h
        core/shader.h
        math/color.h
        math/graphics.h
        math/maths.h
        math/matrix.h
        math/vector.h
        model/model.h
        model/tgaimage.h
        scenes/gameobject.h
        scenes/scenes.h
        platform/platform.h
        )

set(SOURCES
        core/camera.cpp
        core/pch.cpp
        core/renderbuffer.cpp
        math/color.cpp
        math/graphics.cpp
        math/maths.cpp
        math/vector.cpp
        model/model.cpp
        model/tgaimage.cpp
        scenes/gameobject.cpp
        scenes/scenes.cpp
        platform/platform.cpp
        shaders/FlatShader.cpp
        shaders/GouraudShader.cpp
        shaders/NormalMapShader.cpp
        shaders/PhongShader.cpp
        shaders/ShadowShader.cpp
        shaders/TextureShader.cpp
        shaders/TextureWithLightShader.cpp
        shaders/ToonShader.cpp
        main.cpp
        )

add_executable(MinerSoftRender  ${HEADERS} ${SOURCES})

if(MSVC)
    target_compile_options(MinerSoftRender PRIVATE /fp:fast)
else()
    target_compile_options(MinerSoftRender PRIVATE -ffast-math)
    target_link_libraries(MinerSoftRender  m)
endif()

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT MinerSoftRender)
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES})