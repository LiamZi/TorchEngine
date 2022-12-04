set(GLAD_PROJECT_DIR "${EXTERNAL_PROJECT_DIR}/GLad")

set(C_FILES
    ${GLAD_PROJECT_DIR}/glad.c
)

set(GLAD_HEADER_FILES
    ${GLAD_PROJECT_DIR}/glad.h
    ${GLAD_PROJECT_DIR}/khrplatform.h
)

source_group("Source Files" FILES ${C_FILES})
source_group("Header FIles" FILES ${GLAD_HEADER_FILES})

add_library(GLad INTERFACE)
