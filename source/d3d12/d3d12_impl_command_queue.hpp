/*
 * Copyright (C) 2021 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "d3d12_impl_command_list_immediate.hpp"
#include <shared_mutex>

namespace reshade::d3d12
{
	class command_queue_impl : public api::api_object_impl<ID3D12CommandQueue *, api::command_queue>
	{
	public:
		command_queue_impl(device_impl *device, ID3D12CommandQueue *queue);
		~command_queue_impl();

		device_impl *get_device();		// VUGGER_ADDON

		api::command_queue_type get_type() const final;

		void wait_idle() const final;

		void flush_immediate_command_list() const final;

		api::command_list *get_immediate_command_list() final { return _immediate_cmd_list; }

		void begin_debug_event(const char *label, const float color[4]) final;
		void end_debug_event() final;
		void insert_debug_marker(const char *label, const float color[4]) final;

		// VUGGER_ADDON: BEGIN
		void execute_command_lists(uint32_t count, api::command_list **command_lists, bool restore_state) final;
		void finish_command_list(api::command_list **, bool) final { assert(false); }
		// VUGGER_ADDON: END

		// 'ID3D12CommandQueue' is thread-safe, so need to lock when accessed from multiple threads
		std::shared_mutex _mutex;

	private:
		//device_impl *const _device_impl;		// VUGGER_ADDON
		command_list_immediate_impl *_immediate_cmd_list = nullptr;

		HANDLE _wait_idle_fence_event = nullptr;
		mutable UINT64 _wait_idle_fence_value = 0;
		com_ptr<ID3D12Fence> _wait_idle_fence;
	};
}
