//
//  CommonRealtimeGameAlgorithmsCommon.cpp
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/29.
//
//

#include "CommonRealtimeGameAlgorithmsCommon.h"

#include <iostream>
#include "Exception.h"

EONIL_COMMON_REALTIME_GAME_ALGORITHMS_BEGIN





#if		EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_MODE


auto
EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DEBUG_ASSERT(bool const condition, std::string const& message) -> void
{
	if (not condition)
	{
		std::cout << message << "\n";
		std::terminate();
	}
}




#endif


		
auto
halt_if(bool const condition, std::string const& message) -> void
{
	if (condition)
	{
		throw	Halt(str("[Eonil::CommonRealtimeGameAlgorithms::Halt] (UNRECOVERABLE!!!) ") + message);
	}
}
auto
error_if(bool const condition, std::string const& message) -> void
{
	if (condition)
	{
		throw	Error(str("[Eonil::CommonRealtimeGameAlgorithms::Error] ") + message);
	}
}


		
		
		
		
		
		
auto
ExceptionSupportTools::_halt_if_this_is_null() const -> void
{
	halt_if(this == nullptr, "You cannot call any instance method on `nullptr`.");
}
		
EONIL_COMMON_REALTIME_GAME_ALGORITHMS_END
























