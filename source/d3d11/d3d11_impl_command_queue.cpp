/*
 * Copyright (C) 2021 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "d3d11_impl_device.hpp"
#include "d3d11_impl_device_context.hpp"


reshade::d3d11::device_context_impl::device_context_impl(device_impl *device, ID3D11DeviceContext *context) :
	api_object_impl(context, device)			// VUGGER_ADDON
{
	context->QueryInterface(&_annotations);

#if RESHADE_ADDON
	invoke_addon_event<addon_event::init_command_list>(this);
	if (_orig->GetType() == D3D11_DEVICE_CONTEXT_IMMEDIATE)
		invoke_addon_event<addon_event::init_command_queue>(this);
#endif
}
reshade::d3d11::device_context_impl::~device_context_impl()
{
#if RESHADE_ADDON
	if (_orig->GetType() == D3D11_DEVICE_CONTEXT_IMMEDIATE)
		invoke_addon_event<addon_event::destroy_command_queue>(this);
	invoke_addon_event<addon_event::destroy_command_list>(this);
#endif
}

// VUGGER_ADDON:
//reshade::api::device *reshade::d3d11::device_context_impl::get_device()
//{
//	return _device_impl;
//}
// VUGGER_ADDON:

reshade::api::command_list *reshade::d3d11::device_context_impl::get_immediate_command_list()
{
	assert(_orig->GetType() == D3D11_DEVICE_CONTEXT_IMMEDIATE);

	return this;
}

void reshade::d3d11::device_context_impl::flush_immediate_command_list() const
{
	assert(_orig->GetType() == D3D11_DEVICE_CONTEXT_IMMEDIATE);

	_orig->Flush();
}

// VUGGER_ADDON: BEGIN
void reshade::d3d11::device_context_impl::finish_command_list(api::command_list **command_list, bool restore_state)
{
	ID3D11CommandList *native_command_list = nullptr;

	const HRESULT hr = _orig->FinishCommandList(restore_state, &native_command_list);
	if (SUCCEEDED(hr))
	{
		assert(native_command_list != nullptr);

		command_list_impl* command_list_proxy = new command_list_impl(get_device(), native_command_list);		// VUGGER_ADDON
		*command_list = command_list_proxy;
	}
}
// VUGGER_ADDON: END
