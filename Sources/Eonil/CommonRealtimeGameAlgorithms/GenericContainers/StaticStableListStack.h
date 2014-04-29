//
//  StaticStableListStack.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/29.
//
//

#pragma once

#include "../CommonRealtimeGameAlgorithmsCommon.h"
#include "ObjectSlot.h"

EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_BEGIN









/*!
 A stack with no allocation, and all the operations are O(1),
 but capacity is statically fixed.
 
 @classdesign
 The internal storage starts with uninitialize memory block, and will be 
 initialized when you push, and deinitialized when you pop.
 
 @exception
 All methods guarantees strong safety as far as the type `T` provides strong safety for all methods.
 Anyway those guarantees are applied only on debug build. There's no check, safety
 guarantee or throwing on release build. (data just be will corrupted.)
 */
template <typename T, Size const LEN>
class
StaticStableListStack
{
	static_assert(LEN > 0, "Zero-length is not supported.");
	
	static bool const	USE_EXCEPTIONS		=	(EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE == 1);
	
	using	ITEM	=	MemoryStorage<T>;
	using	ITEMS	=	std::array<ITEM, LEN>;
	
//	union
//	{
//		Size	_used		{0};											//	I think it would be better to place size at first.
//		typename std::aligned_storage<sizeof(ITEM)>::type	padding;		//	Align by size of the ITEM.
//	};
	
	Size	_used		{0};											//	I think it would be better to place size at first.
	ITEMS	_items		{};
	
	inline auto			ASSERT_PROPER_INSTANCE() const -> void;
	static inline auto	_except(bool condition, str const& message) -> void;
	
public:
	class
	Exception : std::logic_error
	{
	public:
		using	logic_error::logic_error;
	};
	
public:
	StaticStableListStack() = default;
	StaticStableListStack(StaticStableListStack&&);
	StaticStableListStack(StaticStableListStack const&);
	~StaticStableListStack();
	
	auto	operator=(StaticStableListStack const& o) -> StaticStableListStack&;
	auto	operator=(StaticStableListStack&& o) -> StaticStableListStack&;
	
	auto	empty() const -> bool;
	auto	capacity() const -> Size;
	auto	size() const -> Size;
	
	auto	data() const -> T const*;
	auto	data() -> T*;
	
	auto	at(Size const& index) const -> T const&;
	auto	at(Size const& index) -> T&;
	
	auto	front() const -> T const&;
	auto	front() -> T&;
	auto	back() const -> T const&;
	auto	back() -> T&;
	
	auto	begin() const -> T const*;
	auto	begin() -> T*;
	auto	end() const -> T const*;
	auto	end() -> T*;
	
	auto	clear() -> void;					//!	Resets all internal state to initial state by popping all elements. Use this instead of re-creating object to avoid reallocation.
	auto	push(T const& v) -> void;
	auto	push(T&& v) -> void;
	auto	pop() -> void;
	
	template <typename... ARGS>
	auto	emplace (ARGS&&... args) -> void;
};



template <typename T, Size const LEN> inline auto
StaticStableListStack<T,LEN>::
ASSERT_PROPER_INSTANCE() const -> void
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
}

template <typename T, Size const LEN> inline auto
StaticStableListStack<T,LEN>::
_except(bool condition, const str &message) -> void
{
	if (condition)
	{
		throw	Exception(message);
	}
}




template <typename T, Size const LEN>
StaticStableListStack<T,LEN>::
StaticStableListStack(StaticStableListStack const& o) : _used(o._used)
{
	for (Size i=0; i<o._used; i++)
	{
		_items[i].initialize(o._items[i].value());					//	Copy-assignment.
	}
}
template <typename T, Size const LEN>
StaticStableListStack<T,LEN>::
StaticStableListStack(StaticStableListStack&& o) : _used(std::move(o._used))
{
	for (Size i=0; i<o._used; i++)
	{
		_items[i].initialize(std::move(o._items[i].value()));		//	Move-assignment.
	}
	
	/*
	 Source array need to be alive though they don't have guts.
	 */
}
template <typename T, Size const LEN>
StaticStableListStack<T,LEN>::
~StaticStableListStack()
{
	ASSERT_PROPER_INSTANCE();
	
	clear();
}

template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
operator=(const StaticStableListStack &o) -> StaticStableListStack&
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(&o != nullptr);
	ASSERT_PROPER_INSTANCE();
	
	StaticStableListStack	copy{o};
	
	for (Size i=0; i<o._used; i++)
	{
		std::swap(_items[i].value(), copy._items[i].value());			//	Copy-assignment.
	}
	
	std::swap(copy._used, _used);
	return	*this;
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
operator=(StaticStableListStack &&o) -> StaticStableListStack&
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(&o != nullptr);
	ASSERT_PROPER_INSTANCE();
	
	for (Size i=0; i<o._used; i++)
	{
		_items[i].initialize(std::move(o._items[i].value()));			//	Move-assignment.
	}
	
	_used	=	std::move(o._used);
	return	*this;
}











template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
empty() const -> bool
{
	ASSERT_PROPER_INSTANCE();
	return	_used == 0;
}

template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
capacity() const -> Size
{
	ASSERT_PROPER_INSTANCE();
	return	LEN;
}

template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
size() const -> Size
{
	ASSERT_PROPER_INSTANCE();
	return	_used;
}


template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
data() const -> T const*
{
	ASSERT_PROPER_INSTANCE();
	return	&(_items[0].value());
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
data() -> T*
{
	ASSERT_PROPER_INSTANCE();
	return	&(_items[0].value());
}

template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
at(Size const& index) const -> T const&
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(index >= _used, "Specified index is out of range.");
	}
	
	return	_items[index].value();
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
at(Size const& index) -> T&
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(index >= _used, "Specified index is out of range.");
	}
	
	return	_items[index].value();
}

template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
front() const -> T const&
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(_used == 0, "This stack is empty, and there's no front element.");
	}
	
	return	at(0);
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
front() -> T&
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(_used == 0, "This stack is empty, and there's no front element.");
	}
	
	return	at(0);
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
back() const -> T const&
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(_used == 0, "This stack is empty, and there's no back element.");
	}
	
	return	at(size()-1);
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
back() -> T&
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(_used == 0, "This stack is empty, and there's no back element.");
	}
	
	return	at(size()-1);
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
begin() const -> T const*
{
	return	data();
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
begin() -> T*
{
	return	data();
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
end() const -> T const*
{
	return	data() + size();
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
end() -> T*
{
	return	data() + size();
}






template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
clear() -> void
{
	ASSERT_PROPER_INSTANCE();
	
	if (std::is_trivially_destructible<T>::value)
	{
		//!	Type `T` destructor doesn't need to be called. Just do nothing.
		//!	@ref http://www.cplusplus.com/reference/type_traits/is_trivially_destructible/
	}
	else
	{
		//	Deinitialize all existing instances.
		for (Size i=0; i<_used; i++)
		{
			_items[i].terminate();
		}
	}
	_used	=	0;

}
template <typename T, Size const LEN>
template <typename ...ARGS> auto
StaticStableListStack<T,LEN>::
emplace(ARGS&&... args) -> void
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(_used == LEN, "This stack is full. Cannot push anymore.");
	}
	
	_items[_used].initialize(std::forward<ARGS>(args)...);
	_used++;
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
push(const T &v) -> void
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(_used == LEN, "This stack is full. Cannot push anymore.");
	}
	
	_items[_used].initialize(v);
	_used++;
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
push(T &&v) -> void
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(_used == LEN, "This stack is full. Cannot push anymore.");
	}
	
	_items[_used].initialize(std::move(v));
	_used++;
}
template <typename T, Size const LEN> auto
StaticStableListStack<T,LEN>::
pop() -> void
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except(_used == 0, "This stack is empty. Cannot pop anymore.");
	}
	
	_used--;
	_items[_used].terminate();
}


















































EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END

