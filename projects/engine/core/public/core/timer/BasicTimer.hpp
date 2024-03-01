#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <chrono>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Simple timer that can return passed time since last call.
class BasicTimer
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline									BasicTimer()
	{
		last_time_point	= std::chrono::steady_clock::now();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Tick once. Get amount of time passed since last call to this function.
	///
	///	If first call, then returns time since this timer was created.
	/// 
	/// @return
	/// Amount of time passed in seconds.
	inline f64								Tick()
	{
		auto now		= std::chrono::steady_clock::now();
		auto delta_time	= std::chrono::duration<f64>( now - last_time_point ).count();
		last_time_point	= now;
		return delta_time;
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::chrono::steady_clock::time_point	last_time_point;
};



} // bc
