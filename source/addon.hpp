/*
 * Copyright (C) 2021 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <string>
#include <vector>
#include <cassert>

template <typename T, size_t STACK_ELEMENTS = 16>
struct temp_mem
{
	explicit temp_mem(size_t elements = STACK_ELEMENTS)
	{
		if (elements > STACK_ELEMENTS)
			p = new T[elements];
		else
			p = stack;
	}
	~temp_mem()
	{
		if (p != stack)
			delete[] p;
	}

	T &operator[](size_t element)
	{
		assert(element < STACK_ELEMENTS || p != stack);

		return p[element];
	}

	T *p, stack[STACK_ELEMENTS];
};

// VUGGER_ADDON
#include <atomic>
#include <cstdlib>
#include <array>

template <typename Type, std::size_t BlockSize = 64>
class block_allocator
{
public:
	static constexpr std::size_t k_memory_allocation_alignment = alignof(Type);

	block_allocator()
	{
		static_assert(sizeof(Type) >= sizeof(node), "Type must be larger than node");

		m_blocklist = allocate_aligned<node>();
		m_blocklist->next.store(nullptr);

		m_freelist = allocate_aligned<node>();
		m_freelist->next.store(nullptr);
	}

	~block_allocator()
	{
		delete_aligned_list(m_blocklist);
		//delete_aligned_list(m_freelist);
#if defined(_MSC_VER)
		_aligned_free(m_freelist);
#else
		std::free(m_freelist);
#endif
	}

	Type *allocate()
	{
		node *element = pop_atomic(m_freelist);

		if (element == nullptr) {
			std::size_t sub_block_size = ((sizeof(Type) - 1) / k_memory_allocation_alignment + 1) * k_memory_allocation_alignment;

			node *blockptr = allocate_aligned<node>((sub_block_size * BlockSize) + k_memory_allocation_alignment);
			push_atomic(m_blocklist, blockptr);

			std::uint8_t *block = reinterpret_cast<std::uint8_t *>(blockptr) + k_memory_allocation_alignment;

			for (std::size_t i = BlockSize - 1; i > 0; --i)
			{
				node *sub_block = reinterpret_cast<node *>(block + (sub_block_size * i));
				push_atomic(m_freelist, sub_block);
			}

			element = pop_atomic(m_freelist);
		}

		assert(element);
		return reinterpret_cast<Type *>(element);
	}

	void free(Type *element)
	{
		push_atomic(m_freelist, reinterpret_cast<node *>(element));
	}

	bool contains(const Type *ptr) const
	{
		std::uint8_t *byte_ptr = reinterpret_cast<std::uint8_t *>(const_cast<Type *>(ptr));
		node *block = m_blocklist->next.load();

		while (block)
		{
			std::uint8_t *block_start = reinterpret_cast<std::uint8_t *>(block) + k_memory_allocation_alignment;
			std::size_t sub_block_size = ((sizeof(Type) - 1) / k_memory_allocation_alignment + 1) * k_memory_allocation_alignment;

			std::uint8_t *block_end = block_start + (sub_block_size * BlockSize);

			if (byte_ptr >= block_start && byte_ptr < block_end)
			{
				std::uintptr_t offset = byte_ptr - block_start;
				if (offset % sub_block_size == 0)
				{
					return true;
				}
			}

			block = block->next.load();
		}

		return false;
	}

protected:
	struct node
	{
		std::atomic<node *> next;
	};

	node *m_freelist;
	node *m_blocklist;

	template <typename T>
	static T *allocate_aligned(std::size_t size = sizeof(T))
	{
#if defined(_MSC_VER)
		return reinterpret_cast<T *>(_aligned_malloc(size, k_memory_allocation_alignment));
#else
		return reinterpret_cast<T *>(std::aligned_alloc(k_memory_allocation_alignment, size));
#endif
	}

	static void delete_aligned_list(node *list)
	{
		node *current = list->next.load();
		while (current) {
			node *next = current->next.load();
#if defined(_MSC_VER)
			_aligned_free(current);
#else
			std::free(current);
#endif
			current = next;
		}

#if defined(_MSC_VER)
		_aligned_free(list);
#else
		std::free(list);
#endif
	}

	static void push_atomic(node *head, node *element)
	{
		node *next = head->next.load();
		do {
			element->next.store(next);
		} while (!head->next.compare_exchange_weak(next, element));
	}

	static node *pop_atomic(node *head)
	{
		node *next = head->next.load();
		node *desired;
		do
		{
			if (next == nullptr)
			{
				return nullptr;
			}
			desired = next->next.load();
		} while (!head->next.compare_exchange_weak(next, desired));

		return next;
	}
};

#define DECLARE_MEM(_CLASS_, _BLOCK_SIZE_) \
protected: \
	static block_allocator<_CLASS_, _BLOCK_SIZE_>	ms_allocator; \
public: \
	inline static void* operator new (std::size_t) { return ms_allocator.allocate(); } \
	inline static void* operator new (std::size_t, void* ptr) { return ptr; } \
	inline static void operator delete (void* ptr) { if(ptr) ms_allocator.free(static_cast<_CLASS_*>(ptr)); } \
	inline static bool contains(void *ptr) { return ptr ? ms_allocator.contains(static_cast<_CLASS_*>(ptr)) : false; }

#define DECLARE_MEM_STATIC(_CLASS_, _BLOCK_SIZE_) inline block_allocator<_CLASS_, _BLOCK_SIZE_>	_CLASS_::ms_allocator; 
// VUGGER_ADDON

namespace reshade::api
{
	template <typename T, typename... api_object_base>
	class api_object_impl : public api_object_base...
	{
		static_assert(sizeof(T) <= sizeof(uint64_t));

	public:
		api_object_impl(const api_object_impl &) = delete;
		api_object_impl &operator=(const api_object_impl &) = delete;

		void get_private_data(const uint8_t guid[16], uint64_t *data) const override
		{
			for (auto it = _private_data.begin(); it != _private_data.end(); ++it)
			{
				if (std::memcmp(it->guid, guid, 16) == 0)
				{
					*data = it->data;
					return;
				}
			}

			*data = 0;
		}
		void set_private_data(const uint8_t guid[16], const uint64_t data)  override
		{
			for (auto it = _private_data.begin(); it != _private_data.end(); ++it)
			{
				if (std::memcmp(it->guid, guid, 16) == 0)
				{
					if (data != 0)
						it->data = data;
					else
						_private_data.erase(it);
					return;
				}
			}

			if (data != 0)
			{
				_private_data.push_back({ data, {
					reinterpret_cast<const uint64_t *>(guid)[0],
					reinterpret_cast<const uint64_t *>(guid)[1] } });
			}
		}

		uint64_t get_native() const override final { return (uint64_t)_orig; }		// VUGGER_ADDON

		T _orig;

	protected:
		template <typename... Args>
		explicit api_object_impl(T orig, Args... args) : api_object_base(std::forward<Args>(args)...)..., _orig(orig) {}
		virtual ~api_object_impl()			// VUGGER_ADDON:
		{
			// All user data should ideally have been removed before destruction, to avoid leaks
			assert(_private_data.empty());
		}

	private:
		struct private_data
		{
			uint64_t data;
			uint64_t guid[2];
		};

		std::vector<private_data> _private_data;
	};

	struct api_object;
	struct effect_runtime;
}

#if RESHADE_ADDON

namespace reshade
{
	struct addon_info
	{
		struct overlay_callback
		{
			std::string title;
			void(*callback)(api::effect_runtime *) = nullptr;
		};

		void *handle = nullptr;
#if !RESHADE_ADDON_LITE
		bool  loaded = false;
#endif
		std::string name;
		std::string description;
		std::string file;
		std::string author;
		std::string version;

		std::vector<std::pair<uint32_t, void *>> event_callbacks;
#if RESHADE_GUI
		void(*settings_overlay_callback)(api::effect_runtime *) = nullptr;
		std::vector<overlay_callback> overlay_callbacks;
#endif
	};
}

#endif
