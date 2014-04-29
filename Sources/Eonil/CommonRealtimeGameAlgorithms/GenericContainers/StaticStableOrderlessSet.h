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
	
	/*!
	 Returns index for the iterator as a concept of perfect-hash.
	 Value range N is:
	 
		0 <= N < size()

	 You can use this value as a perfect-hash key to link another object.
	 */
	auto	hash(Iterator) const -> Size;
	
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
hash(Iterator o) const -> Size
{
	return	_items.index(o);
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
