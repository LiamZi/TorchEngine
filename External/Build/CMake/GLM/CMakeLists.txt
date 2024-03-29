set(GLM_PROJECT_DIR "${EXTERNAL_PROJECT_DIR}/GLM")
set(GLM_OUTPUT_DIR "${EXTERNAL_PROJECT_DIR}/lib/GLM/${TORCH_PLATFORM_NAME}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GLM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GLM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GLM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${GLM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${GLM_OUTPUT_DIR})

UpdateExternalLib("GLM" "https://github.com/g-truc/glm.git" "97e09aa3047844991fe5c9625cae9a072a8ed03a" "" "" need_patch)

# set(BUILD_SHARED_LIBS TRUE)

set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
set(LIBRARY_OUTPUT_PATH CACHE STRING "${GLM_OUTPUT_DIR}" FORCE)

add_subdirectory(${GLM_PROJECT_DIR} "${CMAKE_CURRENT_BINARY_DIR}/glm")

set(CMAKE_DISABLE_FIND_PACKAGE_GLM TRUE CACHE BOOL "" FORCE)
set(GLM_INCLUDE_DIR
    ${EXTERNAL_PROJECT_DIR}/GLM/glm
    ${PROJECT_BINARY_DIR}/External/GLM/glm
    CACHE INTERNAL "" FORCE
)

set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR} CACHE INTERNAL "" FORCE)
set(GLM_LIBRARY ${TORCH_GLM_NAME} CACHE INTERNAL "" FORCE)
set(GLM_LIBRARIES ${GLM_LIBRARY} CACHE INTERNAL "" FORCE)
set(GLM_FOUND TRUE CACHE INTERNAL "" FORCE)

if(TORCH_PREFERRED_LIB_TYPE STREQUAL "STATIC")
    set(TORCH_GLM_NAME "glm::glm_static" CACHE STRING "" FORCE)
else()
    set(TORCH_GLM_NAME "glm::glm_shared" CACHE STRING "" FORCE)
endif()

# set_target_properties(${TORCH_GLM_NAME} PROPERTIES 
#     OUTPUT_NAME glm${TORCH_OUTPUT_SUFFIX}
#     FOLDER "${EXTERNAL_VS_FOLDER_PREFIX}/GLM"
# )

# foreach(target "glm::glm_shared" "glm")
# 	set_target_properties(${target} PROPERTIES FOLDER "${EXTERNAL_VS_FOLDER_PREFIX}/GLM/glm")
# endforeach()


# add_subdirectory(${GLM_PROJECT_DIR} ${GLM_OUTPUT_DIR} EXCLUDE_FROM_ALL)



