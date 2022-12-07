add_custom_target(AllInEngine)
SET_TARGET_PROPERTIES(AllInEngine PROPERTIES FOLDER "Torch/Engine")

add_subdirectory(Core)
add_subdirectory(TorchConfig)