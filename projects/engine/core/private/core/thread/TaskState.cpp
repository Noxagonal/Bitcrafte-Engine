
#include <core/PreCompiledHeader.hpp>
#include <core/thread/TaskState.hpp>



auto bc::thread::TaskStateToText( const TaskState & task_state ) noexcept -> TextView
{
	switch( task_state )
	{
	case TaskState::NOT_STARTED:
		return "NOT_STARTED";
	case TaskState::RUNNING:
		return "RUNNING";
	case TaskState::PAUSED:
		return "PAUSED";
	case TaskState::FINISHED:
		return "FINISHED";
	case TaskState::ERROR:
		return "ERROR";
	}

	assert( 0 );
	return "<unknown>";
}
