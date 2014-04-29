//
//  TestSuite1.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/29.
//
//

#ifndef __CommonRealtimeGameAlgorithms__TestSuite1__
#define __CommonRealtimeGameAlgorithms__TestSuite1__

#include <exception>
#include <array>
#include <Eonil/CommonRealtimeGameAlgorithms.h>

using namespace	Eonil::CommonRealtimeGameAlgorithms::GenericContainers;

inline auto
test_assert(bool const cond) -> void
{
	if (not cond)
	{
		std::terminate();
	}
}

inline auto
always_have_some_exception(std::function<auto()->void> const& f1) -> bool
{
	bool	any_exc	=	false;
	try
	{
		f1();
	}
	catch(...)
	{
		any_exc	=	true;
	}
	return	any_exc;
}
inline auto
never_have_any_exception(std::function<auto()->void> const& f1) -> bool
{
	return	not always_have_some_exception(f1);
}
inline auto
test_assert_always_exception(std::function<auto()->void> const& f1) -> void
{
	test_assert(always_have_some_exception(f1));
}
inline auto
test_assert_never_exception(std::function<auto()->void> const& f1) -> void
{
	test_assert(never_have_any_exception(f1));
}











inline auto
test_array_value() -> void
{
	std::array<int, 3>	a1	=	{11,22,33};
	std::array<int, 3>	a2	=	a1;
	
	test_assert(a2.at(0) == 11);
	test_assert(a2.at(1) == 22);
	test_assert(a2.at(2) == 33);
	
	{
		
		struct
		TT1
		{
			int	v	=	1;
			TT1(int v) : v(v) {}
			TT1(TT1&& o) : v(o.v)
			{
				o.v	=	0;
			}
		};
		std::array<TT1, 3>	a1	=	{TT1{11}, TT1{22}, TT1{33}};
		std::array<TT1, 3>	a2	=	std::move(a1);
		
		test_assert(a1.at(0).v == 0);
		test_assert(a1.at(1).v == 0);
		test_assert(a1.at(2).v == 0);
		
		test_assert(a2.at(0).v == 11);
		test_assert(a2.at(1).v == 22);
		test_assert(a2.at(2).v == 33);
	}
}

inline auto
test_memory_storage() -> void
{
	{
		static bool	bad_timing	=	true;
		struct
		TestType1
		{
			TestType1()
			{
				if (bad_timing)
				{
					throw	std::logic_error("CTOR called!");
				}
			}
			~TestType1()
			{
				if (bad_timing)
				{
					throw	std::logic_error("DTOR called!");
				}
			}
		};
		
		MemoryStorage<TestType1>	ms1	{};
		bad_timing	=	false;
		test_assert_never_exception([&](){ ms1.initialize(); });
		test_assert_never_exception([&](){ ms1.terminate(); });
		bad_timing	=	true;
	}
}

inline auto
test_object_slot() -> void
{
	{
		static bool	bad_timing	=	true;
		struct
		TestType1
		{
			TestType1()
			{
				if (bad_timing)
				{
					throw	std::logic_error("CTOR called!");
				}
			}
			~TestType1()
			{
				if (bad_timing)
				{
					throw	std::logic_error("DTOR called!");
				}
			}
		};
		
		bad_timing	=	true;
		ObjectSlot<TestType1>	slot1	{};
		bad_timing	=	false;
		test_assert(slot1.occupation() == false);
		slot1.initialize();
		test_assert(slot1.occupation() == true);
		slot1.terminate();
		test_assert(slot1.occupation() == false);
		bad_timing	=	true;
		test_assert_always_exception([&](){ slot1.initialize(); });
		test_assert(slot1.occupation() == false);
		test_assert_always_exception([&](){ slot1.terminate(); });
		test_assert(slot1.occupation() == false);
	}

	{
		struct
		TestType1
		{
			int	v		=	1;
			
			TestType1(int v) : v(v) {}
		};
		
		ObjectSlot<TestType1>	slot1	{};
		test_assert(not slot1.occupation());
		slot1.initialize(222);
		test_assert(slot1.occupation());
		
		ObjectSlot<TestType1>	slot2	{slot1};
		test_assert(slot2.occupation());
		test_assert(slot2.value().v == 222);
		
		slot1.terminate();
		test_assert(not slot1.occupation());
		ObjectSlot<TestType1>	slot3	{slot1};
		test_assert(not slot3.occupation());
		
		slot2.terminate();
	}
	{
		static int	dtor_count	=	0;
		struct
		TestType1
		{
			int	v		=	1;
			
			TestType1(int v) : v(v) {}
			~TestType1()
			{
				dtor_count++;
			}
		};
		
		{
			ObjectSlot<TestType1>	slot1	{};
			slot1.initialize(222);
			
			ObjectSlot<TestType1>	slot2	{std::move(slot1)};
			test_assert(slot1.occupation());			//	Guts are moved out, but the object itself still alive.
			test_assert(slot2.occupation());
			test_assert(slot2.value().v == 222);
			
			slot1.terminate();
			slot2.terminate();
		}
		
		test_assert(dtor_count == 2);
	}
}

inline auto
test_static_stable_list_stack() -> void
{
	{
		using	STACK	=	StaticStableListStack<int, 3>;
		STACK	st1	{};
		test_assert(st1.size() == 0);
		st1.push(111);
		st1.push(222);
		test_assert_always_exception([&](){ st1.at(2); });
		test_assert_never_exception([&](){ st1.push(333); });
		test_assert_always_exception([&](){ st1.push(444); });
		test_assert(st1.size() == 3);
		test_assert(st1.at(0) == 111);
		test_assert(st1.at(1) == 222);
		test_assert(st1.at(2) == 333);
		test_assert(st1.data()[0] == 111);
		test_assert(st1.data()[1] == 222);
		test_assert(st1.data()[2] == 333);
		test_assert(st1.front() == 111);
		test_assert(st1.back() == 333);
		
		STACK	st2	{st1};
		test_assert(st2.at(0) == 111);
		test_assert(st2.at(1) == 222);
		test_assert(st2.at(2) == 333);
		
		st1.pop();
		st1.pop();
		test_assert_never_exception([&](){ st1.pop(); });
		test_assert_always_exception([&](){ st1.pop(); });
		test_assert(st1.size() == 0);
		test_assert_always_exception([&](){ st1.front(); });
		test_assert_always_exception([&](){ st1.at(0); });
		test_assert_never_exception([&](){ st1.data(); });		//	Data can be accessd always because it means just memory rather than an object or a value.
		
		
		{
			int	sum1	=	0;
			for (auto const& a1: st2)
			{
				sum1	+=	a1;
			}
			test_assert(sum1 == 111 + 222 + 333);
		}
		
		{
			int	vs[]	=	{55, 66, 77};
			int c = 0;
			for (auto& a1: st2)
			{
				a1	=	vs[c];
				c++;
			}
			test_assert(st2.at(0) == 55);
			test_assert(st2.at(1) == 66);
			test_assert(st2.at(2) == 77);
		}
		
	}
	
	{
		static int	ctor_count	=	0;
		static int	dtor_count	=	0;
		
		static int	copy_ctor_count	=	0;
		static int	move_ctor_count	=	0;
		
		struct
		TestType1
		{
			int	v	=	0;
			TestType1() = delete;
			TestType1(int v) : v(v)
			{
				ctor_count++;
			}
			TestType1(TestType1 const& o) : v(o.v)
			{
				copy_ctor_count++;
			}
			TestType1(TestType1&& o) : v(std::move(o.v))
			{
				move_ctor_count++;
			}
			~TestType1()
			{
				dtor_count++;
			}
			
			/*
			 The stack shouldn't use assignment at all.
			 */
			auto	operator=(TestType1 const&) -> TestType1& = delete;
			auto	operator=(TestType1&&) -> TestType1& = delete;
		};
		
		{
			using	STACK	=	StaticStableListStack<TestType1, 3>;
			STACK	st1	{};
			test_assert(st1.size() == 0);
			st1.emplace(111);
			st1.emplace(222);
			st1.emplace(333);
			test_assert(ctor_count == 3);
			test_assert_always_exception([&](){ st1.emplace(444); });
			test_assert(st1.size() == 3);
			
			STACK	st2	{st1};
			test_assert(copy_ctor_count == 3);
			test_assert(st2.size() == 3);
			test_assert(st2.at(0).v == 111);
			test_assert(st2.at(1).v == 222);
			test_assert(st2.at(2).v == 333);
			st2.pop();
			st2.pop();
			st2.pop();
			test_assert(st2.size() == 0);
			test_assert(dtor_count == 3);
			
			STACK	st3	{std::move(st1)};
			test_assert(move_ctor_count == 3);
			test_assert(dtor_count == 3);			//	Nothing more should be destroyed yet. It's just moved.
			test_assert(st3.size() == 3);
			test_assert(st3.at(0).v == 111);
			test_assert(st3.at(1).v == 222);
			test_assert(st3.at(2).v == 333);
			test_assert(st1.size() == 3);			//	Stack must keep all the elements (without guts) after moved.
		}
		
		test_assert(dtor_count == 3 * 3);			//	3 created and pushed, copied once of 3 atoms, and moved once of 3 atoms. Total 9 objects has been destroyed.
	}
	
	{
		using	STACK	=	StaticStableListStack<std::string, 3>;
		STACK	s1		=	{};
		STACK	s2		=	{};
		s1.push("AAA");
		s1.push("BBB");
		s1.push("CCC");
		s2.push("EEE");
		s2.push("FFF");
		s2.push("GGG");
		
		STACK	tmp(std::move(s1));
		test_assert(tmp.size() == 3);
		test_assert(tmp.at(0) == "AAA");
		test_assert(tmp.at(1) == "BBB");
		test_assert(tmp.at(2) == "CCC");
		
		s1				=	std::move(s2);
		test_assert(s1.size() == 3);
		test_assert(s1.at(0) == "EEE");
		test_assert(s1.at(1) == "FFF");
		test_assert(s1.at(2) == "GGG");
		
		s2				=	std::move(tmp);
		test_assert(s2.size() == 3);
		test_assert(s2.at(0) == "AAA");
		test_assert(s2.at(1) == "BBB");
		test_assert(s2.at(2) == "CCC");
	}
	{
		using	STACK	=	StaticStableListStack<std::string, 3>;
		STACK	s1	{};
		STACK	s2	{};
		s1.push("AAA");
		s1.push("BBB");
		s1.push("CCC");
		std::swap(s1, s2);
		test_assert(s1.size() == 0);
		test_assert(s2.size() == 3);
		test_assert(s2.at(0) == "AAA");
		test_assert(s2.at(1) == "BBB");
		test_assert(s2.at(2) == "CCC");
	}
}

inline auto
test_list_map() -> void
{
	{
		using	MAP		=	StaticStableListMap<int, 3>;
		MAP		map1	{};
		
		test_assert(map1.size() == 0);
		test_assert_always_exception([&](){ map1.at(0); });
		test_assert_always_exception([&](){ map1.at(1); });
		test_assert_always_exception([&](){ map1.at(2); });
		
		map1.insert(0, 111);
		map1.insert(1, 222);
		map1.insert(2, 333);
		test_assert(map1.size() == 3);
		test_assert_always_exception([&](){ map1.insert(3, 444); });
		
		map1.erase(0);
		map1.erase(1);
		map1.erase(2);
		test_assert(map1.size() == 0);
		test_assert_always_exception([&](){ map1.at(0); });
		test_assert_always_exception([&](){ map1.at(1); });
		test_assert_always_exception([&](){ map1.at(2); });
	}
	{
		using	MAP		=	StaticStableListMap<int, 3>;
		MAP		map1	{};
		
		map1.insert(0, 111);
		map1.insert(1, 222);
		map1.insert(2, 333);
		
		int		sum1	=	0;
		for (auto const& a1: map1)
		{
			sum1	+=	a1;
		}
		test_assert(sum1 == 111 + 222 + 333);
	}
	{
		using	MAP		=	StaticStableListMap<int, 5>;
		MAP		map1	{};
		
		map1.insert(1, 222);
		map1.insert(3, 333);
		test_assert(map1.size() == 2);
		
		int		cc1		=	0;
		int		sum1	=	0;
		for (auto const& a1: map1)
		{
			sum1	+=	a1;
			cc1		++;
		}
		test_assert(sum1 == 555);
		test_assert(cc1 == 2);
		
		{
			MAP		map2	{map1};
			map2	=	map2;
			test_assert(map2.size() == 2);
			int		cc1		=	0;
			int		sum1	=	0;
			for (auto const& a1: map2)
			{
				sum1	+=	a1;
				cc1		++;
			}
			test_assert(sum1 == 555);
			test_assert(cc1 == 2);
			
			////
			
			MAP		map3	{std::move(map2)};
			map3	=	map3;
			test_assert(map3.size() == 2);
			int		cc3		=	0;
			int		sum3	=	0;
			for (auto const& a3: map3)
			{
				sum3	+=	a3;
				cc3		++;
			}
			test_assert(sum3 == 555);
			test_assert(cc3 == 2);
			
		}
	}
	{
		using	MAP		=	StaticStableListMap<std::string, 3>;
		MAP		map1	=	{};
		map1.insert(0, "AAA");
		map1.insert(1, "BBB");
		map1.insert(2, "CCC");
		test_assert(map1.index(map1.at(0)) == 0);
		test_assert(map1.index(map1.at(1)) == 1);
		test_assert(map1.index(map1.at(2)) == 2);
	}
}

inline auto
test_static_unstable_orderless_set() -> void
{
	using	DBG	=	StaticUnstableOrderlessSet_DebuggingSupport;
	StaticUnstableOrderlessSet<int, 5>	s1	{};
	
	test_assert(s1.size() == 0);
	s1.insert(111);
	s1.insert(222);
	s1.insert(333);
	test_assert(s1.size() == 3);
	s1.insert(444);
	s1.insert(555);
	test_assert(s1.size() == 5);
	test_assert_always_exception([&](){ s1.insert(66); });
	test_assert(s1.size() == 5);
	test_assert(DBG::get_item_slot_at(s1, 0).value() == 111);
	test_assert(DBG::get_item_slot_at(s1, 1).value() == 222);
	test_assert(DBG::get_item_slot_at(s1, 2).value() == 333);
	test_assert(DBG::get_item_slot_at(s1, 3).value() == 444);
	test_assert(DBG::get_item_slot_at(s1, 4).value() == 555);
	
	{
		std::vector<int>	vs	{};
		for (int const& v1: s1)
		{
			vs.push_back(v1);
		}
		test_assert(vs == std::vector<int>{111,222,333,444,555});
	}
	
	int*	vptr1	=	&DBG::get_item_slot_at(s1, 2).value();
	s1.erase(vptr1);
	
	auto&	item_slot	=	DBG::get_item_slot_at(s1, 2);
	test_assert(item_slot.value() == 555);
	test_assert(s1.size() == 4);
	
	{
		std::vector<int>	vs	{};
		for (int const& v1: s1)
		{
			vs.push_back(v1);
		}
		test_assert(vs == std::vector<int>{111,222,555,444});
	}
}

inline auto
test_static_stable_orderless_set() -> void
{
	auto
	get_all_atoms	=	[](StaticStableOrderlessSet<int, 3> const& s) -> std::set<int>
	{
		std::set<int>	vs{};
		for (int a: s)
		{
			vs.insert(a);
		}
		return	vs;
	};
	
	using	SET		=	StaticStableOrderlessSet<int, 3>;
	SET	s1	{};
	SET::Iterator	it1	=	s1.insert(111);
	SET::Iterator	it2	=	s1.insert(222);
	SET::Iterator	it3	=	s1.insert(333);
	test_assert(s1.size() == 3);
	test_assert(get_all_atoms(s1) == std::set<int>{111,222,333});
	test_assert_always_exception([&](){ s1.insert(444); });
	test_assert(s1.size() == 3);
	test_assert(get_all_atoms(s1) == std::set<int>{111,222,333});
	s1.erase(it2);
	test_assert(s1.size() == 2);
	test_assert(get_all_atoms(s1) == std::set<int>{111,333});
	s1.erase(it1);
	test_assert(s1.size() == 1);
	test_assert(get_all_atoms(s1) == std::set<int>{333});
	s1.erase(it3);
	test_assert(get_all_atoms(s1) == std::set<int>{});
	test_assert(s1.size() == 0);
	
	{
		SET	s1	{};
		s1.insert(111);
		auto it1	=	s1.insert(222);
		s1.insert(333);
		s1.erase(it1);
		s1.insert(222);
		std::set<int>	s2	{};
		ObjectSlotRange<int>	r1{s1};
		for (auto const& a: r1)
		{
			s2.insert(a);
		}
		test_assert(s2 == std::set<int>{111,222,333});
	}
}



//template <typename T>
//struct
//slot
//{
//};
//template <typename T>
//struct
//it
//{
//	using	JUST_T	=	typename std::remove_const<T>::type;
//	using	SLOT	=	typename std::conditional<std::is_const<T>::value, slot<JUST_T> const, slot<JUST_T>>::type;
//	
//	SLOT*	ptr;
//	
//	it(SLOT* ptr) : ptr(ptr)
//	{
//	}
//	
//	//	operator it<T const>()
//	//	{
//	//		slot<T> const*	ptr2	=	ptr;
//	//		return	it<int const>(ptr2);
//	//	}
//};


inline auto
test_all() -> void
{
	test_memory_storage();
	test_object_slot();
	test_static_stable_list_stack();
	test_list_map();
	test_static_unstable_orderless_set();
	test_static_stable_orderless_set();
	
	
	
//	slot<int>		slot2	=	{};
//	it<int>			it2		=	&slot2;
//	it<int const>	it3		=	(slot<int> const*)it2.ptr;
}

















#endif /* defined(__CommonRealtimeGameAlgorithms__TestSuite1__) */
