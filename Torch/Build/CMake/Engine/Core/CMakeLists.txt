set(LIB_NAME ${TORCH_CORELIB_NAME})

set(BASE_SOURCE_FILES
    ${TORCH_PROJECT_DIR}/Core/Src/Base/Context.cpp
)

set(BASE_HEADER_FILES
    ${TORCH_PROJECT_DIR}/Core/Include/Torch/Torch.hpp
    ${TORCH_PROJECT_DIR}/Core/Include/Torch/Interfaces/Context.hpp
)

SOURCE_GROUP("Base\\Source Files" FILES ${BASE_SOURCE_FILES})
SOURCE_GROUP("Base\\Header Files" FILES ${BASE_HEADER_FILES})

set(RENDERER_SOURCE_FILES
    ${TORCH_PROJECT_DIR}/Core/Src/Renderer/ElementFormat.cpp
)

set(RENDERER_HEADER_FILES
    ${TORCH_PROJECT_DIR}/Core/Include/Torch/Interfaces/RenderSettings.hpp
    ${TORCH_PROJECT_DIR}/Core/Include/Torch/Renderer/ElementFormat.hpp
)

SOURCE_GROUP("Renderer\\Source Files" FILES ${RENDERER_SOURCE_FILES})
SOURCE_GROUP("Renderer\\Header Files" FILES ${RENDERER_HEADER_FILES})

set(APP_SOURCE_FILES 
    ${TORCH_PROJECT_DIR}/Core/Src/AppPlatform/App3DFramework.cpp
    ${TORCH_PROJECT_DIR}/Core/Src/AppPlatform/Window.cpp
)

if(TORCH_PLATFORM_WINDOWS_DESKTOP)
    set(APP_SOURCE_FILES ${APP_SOURCE_FILES} ${TORCH_PROJECT_DIR}/Core/Src/AppPlatform/WindowWin.cpp)
endif()

set(APP_HEADER_FILES 
    ${TORCH_PROJECT_DIR}/Core/Include/Torch/Interfaces/App3DFramework.hpp
    ${TORCH_PROJECT_DIR}/Core/Include/Torch/Interfaces/Window.hpp
)

SOURCE_GROUP("AppPlatform\\Source Files" FILES ${APP_SOURCE_FILES})
SOURCE_GROUP("AppPlatform\\Header Files" FILES ${APP_HEADER_FILES})

add_library(${LIB_NAME} 
    ${TORCH_PREFERRED_LIB_TYPE} 
    ${BASE_SOURCE_FILES} ${BASE_HEADER_FILES}
    ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES} 
    ${APP_SOURCE_FILES} ${APP_HEADER_FILES}

)

target_compile_definitions(${LIB_NAME}
	PRIVATE
		-DTORCH_CORE_SOURCE
)

if(TORCH_PLATFORM_WINDOWS AND TORCH_COMPILER_GCC)
	# GCC warnings emitted by preprocessor can't be suppressed with pragma GCC diagnostic ignored
	target_compile_options(${LIB_NAME}
		PRIVATE
			-Wno-unknown-pragmas
	)
endif()


target_include_directories(${LIB_NAME} PUBLIC ${TORCH_PROJECT_DIR}/Core/Include)

set_target_properties(${LIB_NAME} PROPERTIES 
                    PROJECT_LABEL ${LIB_NAME} 
                    OUTPUT_NAME ${LIB_NAME}${TORCH_OUTPUT_SUFFIX} 
                    FOLDER "Torch/Engine/Core")

if(TORCH_PREFERRED_LIB_TYPE STREQUAL "SHARED")
    set_target_properties(${LIB_NAME} PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
endif()

TORCH_ADD_PRECOMPILED_HEADER(${LIB_NAME} "${TORCH_PROJECT_DIR}/Core/Include/Torch/Torch.hpp")

target_link_libraries(${LIB_NAME} 
    PUBLIC
    TML
)


if(TORCH_PLATFORM_WINDOWS)
    target_link_libraries(${LIB_NAME}
    PRIVATE
    ws2_32
    )

endif()

add_dependencies(AllInEngine ${LIB_NAME})

if(TORCH_PLATFORM_WINDOWS_DESKTOP)
    set(rc_lib_name "Torch_RC")
    add_library(${rc_lib_name} OBJECT 
        ${TORCH_PROJECT_DIR}/media/Torch.rc
    )

    set_target_properties(${rc_lib_name} PROPERTIES
        FOLDER "Torch/Engine/Core"
    )

    add_dependencies(AllInEngine ${rc_lib_name})
endif()