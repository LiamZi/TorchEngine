set(GLM_PROJECT_DIR "${EXTERNAL_PROJECT_DIR}/ImGUI")
set(GLM_OUTPUT_DIR "${EXTERNAL_PROJECT_DIR}/lib/ImGUI/${TORCH_PLATFORM_NAME}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GLM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GLM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GLM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${GLM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${GLM_OUTPUT_DIR})

UpdateExternalLib("ImGUI" "https://github.com/ocornut/imgui.git" "eec344cc1e40185e505d593ba2cef2f18dea2266" "" "" need_patch)
