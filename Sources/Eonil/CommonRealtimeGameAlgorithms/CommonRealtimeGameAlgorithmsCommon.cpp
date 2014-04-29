//
//  CommonRealtimeGameAlgorithmsCommon.cpp
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/29.
//
//

#include "CommonRealtimeGameAlgorithmsCommon.h"

#include <iostream>

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








EONIL_COMMON_REALTIME_GAME_ALGORITHMS_END
