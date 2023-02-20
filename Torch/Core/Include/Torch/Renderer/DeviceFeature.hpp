#ifndef __TORCH_CORE_RENDER_DEVICE_FEATURE_HPP__
#define __TORCH_CORE_RENDER_DEVICE_FEATURE_HPP__


#pragma once

#include <stdint.h>


namespace Torch
{
	enum TessellationType
	{
		TT_HARDWARE,
		TT_NO,
	};

	struct DeviceFeature
	{
		uint32_t _max_texture_width;
		uint32_t _max_texture_height;
		uint32_t _max_texture_depth;
		uint32_t _max_texture_cube_size;
		uint32_t _max_texture_array_length;
		uint32_t _max_vertex_texture_units;
		uint32_t _max_pixel_texture_units;
		uint32_t _max_geometry_texture_units;
		uint32_t _max_simultaneous_rts;
		uint32_t _max_simultaneous_uavs;
		uint32_t _max_vertex_streams;
		uint32_t _max_texture_anisotropy;

		bool _is_tbdr : true;
		bool _primitive_restart_support : true;
		bool _multithread_rendering_support : true;
		bool _multithread_res_createing_support : true;
		bool _arbitrary_multithread_rendering_support : true;
		bool _mrt_independent_bit_depths_support : true;
		bool _logic_op_support : true;
		bool _independent_blend_support : true;
		bool _depth_texture_support : true;
		bool _fp_color_support : true;
		bool _pack_to_rgba_required : true;
		bool _draw_indirect_support : true;
		bool _no_overwrite_support : true;
		bool _full_npot_texture_support : true;
		bool _render_to_texture_array_support : true;
		bool _explicit_multi_sample_support : true;
		bool _load_from_buffer_support : true;
		bool _uavs_at_every_stage_support : true;
		bool _rovs_support : true;
		bool _flexible_srvs_support : true;
		bool _vp_rt_index_at_every_stage_support : true;

		bool _gs_support : true;
		bool _cs_support : true;
		bool _hs_support : true;
		bool _ds_support : true;

		TessellationType _tessType;


	};
}

#endif
