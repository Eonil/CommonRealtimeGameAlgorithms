//
//  ObjectSlot.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/29.
//
//

#pragma once

#include "../CommonRealtimeGameAlgorithmsCommon.h"
#include "MemoryStorage.h"

EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_BEGIN






/*!
 A `MemoryStorage` with explicit occupation flag and copy/move support.
 
 @classdesign
 This is designed to be laid out consecutively. Iterating is provided by `ObjectSlotIterator`.
 Last slot must be a sential by being marked with sentinel flag to provide iteration properly.
 
 @exception
 All methods guarantees strong exception safety as long as the type `T` provides strong exception
 safety for all of these methods. (no state change if there's an exception thrown)
 
 -	constructors
 -	destructors
 -	copy assignment
 -	move assignment
 
 @note
 The only difference with the `T` itself is, init/term operation is fully manual.
 If you delete this object without calling `terminate` method, the destructor of `T`
 will not be called. That's usually wrong program, and you're responsible to call the
 `terminate` method.
 
 @warning
 Do not use this class for usual object management. This class is designed specially
 for specific classes.
 */
template <typename T>
class
ObjectSlot : ExceptionSupportTools
{
public:
	class
	Exception : public std::logic_error
	{
	public:
		using	logic_error::logic_error;
	};
	
public:
	ObjectSlot() = default;
	ObjectSlot(ObjectSlot const&);
	ObjectSlot(ObjectSlot&&);
	~ObjectSlot();
	
	auto	operator=(ObjectSlot const&) -> ObjectSlot&;
	auto	operator=(ObjectSlot&&) -> ObjectSlot&;
	
	////
	
	auto	occupation() const -> bool;
	
//	auto	data() const -> T const*;				//!	Returns just address of the value memory regardless of occupancy.
//	auto	data() -> T*;							//!	Returns just address of the value memory regardless of occupancy.
	
	auto	value() const -> T const&;
	auto	value() -> T&;
	
	template <typename... ARGS>
	auto	initialize(ARGS&&... args) -> void;
	auto	initialize(T&&) -> void;
	auto	terminate() -> void;
	
	auto	sentinel() const -> bool;
	auto	sentinelize() -> void;
	
public:
	/*!
	 Gets proper pointer address to a `ObjectSlot` from given pointer address to `T`
	 by assuming the `T` object is placed in an `ObjectSlot` object.
	 
	 @discussion
	 The memory for the object (`_mem`) is expected to be at offset `0`.
	 But I am not sure that whether that is guaranteed by the standard,
	 so this function is provided. 
	 
	 This is all about address calculation, and does not check object validity.
	 */
	static auto		resolveAddressOfSlot(T const*) -> ObjectSlot const*;
//	static auto		resolveAddressOfValue(ObjectSlot const*) -> T const*;		//	Just use `value` method instead of this indirection.
	
	
private:
	friend class	ObjectSlotDebugginSupport;
	
	MemoryStorage<T>	_mem			=	{};
	struct
	{
		bool			_occupation		=	false;
		bool			_is_last		=	false;
	};
	
	////
	
	auto	_halt_if_memory_layout_is_bad() const -> void;
};





















template <typename T>
ObjectSlot<T>::
ObjectSlot(ObjectSlot const& o) : _is_last(o._is_last)
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(&o == nullptr, "The supplied pointer shouldn't be `nullptr`.");
	}
	
	////
	
	if (o.occupation())
	{
		initialize(o.value());
	}
}
template <typename T>
ObjectSlot<T>::
ObjectSlot(ObjectSlot&& o) : _is_last(std::move(o._is_last))
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(&o == nullptr, "The supplied pointer shouldn't be `nullptr`.");
	}
	
	////
	
	if (o.occupation())
	{
		initialize(std::move(o.value()));
	}
	
	/*
	 The `o` will be left as occupied and live state with no guts.
	 Somewhere else in the program will `terminate` the `o` eventually.
	 */
}
template <typename T>
ObjectSlot<T>::
~ObjectSlot()
{
	/*
	 This assertion is only for debugging convenience.
	 */
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(not _occupation, "This slot is going to be destroyed, but still being occupied.");
}


template <typename T> auto
ObjectSlot<T>::operator=(const ObjectSlot<T> &o) -> ObjectSlot&
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(&o == nullptr, "Cannot copy-assign from a value pointed by a `nullptr`.");
	}
	
	////
	
	if (&o == this)
	{
		//	Nothing to do.
	}
	else
	{
		if (_occupation)
		{
			terminate();
		}
		if (o._occupation)
		{
			initialize(o.value());
		}
		
		_is_last	=	o._is_last;
	}
	return	*this;
}
template <typename T> auto
ObjectSlot<T>::operator=(ObjectSlot<T> &&o) -> ObjectSlot&
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(&o == nullptr, "Cannot move-assign from a value pointed by a `nullptr`.");
	}
	
	////
	
	if (&o == this)
	{
		//	Nothing to do.
	}
	else
	{
		if (_occupation)
		{
			terminate();
		}
		if (o._occupation)
		{
			initialize(std::move(o.value()));
		}
		
		_is_last	=	std::move(o._is_last);
	}
	return	*this;
}
template <typename T> auto
ObjectSlot<T>::occupation() const -> bool
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
	}
	
	////
	
	return	_occupation;
}
//template <typename T> auto
//ObjectSlot<T>::data() const -> T const*
//{
//	if (USE_EXCEPTION_CHECKINGS)
//	{
//		_halt_if_this_is_null();
//	}
//	
//	////
//	
//	return	&_mem.value();
//}
//template <typename T> auto
//ObjectSlot<T>::data() -> T*
//{
//	if (USE_EXCEPTION_CHECKINGS)
//	{
//		_halt_if_this_is_null();
//	}
//	
//	////
//	
//	return	&_mem.value();
//}
template <typename T> auto
ObjectSlot<T>::value() const -> T const&
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(not _occupation, "This object-slot is not occupied yet.");
	}
	
	////
	
	return	_mem.value();
}
template <typename T> auto
ObjectSlot<T>::value() -> T&
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(not _occupation, "This object-slot is not occupied yet.");
	}
	
	////
	
	return	_mem.value();
}

template <typename T>
template <typename ...ARGS> auto
ObjectSlot<T>::
initialize(ARGS&&... args) -> void
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(_occupation, "This object-slot is already occupied.");
	}
	
	////
	
	_mem.initialize(std::forward<ARGS>(args)...);
	_occupation	=	true;
}
template <typename T> auto
ObjectSlot<T>::
initialize(T&& o) -> void
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(_occupation, "This object-slot is already occupied.");
	}
	
	////
	
	_mem.initialize(std::move(o));
	_occupation	=	true;
}
template <typename T> auto
ObjectSlot<T>::
terminate() -> void
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(not _occupation, "This object-slot is not occupied yet.");
	}
	
	////
	
	_occupation	=	false;
	_mem.terminate();
}
template <typename T> auto
ObjectSlot<T>::
sentinel() const -> bool
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
	}
	
	////
	
	return	_is_last;
}

template <typename T> auto
ObjectSlot<T>::
sentinelize() -> void
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		_halt_if_this_is_null();
		_halt_if_memory_layout_is_bad();
		error_if(_is_last, "This object-slot is alreday a sentinel.");
	}
	
	////
	
	_is_last	=	true;
}


template <typename T> auto
ObjectSlot<T>::
resolveAddressOfSlot(const T *o) -> ObjectSlot const*
{
	if (USE_EXCEPTION_CHECKINGS)
	{
		error_if(o == nullptr, "`nullptr` is not supported.");
	}
	
	////
	
	return	reinterpret_cast<ObjectSlot const*>(o);
	
	/*
	 Trick to get slot pointer from value pointer.
	 Kept for legacy archive purpose.
	 */
//	static constexpr Size const		byte_offset		=	offsetof(ObjectSlot, _mem);
//	uint8_t const*					target_ptr		=	reinterpret_cast<uint8_t const*>(o);
//	uint8_t const*					slot_ptr1		=	target_ptr - byte_offset;
//	ObjectSlot const*				slot_ptr2		=	reinterpret_cast<ObjectSlot const*>(slot_ptr1);
//	
//	if (USE_EXCEPTION_CHECKINGS)
//	{
//		halt_if(target_ptr != slot_ptr1);
//	}
//
//	return	slot_ptr2;
}




















template <typename T> auto
ObjectSlot<T>::
_halt_if_memory_layout_is_bad() const -> void
{
	halt_if(uintptr_t(this) != uintptr_t(&_mem), "Bad memory layout.");
}





//#if EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE
//struct
//ObjectSlotDebugginSupport
//{
//	template <typename T>
//	static inline auto
//	get_value_ptr_without_check(ObjectSlot<T>& o) -> T&
// 	{
//		return	o._mem.value();
//	}
//};
//#endif










EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END

