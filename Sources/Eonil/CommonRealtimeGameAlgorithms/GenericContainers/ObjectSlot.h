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
ObjectSlot
{
	friend class	ObjectSlotDebugginSupport;

	MemoryStorage<T>	_mem;
	bool				_occupation{false};
	
	static bool const	USE_EXCEPTIONS		=	(EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE == 1);
	
	static auto			_except_if(bool const condition, str const& message) -> void;
	
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
	
	auto	value() const -> T const&;
	auto	value() -> T&;
	
	template <typename... ARGS>
	auto	initialize(ARGS&&... args) -> void;
	auto	initialize(T&&) -> void;
	auto	terminate() -> void;
};













template <typename T> auto
ObjectSlot<T>::
_except_if(const bool condition, const str &message) -> void
{
	if (condition)
	{
		throw	Exception(message);
	}
}

template <typename T>
ObjectSlot<T>::
ObjectSlot(ObjectSlot const& o)
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(&o != nullptr);
	
	if (o.occupation())
	{
		initialize(o.value());
	}
}
template <typename T>
ObjectSlot<T>::
ObjectSlot(ObjectSlot&& o)
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(&o != nullptr);
	
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
	if (USE_EXCEPTIONS)
	{
		_except_if(&o == nullptr, "Cannot copy-assign from a value pointed by a `nullptr`.");
	}
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
	
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
	}
	return	*this;
}
template <typename T> auto
ObjectSlot<T>::operator=(ObjectSlot<T> &&o) -> ObjectSlot&
{
	if (USE_EXCEPTIONS)
	{
		_except_if(&o == nullptr, "Cannot copy-assign from a value pointed by a `nullptr`.");
	}
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
	
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
	}
	return	*this;
}
template <typename T> auto
ObjectSlot<T>::occupation() const -> bool
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
	return	_occupation;
}
template <typename T> auto
ObjectSlot<T>::value() const -> T const&
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
	if (USE_EXCEPTIONS)
	{
		_except_if(not _occupation, "This object-slot is not occupied yet.");
	}
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_occupation);
	
	return	_mem.value();
}
template <typename T> auto
ObjectSlot<T>::value() -> T&
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
	if (USE_EXCEPTIONS)
	{
		_except_if(not _occupation, "This object-slot is not occupied yet.");
	}
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_occupation);
	
	return	_mem.value();
}

template <typename T>
template <typename ...ARGS> auto
ObjectSlot<T>::
initialize(ARGS&&... args) -> void
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
	if (USE_EXCEPTIONS)
	{
		_except_if(_occupation, "This object-slot is already occupied.");
	}
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(not _occupation);
	
	_mem.initialize(std::forward<ARGS>(args)...);
	_occupation	=	true;
}
template <typename T> auto
ObjectSlot<T>::
initialize(T&& o) -> void
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
	if (USE_EXCEPTIONS)
	{
		_except_if(_occupation, "This object-slot is already occupied.");
	}
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(not _occupation);
	
	_mem.initialize(std::move(o));
	_occupation	=	true;
}
template <typename T> auto
ObjectSlot<T>::
terminate() -> void
{
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(this != nullptr);
	if (USE_EXCEPTIONS)
	{
		_except_if(not _occupation, "This object-slot is not occupied yet.");
	}
	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(_occupation);
	
	_occupation	=	false;
	_mem.terminate();
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

