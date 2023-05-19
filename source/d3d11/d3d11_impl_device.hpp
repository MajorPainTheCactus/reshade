/*
 * Copyright (C) 2021 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <d3d11_4.h>
#include "com_ptr.hpp"
#include "addon_manager.hpp"

namespace reshade::d3d11
{
	class device_impl;

	// VUGGER_ADDON:
	class shader_resource_view_impl : public api::api_object_impl<ID3D11ShaderResourceView *, api::shader_resource_view>
	{
	public:
		explicit shader_resource_view_impl(device_impl *device, ID3D11ShaderResourceView *shader_resource_view)
			: api_object_impl(shader_resource_view), _device_impl(device)
		{
		}
		virtual ~shader_resource_view_impl() {}

		virtual void override_view(api::resource_view view)
		{
			_orig = reinterpret_cast<ID3D11ShaderResourceView*>(view.handle);
		}

		api::device *get_device() final;

	private:
		device_impl *const _device_impl = nullptr;
	};

	class unordered_access_view_impl : public api::api_object_impl<ID3D11UnorderedAccessView *, api::unordered_access_view>
	{
	public:
		explicit unordered_access_view_impl(device_impl *device, ID3D11UnorderedAccessView *unordered_access_view)
			: api_object_impl(unordered_access_view), _device_impl(device)
		{
		}
		virtual ~unordered_access_view_impl() {}	

		virtual void override_view(api::resource_view view)
		{
			_orig = reinterpret_cast<ID3D11UnorderedAccessView *>(view.handle);
		}

		api::device *get_device() final;

	private:
		device_impl *const _device_impl = nullptr;
	};

	class render_target_view_impl : public api::api_object_impl<ID3D11RenderTargetView *, api::render_target_view>
	{
	public:
		explicit render_target_view_impl(device_impl *device, ID3D11RenderTargetView *render_target_view)
			: api_object_impl(render_target_view), _device_impl(device)
		{
		}
		virtual ~render_target_view_impl() {}

		virtual void override_view(api::resource_view view)
		{
			_orig = reinterpret_cast<ID3D11RenderTargetView *>(view.handle);
		}

		api::device *get_device() final;

	private:
		device_impl *const _device_impl = nullptr;
	};

	class depth_stencil_view_impl : public api::api_object_impl<ID3D11DepthStencilView *, api::depth_stencil_view>
	{
	public:
		explicit depth_stencil_view_impl(device_impl *device, ID3D11DepthStencilView *depth_stencil_view)
			: api_object_impl(depth_stencil_view), _device_impl(device)
		{
		}
		virtual ~depth_stencil_view_impl() {}

		virtual void override_view(api::resource_view view)
		{
			_orig = reinterpret_cast<ID3D11DepthStencilView *>(view.handle);
		}

		api::device *get_device() final;

	private:
		device_impl *const _device_impl = nullptr;
	};
	// VUGGER_ADDON:

	class device_impl : public api::api_object_impl<ID3D11Device *, api::device>
	{
		friend class swapchain_impl;
		friend class device_context_impl;

	public:
		explicit device_impl(ID3D11Device *device);
		~device_impl();

		api::device_api get_api() const final { return api::device_api::d3d11; }

		bool check_capability(api::device_caps capability) const final;
		bool check_format_support(api::format format, api::resource_usage usage) const final;

		bool create_sampler(const api::sampler_desc &desc, api::sampler *out_handle) final;
		void destroy_sampler(api::sampler handle) final;

		bool create_resource(const api::resource_desc &desc, const api::subresource_data *initial_data, api::resource_usage initial_state, api::resource *out_handle, HANDLE *shared_handle = nullptr) final;
		void destroy_resource(api::resource handle) final;

		api::resource_desc get_resource_desc(api::resource resource) const final;

		bool create_resource_view(api::resource resource, api::resource_usage usage_type, const api::resource_view_desc &desc, api::resource_view *out_handle) final;
		void destroy_resource_view(api::resource_view handle) final;

		api::resource get_resource_from_view(api::resource_view view) const final;
		api::resource_view_desc get_resource_view_desc(api::resource_view view) const final;

		// VUGGER_ADDON: BEGIN
		bool map_buffer_region(api::command_list *command_list, api::resource resource, uint64_t offset, uint64_t size, api::map_access access, void **out_data) final;
		void unmap_buffer_region(api::command_list *command_list, api::resource resource) final;
		bool map_texture_region(api::command_list *command_list, api::resource resource, uint32_t subresource, const api::subresource_box *box, api::map_access access, api::subresource_data *out_data) final;
		void unmap_texture_region(api::command_list *command_list, api::resource resource, uint32_t subresource) final;

		void update_buffer_region(api::command_list *command_list, const void *data, api::resource resource, uint64_t offset, uint64_t size) final;
		void update_texture_region(api::command_list *command_list, const api::subresource_data &data, api::resource resource, uint32_t subresource, const api::subresource_box *box) final;
		// VUGGER_ADDON: END

		bool create_pipeline(api::pipeline_layout layout, uint32_t subobject_count, const api::pipeline_subobject *subobjects, api::pipeline *out_handle) final;
		bool create_input_layout(uint32_t count, const api::input_element *desc, const api::shader_desc &signature, api::pipeline *out_handle);
		bool create_vertex_shader(const api::shader_desc &desc, api::pipeline *out_handle);
		bool create_hull_shader(const api::shader_desc &desc, api::pipeline *out_handle);
		bool create_domain_shader(const api::shader_desc &desc, api::pipeline *out_handle);
		bool create_geometry_shader(const api::shader_desc &desc, api::pipeline *out_handle);
		bool create_pixel_shader(const api::shader_desc &desc, api::pipeline *out_handle);
		bool create_compute_shader(const api::shader_desc &desc, api::pipeline *out_handle);
		bool create_rasterizer_state(const api::rasterizer_desc &desc, api::pipeline *out_handle);
		bool create_blend_state(const api::blend_desc &desc, api::pipeline *out_handle);
		bool create_depth_stencil_state(const api::depth_stencil_desc &desc, api::pipeline *out_handle);
		void destroy_pipeline(api::pipeline handle) final;

		bool create_pipeline_layout(uint32_t param_count, const api::pipeline_layout_param *params, api::pipeline_layout *out_handle) final;
		void destroy_pipeline_layout(api::pipeline_layout handle) final;

		bool allocate_descriptor_sets(uint32_t count, api::pipeline_layout layout, uint32_t layout_param, api::descriptor_set *out_sets) final;
		void free_descriptor_sets(uint32_t count, const api::descriptor_set *sets) final;

		void get_descriptor_pool_offset(api::descriptor_set set, uint32_t binding, uint32_t array_offset, api::descriptor_pool *out_pool, uint32_t *out_offset) const final;

		void copy_descriptor_sets(uint32_t count, const api::descriptor_set_copy *copies) final;
		void update_descriptor_sets(uint32_t count, const api::descriptor_set_update *updates) final;

		bool create_query_pool(api::query_type type, uint32_t size, api::query_pool *out) final;
		void destroy_query_pool(api::query_pool handle) final;

		bool get_query_pool_results(api::query_pool pool, uint32_t first, uint32_t count, void *results, uint32_t stride) final;

		void set_resource_name(api::resource handle, const char *name) final;
		void set_resource_view_name(api::resource_view handle, const char *name) final;
		void set_object_data(uint64_t handle, const uint8_t (&guid)[16], uint32_t size, void* data) final;
		void get_object_data(uint64_t handle, const uint8_t (&guid)[16], uint32_t* size, void* data) final;
		void set_resource_data(api::resource handle, const uint8_t (&guid)[16], uint32_t size, void* data) final;
		void get_resource_data(api::resource handle, const uint8_t (&guid)[16], uint32_t* size, void* data) final;
	};
}
