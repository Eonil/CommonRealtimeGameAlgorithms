//
//  ObjectSlotRange.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/29.
//
//

#pragma once

#include "../CommonRealtimeGameAlgorithmsCommon.h"
#include "ObjectSlotIterator.h"
#include "../GenericUtilities/Range.h"

EONIL_COMMON_REALTIME_GAME_ALGORITHMS_FLAT_CONTAINERS_BEGIN




template <typename T>		using	ObjectSlotRange		=	GenericUtilities::Range<ObjectSlotIterator<T>>;





///*!
// Single pointer sized object which wraps pointer to an object-slot.
// Accessing target value-object in the slot will become effectively zero-cost
// because it will ultimately reduced into a same pointer at machine level.
// 
// It's recommended to store this iterator to point a target value.
// */
//template <typename T>
//class
//ObjectSlotRange final
//{
//public:
//	ObjectSlotRange() = default;
//	ObjectSlotRange(ObjectSlotIterator<T> begin, ObjectSlotIterator<T> end);
//	
//	template <typename C>
//	ObjectSlotRange(C& container) : ObjectSlotRange(container.begin(), container.end())
//	{
//	}
//
//	
////	auto	begin() const -> ObjectSlotIterator<T const>;
//	auto	begin() -> ObjectSlotIterator<T>;
////	auto	end() const -> ObjectSlotIterator<T const>;
//	auto	end() -> ObjectSlotIterator<T>;
//	
//private:
//	ObjectSlotIterator<T>	_begin	{};
//	ObjectSlotIterator<T>	_end	{};
//};
//
//
//
//
//template <typename T>
//ObjectSlotRange<T>::
//ObjectSlotRange(ObjectSlotIterator<T> begin, ObjectSlotIterator<T> end) : _begin(begin), _end(end)
//{
//}
//
//template <typename T> auto
//ObjectSlotRange<T>::
//begin() -> ObjectSlotIterator<T>
//{
//	return	_begin;
//}
//template <typename T> auto
//ObjectSlotRange<T>::
//end() -> ObjectSlotIterator<T>
//{
//	return	_end;
//}
//

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	






EONIL_COMMON_REALTIME_GAME_ALGORITHMS_FLAT_CONTAINERS_END