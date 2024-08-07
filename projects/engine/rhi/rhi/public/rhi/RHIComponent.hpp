#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <rhi/RHIComponentCreateInfo.hpp>
#include <rhi/RHIComponentStartInfo.hpp>

#include <core/containers/List.hpp>
#include <core/containers/Text.hpp>



namespace bc {
namespace rhi {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RHIComponent
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RHIComponent(
		const RHIComponentCreateInfo								&	create_info
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~RHIComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void														Start(
		const RHIComponentStartInfo									&	rhi_start_info
	) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual List<Text>													GetGraphicsCardList() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual i64															GetPrimaryGraphicsCardIndex() = 0;
};



} // rhi
} // bc
