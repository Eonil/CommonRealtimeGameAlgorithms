//
//  StaticStableListMap.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/29.
//
//

#pragma once

#include "../CommonRealtimeGameAlgorithmsCommon.h"
#include "ObjectSlot.h"
#include "ObjectSlotIterator.h"

EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_BEGIN









/*!
 A map which maps an integer to a value slot.
 Capacity is statically fixed.
 
 @classdesign
 The internal storage starts with uninitialize memory block, and will be
 initialized when you insert, and deinitialized when you erase.

 This object tracks and keeps occupation state of each slots, and this increases
 instance size of this object.
 
 @discussion
 This is very dense at full with a small padding for marking occupation of each slots.
 If map is not full, there can be many empty slots, and filled slots can be treated as sparse list.

 @exception
 All methods guarantees strong safety as far as the type `T` provides strong safety for all methods.
 Anyway those guarantees are applied only on debug build. There's no check, safety
 guarantee or throwing on release build. (data just be will corrupted.)
 */
template <typename T, Size const LEN>
class
StaticStableListMap
{
	using	ITEM	=	ObjectSlot<T>;
	using	ITEMS	=	std::array<ITEM, LEN>;
	
	
	
public:
	using	ConstIterator		=	ObjectSlotIterator<T const>;
	using	Iterator			=	ObjectSlotIterator<T>;
	
	class
	Exception : std::logic_error
	{
	public:
		using	logic_error::logic_error;
	};
	
	
	
public:
	StaticStableListMap() = default;
	StaticStableListMap(StaticStableListMap&&);
	StaticStableListMap(StaticStableListMap const&);
	~StaticStableListMap();
	
	auto	operator=(StaticStableListMap const& o) -> StaticStableListMap&;
	auto	operator=(StaticStableListMap&& o) -> StaticStableListMap&;
	
	////
	
	auto	empty() const -> bool;
	auto	capacity() const -> Size;
	auto	size() const -> Size;
	
	auto	data() const -> T const*;
	auto	data() -> T*;
	
	auto	index(ConstIterator) const -> Size;
	
	auto	at(Size const& index) const -> T const&;
	auto	at(Size const& index) -> T&;
	
	auto	begin() const -> ConstIterator;
	auto	begin() -> Iterator;
	auto	end() const -> ConstIterator;
	auto	end() -> Iterator;
	
	template <typename... ARGS>
	auto	emplace(Size const index, ARGS&&... args) -> void;
	auto	insert(Size const index, T const& v) -> void;
	auto	insert(Size const index, T&& v) -> void;
	auto	erase(Size const index) -> void;
	auto	clear() -> void;

	
	
	
private:
	static_assert(LEN > 0, "Zero-length is not supported.");
	
	static bool const	USE_EXCEPTIONS		=	(EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE == 1);
	
	Size	_count		{0};					//	I think it would be better to place size at first.
	ITEMS	_items		{};
	
	inline auto			ASSERT_PROPER_INSTANCE() const -> void;
	static inline auto	_except_if(bool condition, str const& message) -> void;
};












//template <typename T, Size const LEN>
//StaticStableListMap<T,LEN>::
//StaticStableListMap(StaticStableListMap const& o) : _count(o._count)
//{
//	for (Size i=0; i<LEN; i++)
//	{
//		if (o._items[i].occupation())
//		{
//			_items[i].initialize(o._items[i].value());				//	Copy-assignment.
//		}
//	}
//}
//template <typename T, Size const LEN>
//StaticStableListMap<T,LEN>::
//StaticStableListMap(StaticStableListMap&& o) : _count(std::move(o._count))
//{
//	for (Size i=0; i<LEN; i++)
//	{
//		if (o._items[i].occupation())
//		{
//			_items[i].initialize(std::move(o._items[i].value()));	//	Copy-assignment.
//		}
//	}
//}
template <typename T, Size const LEN>
StaticStableListMap<T,LEN>::
StaticStableListMap(StaticStableListMap const& o) : _count(o._count), _items(o._items)
{
}
template <typename T, Size const LEN>
StaticStableListMap<T,LEN>::
StaticStableListMap(StaticStableListMap&& o) : _count(std::move(o._count)), _items(std::move(o._items))
{
}

template <typename T, Size const LEN>
StaticStableListMap<T,LEN>::
~StaticStableListMap()
{
	ASSERT_PROPER_INSTANCE();
	
	clear();
}

template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
operator=(const StaticStableListMap &o) -> StaticStableListMap&
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(&o != nullptr);
	_except_if(&o == nullptr, "Cannot copy-assign from a value pointed by a `nullptr`.");
	ASSERT_PROPER_INSTANCE();
	
	if (&o != this)
	{
		StaticStableListMap	copy	=	o;
		std::swap(_count, copy._count);
		std::swap(_items, copy._items);
	}
	return	*this;
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
operator=(StaticStableListMap &&o) -> StaticStableListMap&
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(&o != nullptr);
	_except_if(&o == nullptr, "Cannot move-assign from a value pointed by a `nullptr`.");
	ASSERT_PROPER_INSTANCE();

	if (&o != this)
	{
		_count	=	std::move(o._count);
		_items	=	std::move(o._items);
	}
	return	*this;
}
//template <typename T, Size const LEN> auto
//StaticStableListMap<T,LEN>::
//operator=(const StaticStableListMap &o) -> StaticStableListMap&
//{
//	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(&o != nullptr);
//	_except_if(&o == nullptr, "Cannot copy-assign from a value pointed by a `nullptr`.");
//	ASSERT_PROPER_INSTANCE();
//	
//	if (&o == this)
//	{
//		//	Do nothing.
//	}
//	else
//	{
//		for (Size i=0; i<LEN; i++)
//		{
//			if (o._items[i].occupation())
//			{
//				_items[i].initialize(o._items[i].value());
//			}
//		}
//	}
//	return	*this;
//}
//template <typename T, Size const LEN> auto
//StaticStableListMap<T,LEN>::
//operator=(StaticStableListMap &&o) -> StaticStableListMap&
//{
//	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(&o != nullptr);
//	_except_if(&o == nullptr, "Cannot move-assign from a value pointed by a `nullptr`.");
//	ASSERT_PROPER_INSTANCE();
//	
//	if (&o == this)
//	{
//		//	Do nothing.
//	}
//	else
//	{
//		for (Size i=0; i<LEN; i++)
//		{
//			if (o._items[i].occupation())
//			{
//				_items[i].initialize(std::move(o._items[i].value()));
//			}
//		}
//	}
//	
//	/*
//	 The source object is filled with objects with no guts. Anyway they will be destroyed by the source object itself.
//	 */
//	
//	return	*this;
//}











template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
empty() const -> bool
{
	ASSERT_PROPER_INSTANCE();
	return	_count == 0;
}

template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
capacity() const -> Size
{
	ASSERT_PROPER_INSTANCE();
	return	LEN;
}

template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
size() const -> Size
{
	ASSERT_PROPER_INSTANCE();
	return	_count;
}


template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
data() const -> T const*
{
	ASSERT_PROPER_INSTANCE();
	return	&(_items[0].value());
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
data() -> T*
{
	ASSERT_PROPER_INSTANCE();
	return	&(_items[0].value());
}

template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
index(ConstIterator o) const -> Size
{
	ITEM const*	ptr1	=	_items.data();
	ITEM const*	ptr2	=	(ITEM const*)o;
	Size		offset	=	ptr2 - ptr1;
	return		offset;
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
at(Size const& index) const -> T const&
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except_if(index >= LEN, "Specified index is out of range.");
	}
	
	return	_items[index].value();
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
at(Size const& index) -> T&
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except_if(index >= LEN, "Specified index is out of range.");
	}
	
	return	_items[index].value();
}

template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
begin() const -> ConstIterator
{
	return	_items.data();
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
begin() -> Iterator
{
	return	_items.data();
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
end() const -> ConstIterator
{
	return	_items.data() + LEN;
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
end() -> Iterator
{
	return	_items.data() + LEN;
}






template <typename T, Size const LEN>
template <typename ...ARGS> auto
StaticStableListMap<T,LEN>::
emplace(Size const index, ARGS&&... args) -> void
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except_if(index >= LEN, "The specified index is out of range.");
		_except_if(_items[index].occupation(), "The slot at the index is already occupied.");
	}
	
	_items[index].initialize(std::forward<ARGS>(args)...);
	_count++;
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
insert(Size const index, const T &v) -> void
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except_if(index >= LEN, "The specified index is out of range.");
		_except_if(_items[index].occupation(), "The slot at the index is already occupied.");
	}
	
	_items[index].initialize(v);
	_count++;
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
insert(Size const index, T &&v) -> void
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except_if(index >= LEN, "The specified index is out of range.");
		_except_if(_items[index].occupation(), "The slot at the index is already occupied.");
	}
	
	_items[index].initialize(std::move(v));
	_count++;
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
erase(Size const index) -> void
{
	ASSERT_PROPER_INSTANCE();
	if (USE_EXCEPTIONS)
	{
		_except_if(index >= LEN, "The specified index is out of range.");
		_except_if(not _items[index].occupation(), "The slot at the index is not occupied.");
	}
	
	_count--;
	_items[index].terminate();
}
template <typename T, Size const LEN> auto
StaticStableListMap<T,LEN>::
clear() -> void
{
	ASSERT_PROPER_INSTANCE();
	
//	if (std::is_trivially_destructible<T>::value)
//	{
//		//!	Type `T` destructor doesn't need to be called. Just do nothing.
//		//!	@ref http://www.cplusplus.com/reference/type_traits/is_trivially_destructible/
//	}
//	else
	{
		//	Deinitialize all existing instances.
		for (Size i=0; i<LEN; i++)
		{
			if (_items[i].occupation())
			{
				_items[i].terminate();
			}
		}
	}
	_count	=	0;
}































template <typename T, Size const LEN> inline auto
StaticStableListMap<T,LEN>::
ASSERT_PROPER_INSTANCE() const -> void
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
}
template <typename T, Size const LEN> inline auto
StaticStableListMap<T,LEN>::
_except_if(bool condition, const str &message) -> void
{
	if (condition)
	{
		throw	Exception(message);
	}
}






















EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END

