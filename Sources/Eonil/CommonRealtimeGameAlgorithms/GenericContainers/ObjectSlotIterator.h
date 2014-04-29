//
//  ObjectSlotIterator.h
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
 Single pointer sized object which wraps pointer to an object-slot.
 Accessing target value-object in the slot will become effectively zero-cost
 because it will ultimately reduced into a same pointer at machine level.
 
 It's recommended to store this iterator to point a target value.
 */
template <typename T>
class
ObjectSlotIterator final
{
	using	JUST_T	=	typename std::remove_const<T>::type;
	using	SLOT	=	typename std::conditional<std::is_const<T>::value, ObjectSlot<JUST_T> const, ObjectSlot<JUST_T>>::type;
	
	auto	_skip_all_unoccupied_slots() -> void;
	
public:
	ObjectSlotIterator() = default;
	ObjectSlotIterator(SLOT* ptr);
	
	operator ObjectSlotIterator<JUST_T const>();
	explicit operator ObjectSlot<JUST_T> const*() const;
	
public:
	auto	operator*() const -> T&;
	
	auto	operator==(ObjectSlotIterator const&) const -> bool;
	auto	operator!=(ObjectSlotIterator const&) const -> bool;
	auto	operator<(ObjectSlotIterator const&) const -> bool;
	auto	operator>(ObjectSlotIterator const&) const -> bool;
	auto	operator<=(ObjectSlotIterator const&) const -> bool;
	auto	operator>=(ObjectSlotIterator const&) const -> bool;
	auto	operator+(Size const) const -> ObjectSlotIterator;
	auto	operator-(Size const) const -> ObjectSlotIterator;
	auto	operator-(ObjectSlotIterator const&) const -> Size;

	auto	operator++() -> void;
	
private:
	SLOT*	_ptr	{nullptr};
};






template <typename T> auto
ObjectSlotIterator<T>::_skip_all_unoccupied_slots() -> void
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	while (not _ptr->occupation())
	{
		_ptr++;
	}
}
template <typename T>
ObjectSlotIterator<T>::ObjectSlotIterator(SLOT* slot_ptr) : _ptr(slot_ptr)
{
	_skip_all_unoccupied_slots();
}
template <typename T>
ObjectSlotIterator<T>::operator ObjectSlotIterator<JUST_T const>()
{
	return	_ptr;
}
template <typename T>
ObjectSlotIterator<T>::operator ObjectSlot<JUST_T> const*() const
{
	return	_ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator*() const -> T&
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr->occupation());
	
	return	_ptr->value();
}
template <typename T> auto
ObjectSlotIterator<T>::operator==(const ObjectSlotIterator &o) const -> bool
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(o._ptr != nullptr);
	return	_ptr == o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator!=(const ObjectSlotIterator &o) const -> bool
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(o._ptr != nullptr);
	return	_ptr != o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator<(const ObjectSlotIterator &o) const -> bool
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(o._ptr != nullptr);
	
	return	_ptr < o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator>(const ObjectSlotIterator &o) const -> bool
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(o._ptr != nullptr);
	
	return	_ptr > o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator<=(const ObjectSlotIterator &o) const -> bool
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(o._ptr != nullptr);
	
	return	_ptr <= o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator>=(const ObjectSlotIterator &o) const -> bool
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(o._ptr != nullptr);
	
	return	_ptr >= o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator+(const Size o) const -> ObjectSlotIterator
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	return	_ptr + o;
}
template <typename T> auto
ObjectSlotIterator<T>::operator-(const Size o) const -> ObjectSlotIterator
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	return	_ptr - o;
}
template <typename T> auto
ObjectSlotIterator<T>::operator-(ObjectSlotIterator const& o) const -> Size
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(o._ptr != nullptr);
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr >= o._ptr);
	
	Size	offset	=	_ptr - o._ptr;
	return	offset;
}
template <typename T> auto
ObjectSlotIterator<T>::operator++() -> void
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
	
	_ptr++;
	_skip_all_unoccupied_slots();
}


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	






EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END