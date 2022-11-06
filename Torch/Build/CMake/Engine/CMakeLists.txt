add_custom_target(AllInEngine)
set_target_properties(AllInEngine PROPERTIES FOLDER "Torch/Engine")

add_subdirectory(Core)
add_subdirectory(TorchConfig)