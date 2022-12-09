if(TORCH_PLATFORM_ANDROID)
    set(library_output_dir ${CMAKE_CURRENT_BINARY_DIR}/libs/${TORCH_ARCH_NAME})
else()
    set(library_output_dir ${TORCH_BIN_DIR})
endif(TORCH_PLATFORM_ANDROID)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${library_output_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${library_output_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${library_output_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${library_output_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${library_output_dir})


macro(SETUP_SAMPLE EXE_NAME)
    if(TORCH_PLATFORM_ANDROID)
    elseif(TORCH_PLATFORM_IOS)
    elseif(TORCH_PLATFORM_WINDOWS_DESKTOP)
        SET(RESOURCE_FILES ${RESOURCE_FILES}
        $<TARGET_OBJECTS:Torch_RC>)
    endif(TORCH_PLATFORM_ANDROID)

    source_group("Source Files" FILES ${SOURCE_FILES})
    source_group("Header Files" FILES ${HEADER_FILES})
    source_group("Resource Files" FILES ${RESOURCE_FILES})
    source_group("Effect Files" FILES ${EFFECT_FILES})
    source_group("Post Processors" FILES ${POST_PROCESSORS})
    
    if(TORCH_PLATFORM_ANDROID)
    else()
        add_executable(${EXE_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} ${EFFECT_FILES} ${POST_PROCESSORS})
    endif(TORCH_PLATFORM_ANDROID)
    
    if(TORCH_COMPILER_CLANGCL OR TORCH_COMPILER_CLANG)
        target_compile_options(${EXE_NAME} PRIVATE -Wno-inconsistent-missing-override)
    endif()

    set_target_properties(${EXE_NAME} PROPERTIES 
        PROJECT_LABEL ${EXE_NAME} 
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        FOLDER "Torch/Samples"
    )
    
    if(TORCH_PLATFORM_ANDROID)
    else()
        set_target_properties(${EXE_NAME} PROPERTIES OUTPUT_NAME ${EXE_NAME}${TORCH_OUTPUT_SUFFIX})
    endif()

    if(TORCH_PLATFORM_ANDROID)
    else()
        target_link_libraries(${EXE_NAME} PRIVATE Common)
    endif()

    add_dependencies(${EXE_NAME} Common)

    
endmacro(SETUP_SAMPLE EXE_NAME)

add_subdirectory(Common)
add_subdirectory(Tutor)

