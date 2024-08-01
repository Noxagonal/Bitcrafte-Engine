#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/Text.hpp>



namespace bc {
namespace thread {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class TaskState : u32
{
	NOT_STARTED,
	RUNNING,
	PAUSED,
	FINISHED,
	ERROR,
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::TextView								TaskStateToText(
	const TaskState						&	task_state
);



} // thread
} // bc
