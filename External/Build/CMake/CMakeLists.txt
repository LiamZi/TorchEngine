set (EXTERNAL_PROJECT_DIR "${TORCH_ROOT_DIR}/External")

set(EXTERNAL_VS_FOLDER_PREFIX "External")

message("External external external !!!!")

if(NOT EXISTS ${git_executable})
	unset(git_executable CACHE)
endif()

find_program(git_executable NAMES git git.exe git.cmd)
if(NOT git_executable)
	message(FATAL_ERROR "Failed to find git. ")
endif()

function(CloneExternalLib name url brach shallow_exclude)
	set(external_folder "${TORCH_ROOT_DIR}/External")
	set(external_lib_folder "${external_folder}/${name}")

	if(EXISTS ${external_lib_folder})
		message(STATUS "Remoing ${external_lib_folder} ... ")
		file(REMOVE_RECURSE ${external_lib_folder})
	endif()

	message(STATUS "Cloning ${name} ... ")
	set(brach_param "")
	set(brach_name_param "")
	if(NOT ("x${brach}" STREQUAL "x"))
		set(branch_param "-b")
		set(brach_name_param "${branch}")
	endif()

	set(shallow_excluse_param "")
	if(NOT ("x${shallow_exclude}" STREQUAL "x"))
		set(shallow_excluse_param "--shallow_exclude=${shallow_exclude}")
	endif()

	execute_process(COMMAND "${git_executable}" "clone" ${url} ${branch_param} ${branch_name_param} ${shallow_exclude_param} "${name}" "-n" WORKING_DIRECTORY "${external_folder}")
	
endfunction(CloneExternalLib name url brach shallow_exclude)

function(CheckoutExternalLib name rev)
	set(external_folder "${TORCH_ROOT_DIR}/External/${name}")
	execute_process(COMMAND "${git_executable}" "checkout" "-q" ${rev} WORKING_DIRECTORY ${external_lib_folder} RESULT_VARIABLE checkout_err)
	set(${ARGV2} ${checkout_err} PARENT_SCOPE)
endfunction(CheckoutExternalLib name rev)


function(UpdateExternalLib name url rev)
	set(external_folder "${TORCH_ROOT_DIR}/External")
	set(external_lib_folder "${external_folder}/${name}")

	if(EXISTS "${external_lib_folder}/.git")
		set(need_clone FALSE)
		set(need_update_submodule FALSE)

		set(cached_rev_file_name "${CMAKE_CURRENT_BINARY_DIR}/TORCH_${name}_REV")
		if(EXISTS "${cached_rev_file_name}")
			file(STRINGS "${cached_rev_file_name}" cached_rev)
		else()
			set(cached_rev "")
		endif()

		if("${cached_rev}" STREQUAL "${rev}")
			set(need_clone FALSE)
		else()
			message(STATUS "Updating ${name} to revision ${rev} ... ")
			execute_process(COMMAND "${git_executable}" "fetch" "origin" WORKING_DIRECTORY ${external_lib_folder})
			execute_process(COMMAND "${git_executable}" "rev-parse" "HEAD" WORKING_DIRECTORY ${external_lib_folder} OUTPUT_VARIABLE head_rev)
			string(STRIP ${head_rev} head_rev)
			if(${head_rev} STREQUAL ${rev})
				set(need_checkout FALSE)
			else()
				set(need_checkout TRUE)
			endif()
			file(WRITE "${cached_rev_file_name}" ${rev})
		endif()
	else()
		set(need_clone TRUE)
		set(need_update_submodule TRUE)
	endif()

	if(need_clone)
		CloneExternalLib(${name} ${url} "${ARGV3}" "${ARGV4}")
		set(need_checkout TRUE)
	endif()

	if(need_checkout)
		message(STATUS "Checking out to revison ${rev} ... ")
		CheckoutExternalLib(${name} ${rev} checkout_err)
		if(checkout_err)
			message(STATUS "Could not checkout revison ${rev}, reclone the repository . ")
			CloneExternalLib(${name} ${url} "${ARGV3}" "${ARGV4}")

			message(STATUS "Checking out to revision ${rev}")
			CheckoutExternalLib(${name} ${rev})
		endif()
	endif()

	if(need_update_submodule)
		message("need_update_submodule")
		execute_process(COMMAND "${git_executable}" "submodule" "init" WORKING_DIRECTORY ${external_lib_folder})
		execute_process(COMMAND "${git_executable}" "submodule" "update" "--remote" "--recursive" WORKING_DIRECTORY ${external_lib_folder})
	endif()

	set(${ARGV5} ${need_checkout} PARENT_SCOPE)
endfunction(UpdateExternalLib name url rev)

function(ApplyPatch name patch)
	set(external_folder "${TORCH_ROOT_DIR}/External")
	set(external_lib_folder "${external_folder}/${name}")

	execute_process(COMMAND "${git_executable}" "apply" "--check" "--ignore-space-change" "${patch}" WORKING_DIRECTORY ${external_lib_folder} RESULT_VARIABLE checkout_err)
	if(NOT checkout_err)
		message(STATUS "Applying ${patch} ...")
		execute_process(COMMAND "${git_executable}" "am" "--ignore-space-change" "${path}" WORKING_DIRECTORY ${external_lib_folder})
	endif()
endfunction(ApplyPatch name patch)


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
add_subdirectory(GLM)
add_subdirectory(EASTL)
