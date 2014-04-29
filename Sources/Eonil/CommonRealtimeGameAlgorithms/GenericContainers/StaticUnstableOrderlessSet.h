//
//  StaticUnstableOrderlessSet.h
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
StaticUnstableOrderlessSet
{
public:
	class
	Exception : public std::logic_error
	{
	public:
		using	logic_error::logic_error;
	};
	
public:
	auto	empty() const -> bool;
	auto	size() const -> Size;
	
	auto	begin() const -> T const*;
	auto	begin() -> T*;
	auto	end() const -> T const*;
	auto	end() -> T*;
		
	auto	insert(T const&) -> void;
	auto	insert(T&&) -> void;
	auto	erase(T*) -> void;
	auto	clear() -> void;
	
private:
	using	ITEM	=	MemoryStorage<T>;
	using	ITEMS	=	std::array<ITEM, LEN>;
	
	Size	_count	{};
	ITEMS	_slots	{};
	
	static bool const		USE_EXCEPTIONS	=	(EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE == 1);
	static auto				_except_if(bool const condition, str const& message) -> void;
	auto					_last() -> MemoryStorage<T>&;
	
	
	
#if EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE
private:
	friend class		StaticUnstableOrderlessSet_DebuggingSupport;
#endif
};


















template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
empty() const -> bool
{
	return	_count == 0;
}
template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
size() const -> Size
{
	return	_count;
}

template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
begin() const -> T const*
{
	return	&_slots.at(0).value();
}
template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
begin() -> T*
{
	return	&_slots.at(0).value();
}
template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
end() const -> T const*
{
	return	&(_slots.data() + _count)->value();
}
template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
end() -> T*
{
	return	&(_slots.data() + _count)->value();
}



template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
insert(T const &o) -> void
{
	if (USE_EXCEPTIONS)
	{
		_except_if(_count == LEN, "This set is full.");
	}
	
	_slots.at(_count).initialize(o);
	_count++;
}
template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
insert(T &&o) -> void
{
	if (USE_EXCEPTIONS)
	{
		_except_if(_count == LEN, "This set is full.");
	}
	
	_slots.at(_count).initialize(std::move(o));
	_count++;
}
template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
erase(T *ptr1) -> void
{
	if (USE_EXCEPTIONS)
	{
		_except_if(_count == 0, "This set is empty.");
	}
	
	if (ptr1 != &_last().value())
	{
		/*
		 Pull last to here.
		 */
		ITEM*	ptr2	=	reinterpret_cast<ITEM*>(ptr1);
		ptr2->terminate();
		ptr2->initialize(std::move(_last().value()));
	}
	
	_count--;
	_slots.at(_count).terminate();
}
template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
clear() -> void
{
	for (Size i=0; i<_count; i++)
	{
		_slots.at(i).terminate();
	}
	_count	=	0;
}












template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
_except_if(const bool condition, const str &message) -> void
{
	if (condition)
	{
		throw	 Exception(message);
	}
}
template <typename T, Size const LEN> auto
StaticUnstableOrderlessSet<T,LEN>::
_last() -> MemoryStorage<T>&
{
	if (USE_EXCEPTIONS)
	{
		_except_if(_count == 0, "No items, no last item.");
	}
	return	_slots.at(_count-1);
}










#if EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE
struct
StaticUnstableOrderlessSet_DebuggingSupport
{
	template<typename T, Size const LEN>
	static auto
	get_item_slot_at(StaticUnstableOrderlessSet<T,LEN>& set, Size const index) -> typename StaticUnstableOrderlessSet<T,LEN>::ITEM&
	{
		return	set._slots.at(index);
	}
};
#endif











EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END


