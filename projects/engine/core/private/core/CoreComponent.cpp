
#include "PreCompiledHeader.hpp"
#include <core/CoreComponent.hpp>

#include <core/diagnostic/logger/Logger.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent		*	global_core			= nullptr;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent::CoreComponent(
	const bc::CoreComponentCreateInfo & create_info
)
{
	BHardAssert( global_core == nullptr, "More than one core instance is not allowed" );
	global_core = this;

	// TODO: Start memory pool.

	logger = MakeUnique<diagnostic::Logger>( create_info.logger_create_info );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent::~CoreComponent()
{
	BHardAssert( global_core == this, "Global core singleton changed in running application" );

	// Cleanup code here...

	global_core = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::diagnostic::Logger * bc::CoreComponent::GetLogger()
{
	return logger.Get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::CoreComponent * bc::GetCore()
{
	return global_core;
}
