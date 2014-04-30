//
//  TestSuite2.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/30.
//
//

#pragma once

#include "TestCommon.h"











//
//
//
//
//
//
//
//struct
//Finish
//{
//	static auto
//	test1() -> void
//	{
//	}
//};
//
//template <size_t const IDX, size_t const COUNT>
//struct
//Tester1
//{
//	using	NEXT_TESTER	=	typename std::conditional<IDX < COUNT, Tester1<IDX+1, COUNT>, Finish>::type;
//	
//	///
//	
//	
//	static auto
//	test1() -> void
//	{
//		std::cout << IDX << "\n";
//		
//		using	T1	=	typename std::aligned_storage<IDX>::type;
//		StaticStableListMap<T1, 1024>	m1	{};
//		for (size_t i=0; i<1024; i++)
//		{
//			m1.insert(i, {});
//			for (size_t j=0; j<IDX; j++)
//			{
//				m1.at(i).data[j]	=	1;
//			}
//		}
//		
//		////
//		
//		NEXT_TESTER::test1();
//	}
//};
//
//
//inline auto
//test_mem_alingment() -> void
//{
//	struct
//	bad1
//	{
//		char a = 0, b = 0, c = 0;
//	};
//
//	{
//		size_t	sz1		=	sizeof(MemoryStorage<bad1>);
//		test_assert(sz1 == 3);
//		
//		size_t	sz2	=	sizeof(ObjectSlot<bad1>);
//		test_assert(sz2 == 3 + 1 + 1);
//		
//		size_t	sz3			=	sizeof(std::array<ObjectSlot<bad1>, 3>);
//		test_assert(sz3 == (3+1+1) * 3);
//	}
//	{
//		MemoryStorage<bad1>	o1	=	{};
//		size_t	sz1	=	sizeof(o1);
//		test_assert(sz1 == 3);
//		
//		ObjectSlot<bad1>	o2	=	{};
//		size_t	sz2	=	sizeof(o2);
//		test_assert(sz2 == 3 + 1 + 1);
//		
//		std::array<ObjectSlot<bad1>, 3>	o3	=	{};
//		size_t	sz3	=	sizeof(o3);
//		test_assert(sz3 == (3+1+1) * 3);
//
//		using	o3_aligned_storage	=	std::aligned_storage<sizeof(std::array<ObjectSlot<bad1>, 3>)>::type;
//		size_t	sz3_aleign			=	sizeof(o3_aligned_storage);
//		test_assert(sz3_aleign == (3+1+1) * 3 +	1);
//		
//		StaticStableListMap<bad1, 3>	o4	=	{};
//		size_t	sz4				=	sizeof(o4);
//
//		test_assert(sz4 == sizeof(o3_aligned_storage) + sizeof(size_t));
//		
//	}
//	{
////		using	K	=	StaticStableListMap<bad1, 3>;
////		K		k1	{};
////		size_t	sz1	=	sizeof(k1);
////		
////		test_assert(sz1 == sizeof(size_t) + sizeof(ObjectSlot<bad1>) * 3);
////		for (size_t i=0; i<k1.capacity(); i++)
////		{
////			k1.insert(i, {});
////		}
////		
////		for (size_t i=0; i<k1.capacity(); i++)
////		{
////			k1.at(i).a++;
////			k1.at(i).b++;
////			k1.at(i).c++;
////		}
//		
//		Tester1<1,100>::test1();
//	}
////	struct
////	Particle1
////	{
////		std::unique_ptr<size_t>	_sight_sensor	{nullptr};
////		intptr_t				_polarity		{0};
////		
////		////
////		
////		size_t			phase	{0};
////		
////		struct
////		{
////			float			flashing_intensity		=	0.0;
////			float			lighting_intensity		=	0.0;
////			size_t			decaying_life			=	0;				//	Life length...
////			size_t			decaying_age			=	0;
////			
////			size_t			jump_cooltime_in_tiks	=	0;
////		}
////		factors			{};
////		
////		class
////		KnownToAnyCounter
////		{
////			size_t	_state	{0};
////		};
////		
////		struct
////		{
////			KnownToAnyCounter	known_to_any_counter	{};
////
////		}
////		flags			{};
////		
////	};
////	
////	{
////		using	K	=	StaticStableListMap<Particle1, 64>;
////		K		k1	{};
////		
////		for (size_t i=0; i<64; i++)
////		{
////			k1.insert(0, {});
////		}
////		
////		for (size_t i=0; i<64; i++)
////		{
////			k1.at(i).factors.flashing_intensity	+= 1.0;
////		}
////	}
//}












inline auto
test_all2() -> void
{
//	test_mem_alingment();
}

