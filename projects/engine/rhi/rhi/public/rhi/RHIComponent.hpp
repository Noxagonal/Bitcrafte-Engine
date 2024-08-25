#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <rhi/RHIComponentCreateInfo.hpp>
#include <rhi/RHIComponentStartInfo.hpp>

#include <core/containers/List.hpp>
#include <core/containers/Text.hpp>
#include <core/containers/UniquePtr.hpp>



namespace bc {
namespace window_manager { class WindowManagerComponent; }
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RHIComponent
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIComponent( const RHIComponentCreateInfo& create_info );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~RHIComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Start( const RHIComponentStartInfo& rhi_start_info ) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual auto GetGraphicsCardList() -> List<Text> = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual auto GetPrimaryGraphicsCardIndex() -> i64 = 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
auto CreateRHIComponent(
	window_manager::WindowManagerComponent&		window_manager,
	const RHIComponentCreateInfo&				create_info
) -> UniquePtr<RHIComponent>;



} // rhi
} // bc
