//
//  TestSuite2.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/30.
//
//

#pragma once

#include "TestCommon.h"

#include <memory>
#include <cmath>










inline auto
test_mem_alignment() -> void
{
	
	{
		struct
		ITEM
		{
			char	a, b, c;
		};
		std::array<ITEM, 3>	a1{};
		test_log(sizeof(ITEM));
		test_log(sizeof(a1));
		test_assert(sizeof(a1) == sizeof(ITEM) * 3);
		a1.at(0).a	=	1;
		a1.at(1).a	=	1;
	}
	
	
	////
	
	test_assert(sizeof(uint32_t) == 4);
	test_assert(sizeof(std::float_t) == 4);
	
	{
		struct
		ITEM
		{
			uint32_t		a;
		};
		std::array<ITEM, 3>	a1{};
		test_log(sizeof(ITEM));
		test_log(sizeof(a1));
		test_assert(sizeof(a1) == sizeof(ITEM) * 3);
		a1.at(0).a	=	1;
		a1.at(1).a	=	1;
	}
	
	{
		struct
		ITEM
		{
			std::float_t	a;
		};
		std::array<ITEM, 3>	a1{};
		test_log(sizeof(ITEM));
		test_log(sizeof(a1));
		test_assert(sizeof(a1) == sizeof(ITEM) * 3);
		a1.at(0).a	=	1;
		a1.at(1).a	=	1;
	}

	{
		struct
		ITEM
		{
			uint32_t		a;
			bool			f1, f2;
		};
		std::array<ITEM, 3>	a1{};
		test_log(sizeof(ITEM));
		test_log(sizeof(a1));
		test_assert(sizeof(a1) == sizeof(ITEM) * 3);
		a1.at(0).a	=	1;
		a1.at(1).a	=	1;
	}
	
	{
		struct
		ITEM
		{
			std::float_t	a;
			bool			f1, f2;
		};
		std::array<ITEM, 3>	a1{};
		test_log(sizeof(ITEM));
		test_log(sizeof(a1));
		test_assert(sizeof(a1) == sizeof(ITEM) * 3);
		a1.at(0).a	=	1;
		a1.at(1).a	=	1;
	}

	{
		using	ITEM	=	ObjectSlot<uint32_t>;
		using	ARR		=	std::array<ObjectSlot<uint32_t>, 3>;
		ARR	a1{};
		test_log(sizeof(ITEM));
		test_log(sizeof(ARR));
		/*
		 Compiler will put extra padding to provide memory alignment.
		 This is possible because the type `T` is known at compiler time
		 on `ObjectSlot` type.
		 */
		test_assert(sizeof(ITEM) == (4+2)+2);
		test_assert(sizeof(a1) == ((4+2)+2) * 3);
		a1.at(0).initialize(1);
		a1.at(1).initialize(2);
		test_assert(a1.at(0).value() == 1);
		test_assert(a1.at(1).value() == 2);
		a1.at(0).terminate();
		a1.at(1).terminate();
	}
	
	{
		std::array<ObjectSlot<std::float_t>, 3>	a1{};
		test_assert(sizeof(a1) == ((4+2)+2) * 3);
		a1.at(0).initialize(1);
		a1.at(1).initialize(2);
		test_assert(a1.at(0).value() == 1);
		test_assert(a1.at(1).value() == 2);
		a1.at(0).terminate();
		a1.at(1).terminate();
	}
	
	
	
//	std::array<ObjectSlot<char>, 366>	a1{};
//	for (int i=0; i<366; i ++)
//	{
//		a1[i].initialize(i);
//	}
//	for (int i=0; i<366; i ++)
//	{
//		std::cout << a1[i].value() << "\n";
//	}
//	for (int i=0; i<366; i ++)
//	{
//		a1[i].terminate();
//	}
	
	
	
	
	
	
	
	
	
//	struct
//	AAA
//	{
//		char aaa[3];
//	};
//	std::array<ObjectSlot<AAA>, 3> a2{};
//	a2.at(1).initialize();
//	auto v1 = a2.at(1).value();
//	a2.at(1).value().aaa[0] = 1;
//	a2.at(1).terminate();
//	
//	////
//	
//	StaticStableListMap<AAA, 3>	m1{};
//	m1.insert(0, {});
//	m1.insert(1, {});
//	m1.insert(2, {});
//	
//	std::cout << sizeof(m1) << "\n";
//	std::cout << uintptr_t(&m1) << "\n";
//	std::cout << uintptr_t(&m1.at(0).aaa) << "\n";
//	std::cout << uintptr_t(&m1.at(1).aaa) << "\n";
//	std::cout << uintptr_t(&m1.at(2).aaa) << "\n";
//	
//	uintptr_t	map_ptr	=	uintptr_t(&m1);
//	uintptr_t	a0_ptr	=	uintptr_t(&m1.at(0));
//	uintptr_t	a1_ptr	=	uintptr_t(&m1.at(1));
//	uintptr_t	a2_ptr	=	uintptr_t(&m1.at(2));
//	
//	uintptr_t	diff_o		=	a0_ptr - map_ptr;
//	uintptr_t	diff_0_1	=	a1_ptr - a0_ptr;
//	uintptr_t	diff_1_2	=	a2_ptr - a1_ptr;
//	
//	test_assert(diff_o % sizeof(uintptr_t) == 0);
//	test_assert(diff_0_1 % sizeof(uintptr_t) == 0);
//	test_assert(diff_1_2 % sizeof(uintptr_t) == 0);
//	
}

inline auto
raise_EXC_ARM_DA_ALIGN() -> void
{
	char mem[16];
	char* p1 = mem;
	double* p2 = (double*)(p1 + 1);
	*p2 =  10;
	
}









inline auto
test_all2() -> void
{
	test_mem_alignment();
}

