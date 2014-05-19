//
//  Exception.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 14/4/30.
//
//

#pragma once
#include "CommonRealtimeGameAlgorithmsCommon.h"
EONIL_COMMON_REALTIME_GAME_ALGORITHMS_BEGIN







/*!
 Root class of categorization.
 */
class
Exception : std::logic_error
{	
protected:
	Exception() = delete;
	Exception(std::string const& domain, std::string const& category, std::string const& message);
private:
	std::string const		domain		=	"";
	std::string const		category	=	"";
	std::string const		message		=	"";
};









/*!
 Represents **recoverable** exception.
 
 @discussion
 This is thrown when the code can guarantee *strong exception safety*.
 No changes are made, and no memory leaked. You can just safely treat
 all the trials are fully rollback, and continue program execution.
 
 This is mostly about input validation.
 If the function cannot provide strong exception safety, that means 
 the execution is unrecoverable, and it should throw `Halt` instead of.
 
 This mean;
 
 -	The library has no bug on the case.
 -	Your input is wrong.
 -	So you can recover.
 
 @note
 Some code may explicitly return/output this object if they cannot afford
 exception catching cost.
 */
class
Error : public Exception
{
public:
	Error() = delete;
	Error(std::string const& domain, std::string const& message);
};







/*!
 Represents **UNRECOVERABLE** exception.
 
 @discussion
 This is thrown when unrecoverable state corruption has been dicsovered.
 Though this library guarantees strong exception safety, state can be corrupted
 by bugs. So this is a result of bug detections, mostly about logical integrity.
 
 This means;
 
 -	The library may contain a BUG.
 -	You input doesn't matter.
 -	Program should crash to prevent further damage.
 
 @warning
 NEVER try to recover from this kind of exception. Program state already corrupted.
 */
class
Crash : public Exception
{
public:
	Crash() = delete;
	Crash(std::string const& domain, std::string const& message);
};







EONIL_COMMON_REALTIME_GAME_ALGORITHMS_END

