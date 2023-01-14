add_custom_target(Engine)
SET_TARGET_PROPERTIES(Engine PROPERTIES FOLDER "Torch/Engine")

add_subdirectory(Core)
add_subdirectory(Plugins)
add_subdirectory(TorchConfig)