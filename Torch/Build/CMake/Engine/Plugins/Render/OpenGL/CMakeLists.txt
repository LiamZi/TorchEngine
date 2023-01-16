if(TORCH_PLATFORM_ANDROID OR TORCH_PLATFORM_IOS)
    return()
endif()

set(LIB_NAME Torch_Engine_OpenGL)

set(OPENGL_SOURCE_FILES 
    ${TORCH_PROJECT_DIR}/Plugins/Src/Render/OpenGL/OGLRenderEngine.cpp
    ${TORCH_PROJECT_DIR}/Plugins/Src/Render/OpenGL/OGLFrameBuffer.cpp
    ${TORCH_PROJECT_DIR}/Plugins/Src/Render/OpenGL/OGLCanvas.cpp
)

set(OPENGL_HEADER_FILES
    ${TORCH_PROJECT_DIR}/Plugins/Include/OpenGL/OGLRenderEngine.hpp
    ${TORCH_PROJECT_DIR}/Plugins/Include/OpenGL/OGLFrameBuffer.hpp
    ${TORCH_PROJECT_DIR}/Plugins/Include/OpenGL/OGLCanvas.hpp
)

source_group("Source FIles" FILES ${OPENGL_SOURCE_FILES})
source_group("Header FIles" FILES ${OPENGL_HEADER_FILES})

add_library(${LIB_NAME} SHARED ${OPENGL_SOURCE_FILES} ${OPENGL_HEADER_FILES})

target_include_directories(${LIB_NAME}
    PRIVATE
    ${TORCH_PROJECT_DIR}/Plugins/Include
)

set_target_properties(${LIB_NAME} PROPERTIES
    PROJECT_LABEL ${LIB_NAME}
    OUTPUT_NAME ${LIB_NAME}${TORCH_OUTPUT_SUFFIX}
    FOLDER "Torch/Engine/Plugins/Render"
)

if(TORCH_PREFERRED_LIB_TYPE STREQUAL "SHARED")
    set_target_properties(${LIB_NAME} PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
endif()

TORCH_ADD_PRECOMPILED_HEADER(${LIB_NAME} "${TORCH_PROJECT_DIR}/Core/Include/Torch/Torch.hpp")

target_link_libraries(${LIB_NAME}
    PRIVATE
    ${TORCH_CORELIB_NAME}
)

add_dependencies(Engine ${LIB_NAME})
