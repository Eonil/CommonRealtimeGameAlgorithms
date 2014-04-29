//
//  StaticStableOrderlessSet.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/29.
//
//

#pragma once

#include "../CommonRealtimeGameAlgorithmsCommon.h"
#include "StaticStableListStack.h"
#include "StaticStableListMap.h"

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
StaticStableOrderlessSet
{
public:
	class
	Exception : public std::logic_error
	{
	public:
		using	logic_error::logic_error;
	};
	
	using	ConstIterator	=	ObjectSlotIterator<T const>;
	using	Iterator		=	ObjectSlotIterator<T>;
	
public:
	auto	empty() const -> bool;
	auto	size() const -> Size;
	
	auto	begin() const -> ConstIterator;
	auto	begin() -> Iterator;
	auto	end() const -> ConstIterator;
	auto	end() -> Iterator;
	
	auto	insert(T const&) -> Iterator;
	auto	insert(T&&) -> Iterator;
	auto	erase(Iterator) -> void;
	auto	clear() -> void;
	
private:
	StaticStableListMap<T, LEN>			_items			{};
	StaticStableListStack<Size, LEN>	_free_slot_idxs	{};		//	Use circular queue as soon as ready.
	
	static bool const		USE_EXCEPTIONS	=	(EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE == 1);
	static auto				_except_if(bool const condition, str const& message) -> void;
	auto					_feed_available_slot_idx() -> Size;								//	You must consume fed index. If you don't use it, it will leak.
};











template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
size() const -> Size
{
	return	_items.size();
}
template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
begin() const -> ConstIterator
{
	return	_items.begin();
}
template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
begin() -> Iterator
{
	return	_items.begin();
}
template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
end() const -> ConstIterator
{
	return	_items.end();
}
template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
end() -> Iterator
{
	return	_items.end();
}
template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
insert(const T &o) -> Iterator
{
	if (USE_EXCEPTIONS)
	{
		_except_if(_items.size() == LEN, "This set is full.");
	}
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_items.size() < LEN);		//	Also catches insane size overflow error if it happens...
	
	////
	
	Size	idx1	=	_feed_available_slot_idx();
	_items.insert(idx1, o);
	
	return	_items.begin() + idx1;
}
template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
insert(T &&o) -> Iterator
{
	if (USE_EXCEPTIONS)
	{
		_except_if(_items.size() == LEN, "This set is full.");
	}
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_items.size() < LEN);		//	Also catches insane size overflow error if it happens...
	
	////
	
	Size	idx1	=	_feed_available_slot_idx();
	_items.insert(idx1, std::move(o));
	
	return	_items.begin() + idx1;
}
template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
erase(Iterator o) -> void
{
	if (USE_EXCEPTIONS)
	{
		_except_if(_items.size() == 0, "This set is empty.");
		_except_if(_items.begin() > o, "The iterator is out of range.");
		_except_if(_items.end() <= o, "The iterator is out of range.");
	}
	
	////
	
	ConstIterator	o2	=	o;
	Size	idx1	=	_items.index(o2);
	_items.erase(idx1);
	_free_slot_idxs.push(idx1);
}
template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
clear() -> void
{
	_items.clear();
	_free_slot_idxs.clear();
}



















template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
_except_if(const bool condition, const str &message) -> void
{
	if (condition)
	{
		throw	Exception(message);
	}
}

template <typename T, Size const LEN> auto
StaticStableOrderlessSet<T,LEN>::
_feed_available_slot_idx() -> Size
{
	if (_free_slot_idxs.size() == 0)
	{
		return	_items.size();
	}
	else
	{
		Size	idx1	=	_free_slot_idxs.back();
		_free_slot_idxs.pop();
		return	idx1;
	}
}

















EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END
