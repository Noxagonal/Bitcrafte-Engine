
#include <core/PreCompiledHeader.hpp>
#include <core/thread/TaskPriority.hpp>



bc::TextView bc::thread::TaskPriorityToText(
	const TaskPriority & task_state
)
{
	switch( task_state )
	{
	case TaskPriority::REALTIME:
		return "REALTIME";
	case TaskPriority::HIGH:
		return "HIGH";
	case TaskPriority::ABOVE_NORMAL:
		return "ABOVE_NORMAL";
	case TaskPriority::NORMAL:
		return "NORMAL";
	case TaskPriority::BELOW_NORMAL:
		return "BELOW_NORMAL";
	case TaskPriority::LOW:
		return "LOW";
	case TaskPriority::BACKGROUND:
		return "BACKGROUND";
	}

	assert( 0 );
	return "<unknown>";
}
