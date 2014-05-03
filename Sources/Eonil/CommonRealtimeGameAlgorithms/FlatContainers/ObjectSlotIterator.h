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
 A forward-only iterator. (no random access)
 
 @discussion
 Single pointer sized object which wraps pointer to an object-slot.
 Accessing target value-object in the slot will become effectively zero-cost
 because it will ultimately reduced into a same pointer at machine level.
 
 It's recommended to store this iterator to point a target value.
 
 @note
 Uses `nullptr` for past-the-end marker.
 */
template <typename T>
class
ObjectSlotIterator final : ExceptionSupportTools
{
	using	JUST_T	=	typename std::remove_const<T>::type;
	using	SLOT	=	typename std::conditional<std::is_const<T>::value, ObjectSlot<JUST_T> const, ObjectSlot<JUST_T>>::type;
	
public:
	ObjectSlotIterator() = default;
	ObjectSlotIterator(SLOT* ptr);
	
	operator ObjectSlotIterator<JUST_T const>();
	explicit operator ObjectSlot<JUST_T> const*() const;
	
public:
	auto	operator*() const -> T&;
	auto	operator->() const -> T*;
	
	auto	operator==(ObjectSlotIterator const&) const -> bool;
	auto	operator!=(ObjectSlotIterator const&) const -> bool;
	auto	operator<(ObjectSlotIterator const&) const -> bool;
	auto	operator>(ObjectSlotIterator const&) const -> bool;
	auto	operator<=(ObjectSlotIterator const&) const -> bool;
	auto	operator>=(ObjectSlotIterator const&) const -> bool;
//	auto	operator+(Size const) const -> ObjectSlotIterator;
//	auto	operator-(Size const) const -> ObjectSlotIterator;
//	auto	operator-(ObjectSlotIterator const&) const -> Size;

	auto	operator++() -> void;
	
private:
	SLOT*	_ptr	{nullptr};
	
	auto	_halt_if_current_iterator_is_bad() const -> void;
	auto	_halt_if_supplied_iterator_is_bad(ObjectSlotIterator const& o) const -> void;
	
	auto	_skip_all_unoccupied_slots(bool advance_one_unconditionally) -> void;
	
};

















template <typename T>
ObjectSlotIterator<T>::ObjectSlotIterator(SLOT* slot_ptr) : _ptr(slot_ptr)
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		
		/*
		 Current iterator can be bad at this point.
		 */
	}
	
	////
	
	_skip_all_unoccupied_slots(false);
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
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_current_iterator_is_bad();
		error_if(_ptr == nullptr, "Current iterator is the end one. Cannot dereference.");
	}
	
	////
	
	return	_ptr->value();
}
template <typename T> auto
ObjectSlotIterator<T>::operator->() const -> T*
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_current_iterator_is_bad();
		error_if(_ptr == nullptr, "Current iterator is the end one. Cannot dereference.");
	}
	
	////
	
	return	&_ptr->value();
}
template <typename T> auto
ObjectSlotIterator<T>::operator==(const ObjectSlotIterator &o) const -> bool
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_current_iterator_is_bad();
		_halt_if_supplied_iterator_is_bad(o);
	}
	
	////
	
	return	_ptr == o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator!=(const ObjectSlotIterator &o) const -> bool
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_current_iterator_is_bad();
		_halt_if_supplied_iterator_is_bad(o);
	}
	
	////
	
	return	_ptr != o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator<(const ObjectSlotIterator &o) const -> bool
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_current_iterator_is_bad();
		_halt_if_supplied_iterator_is_bad(o);
	}
	
	////
	
	return	_ptr < o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator>(const ObjectSlotIterator &o) const -> bool
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_current_iterator_is_bad();
		_halt_if_supplied_iterator_is_bad(o);
	}
	
	////
	
	return	_ptr > o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator<=(const ObjectSlotIterator &o) const -> bool
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_current_iterator_is_bad();
		_halt_if_supplied_iterator_is_bad(o);
	}
	
	////
	
	return	_ptr <= o._ptr;
}
template <typename T> auto
ObjectSlotIterator<T>::operator>=(const ObjectSlotIterator &o) const -> bool
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_current_iterator_is_bad();
		_halt_if_supplied_iterator_is_bad(o);
	}
	
	////
	
	return	_ptr >= o._ptr;
}
//template <typename T> auto
//ObjectSlotIterator<T>::operator+(const Size o) const -> ObjectSlotIterator
//{
//	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
//	return	_ptr + o;
//}
//template <typename T> auto
//ObjectSlotIterator<T>::operator-(const Size o) const -> ObjectSlotIterator
//{
//	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
//	return	_ptr - o;
//}
//template <typename T> auto
//ObjectSlotIterator<T>::operator-(ObjectSlotIterator const& o) const -> Size
//{
//	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr != nullptr);
//	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(o._ptr != nullptr);
//	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_ptr >= o._ptr);
//	
//	Size	offset	=	_ptr - o._ptr;
//	return	offset;
//}
template <typename T> auto
ObjectSlotIterator<T>::operator++() -> void
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_current_iterator_is_bad();
	}
	
	////
	
	_skip_all_unoccupied_slots(true);
}


	
	
	
	
	
	
	
	
template <typename T> auto
ObjectSlotIterator<T>::_halt_if_current_iterator_is_bad() const -> void
{
//	halt_if(_ptr == nullptr, "Current iterator is bad.");
	halt_if(_ptr != nullptr and _ptr->occupation() == false, "Current iterator is bad. Object slot is not occupied.");
}
template <typename T> auto
ObjectSlotIterator<T>::_halt_if_supplied_iterator_is_bad(const ObjectSlotIterator &o) const -> void
{
//	halt_if(o._ptr == nullptr, "Supplied iterator is bad.");
	halt_if(o._ptr != nullptr and o._ptr->occupation() == false, "Supplied iterator is bad. Object slot is not occupied.");
}


	
	
/*!
 As a result, current iterator will become valid one
 or (past-the-)end one.
 */
template <typename T> auto
ObjectSlotIterator<T>::_skip_all_unoccupied_slots(bool advance_one_unconditionally) -> void
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		
		/*
		 Current iterator can be bad at this point.
		 */
	}
	
	////

	while (advance_one_unconditionally or (_ptr != nullptr and not _ptr->occupation()))
	{
		advance_one_unconditionally	=	false;
		if (_ptr->sentinel())
		{
			_ptr	=	nullptr;
			return;
		}
		else
		{
			_ptr++;
		}
	}
}
	
	
	
	






EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END