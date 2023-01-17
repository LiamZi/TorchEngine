message("camke glad #####")
set(GLAD_PROJECT_DIR "${EXTERNAL_PROJECT_DIR}/GLad")
set(LIB_NAME GLad)

set(GLAD_SOURCE_FILES
    ${GLAD_PROJECT_DIR}/glad.c
)

set(GLAD_HEADER_FILES
    ${GLAD_PROJECT_DIR}/glad.h
    ${GLAD_PROJECT_DIR}/khrplatform.h
)

source_group("Source Files" FILES ${C_FILES})
source_group("Header FIles" FILES ${GLAD_HEADER_FILES})

add_library(${LIB_NAME} 
    ${GLAD_SOURCE_FILES} ${GLAD_HEADER_FILES}
)

target_include_directories(${LIB_NAME} INTERFACE ${GLAD_PROJECT_DIR})

set_target_properties(${LIB_NAME} PROPERTIES 
                PROJECT_LABEL ${LIB_NAME}
                OUTPUT_NAME ${LIB_NAME}${TORCH_OUTPUT_SUFFIX}
                FOLDER "External/GLad"
)

