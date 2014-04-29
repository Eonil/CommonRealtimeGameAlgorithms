//
//  CommonRealtimeGameAlgorithmsCommon.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/29.
//
//

#pragma once

#ifndef EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE
#error	"You must define `EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE` to one of `1`(true) or `0`(false) in preprocessor flags."
#endif

#ifndef EONIL_COMMON_REALTIME_GAME_ALGORITHMS_HEAVY_TRACE_DEBUG_MODE
#define	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_HEAVY_TRACE_DEBUG_MODE				EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE
#endif


#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_BEGIN									namespace Eonil { namespace CommonRealtimeGameAlgorithms {
#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_END									} }

#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUGGING_TOOLS_BEGIN					namespace Eonil { namespace CommonRealtimeGameAlgorithms { namespace DebuggingTools {
#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUGGING_TOOLS_END					} } }

#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_BEGIN				namespace Eonil { namespace CommonRealtimeGameAlgorithms { namespace GenericContainers {
#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END				} } }

#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_UTILITIES_BEGIN				namespace Eonil { namespace CommonRealtimeGameAlgorithms { namespace GenericUtilities {
#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_UTILITIES_END					} } }



#include <memory>
#include <string>
#include <vector>
#include <set>



EONIL_COMMON_REALTIME_GAME_ALGORITHMS_BEGIN
using	Size		=	std::size_t;
using	str			=	std::string;
template <typename T>	using	vec		=	std::vector<T>;
#if		EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE
auto	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(bool const condition, std::string const& message = "[Eonil::CommonRealtimeGameAlgorithms] Assertion failure") -> void;
#else
#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(...)						//	Erase the macro for release mode.
#endif
#if		EONIL_COMMON_REALTIME_GAME_ALGORITHMS_HEAVY_TRACE_DEBUG_MODE
auto	EONIL_COMMON_REALTIME_GAME_ALGORITHMS_HEAVY_TRACE_DEBUG_ASSERT(bool const condition, std::string const& message = "[Eonil::CommonRealtimeGameAlgorithms] **HEAVY-TRACE** Assertion failure!!!") -> void;
#else
#define EONIL_COMMON_REALTIME_GAME_ALGORITHMS_HEAVY_TRACE_DEBUG_ASSERT(...)				//	Erase the macro for release mode.
#endif
EONIL_COMMON_REALTIME_GAME_ALGORITHMS_END



EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_BEGIN
template <typename T>						class	ObjectSlot;
template <typename T>						class	ObjectSlotIterator;
//template <typename T>						class	ObjectSlotRange;
template <typename T, Size const LEN>		class	StaticStableListStack;
template <typename T, Size const LEN>		class	StaticStableListMap;
template <typename T, Size const LEN>		class	StaticStableOrderlessSet;
template <typename T, Size const LEN>		class	StaticUnstableOrderlessSet;
EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_CONTAINERS_END



EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_UTILITIES_BEGIN
template <typename IT>						class	Range;
EONIL_COMMON_REALTIME_GAME_ALGORITHMS_GENERIC_UTILITIES_END



