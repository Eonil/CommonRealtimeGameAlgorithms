//
//  ObjectPack.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/30.
//
//

#pragma once

#include "../CommonRealtimeGameAlgorithmsCommon.h"
#include "MemoryStorage.h"
#include <bitset>

EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_BEGIN








/*
 Implement this later...
 */








///*!
// More compact/batched version of `ObjectSlot`.
// 
// Length of a pack is fixed by hardware system.
// 
// @param
// LEN
// It's recommended to set this to a multiplication of machine native word-size.
// */
//template <typename T, Size const LEN>
//class
//ObjectPack : ExceptionSupportTools
//{
//	using	OCCS	=	std::bitset<LEN>;
//	using	MEM		=	MemoryStorage<T>;
//	using	MEMS	=	std::array<MEM, LEN>;
//	
//	static_assert(sizeof(MEMS) == sizeof(MEM) * LEN, "Unsupported system. Cannot satisfy precise memory layout assumption.");
//	
//public:
//	~ObjectPack();
//	
//	auto	capacity() const -> Size;
//	auto	size() const -> Size;
//	auto	empty() const -> bool;
//	auto	full() const -> bool;
//	
//	/*!
//	 Resolves index from a pointer to a value.
//	 
//	 @note
//	 This uses pointer address arithmetics to calculate offset.
//	 */
//	auto	index(T const*) const -> Size;
////	auto	index(ConstIterator) const -> Size;
//	
//	auto	at(Size const& index) const -> T const&;
//	auto	at(Size const& index) -> T&;
//	
////	auto	begin() const -> ConstIterator;
////	auto	begin() -> Iterator;
////	auto	end() const -> ConstIterator;
////	auto	end() -> Iterator;
//	
//	template <typename... ARGS>
//	auto	emplace(Size const index, ARGS&&... args) -> void;
//	auto	insert(Size const index, T const& v) -> void;
//	auto	insert(Size const index, T&& v) -> void;
//	auto	erase(Size const index) -> void;
//	auto	clear() -> void;
//	
//private:
//	OCCS	_occupations	{};
//	MEMS	_memories		{};
//	
//	static_assert(sizeof(_occupations) == sizeof(typename std::aligned_storage<sizeof(OCCS)>::type), "The bitset is not aligned properly. That may affect performance, and prohibited");
//};
//
//
//
//
//
//
//
//
//
//
//template <typename T, Size const LEN>
//ObjectPack<T,LEN>::
//~ObjectPack()
//{
//	if (USE_EXCEPTION_CHECKINGS)
//	{
//		_halt_if_this_is_null();
//	}
//	
//	////
//	
//	clear();
//}
//
//template <typename T, Size const LEN> auto
//ObjectPack<T,LEN>::
//at(const Size &index) const -> T const&
//{
//	if (USE_EXCEPTION_CHECKINGS)
//	{
//		_halt_if_this_is_null();
//		error_if(not _occupations[index], "No value was set to the slot at index.");
//	}
//	
//	////
//	
//	return	_memories.at(index).value();
//}
//
//template <typename T, Size const LEN> auto
//ObjectPack<T,LEN>::
//clear() -> void
//{
//	if (USE_EXCEPTION_CHECKINGS)
//	{
//		_halt_if_this_is_null();
//	}
//	
//	////
//	
//	for (Size i=0; i<LEN; i++)
//	{
//		if (_occupations[i])
//		{
//			_memories.at(i).terminate();
//		}
//	}
//}




















EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END

