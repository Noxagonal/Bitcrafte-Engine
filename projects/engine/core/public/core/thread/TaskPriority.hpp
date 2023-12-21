#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/Text.hpp>



namespace bc {
namespace thread {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class TaskPriority : uint32_t
{
	REALTIME		= 0,	///< Task must be run immediately and always before any other tasks regardless how old they are.

	HIGH,					///< Task should be run within this frame.
	ABOVE_NORMAL,			///< Task should be run within this or the next frame.
	NORMAL,					///< Task should be run within a few frames.
	BELOW_NORMAL,			///< Task should be run within 100 frames.
	LOW,					///< Task can be run whenever possible.

	BACKGROUND,				///< Task can be run when there's absolutely nothing else happening. eg, cleanup.
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TextView							TaskPriorityToText(
	const TaskPriority			&	task_priority
);



} // thread
} // bc
