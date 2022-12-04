set (EXTERNAL_PROJECT_DIR "${TORCH_ROOT_DIR}/External")

set(EXTERNAL_VS_FOLDER_PREFIX "External")

message("External external external !!!!")

FOREACH(flag_var
		CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
	IF(TORCH_COMPILER_MSVC OR TORCH_COMPILER_CLANGCL)
		STRING(REPLACE "/Wall" "/W0" ${flag_var} "${${flag_var}}")
		STRING(REPLACE "/WX" "/WX-" ${flag_var} "${${flag_var}}")
	ELSE()
		STRING(REPLACE "-Wall" "-w" ${flag_var} "${${flag_var}}")
		STRING(REPLACE "-Werror" "-Wno-error" ${flag_var} "${${flag_var}}")
	ENDIF()
ENDFOREACH()


add_subdirectory(GLad)
add_subdirectory(rapidjson)