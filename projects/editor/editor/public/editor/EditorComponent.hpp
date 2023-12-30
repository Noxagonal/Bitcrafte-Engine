#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



#include <memory>



namespace bc {
namespace core { class CoreComponent; }
namespace engine { class EngineComponent; }
namespace window_manager { class Window; }

namespace editor {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EditorComponent
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	EditorComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~EditorComponent();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void														Run();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<CoreComponent>								core;

	UniquePtr<bc::engine::EngineComponent>						engine;

	UniquePtr<window_manager::Window>							main_window;

	bool														should_close				= false;
};



} // editor
} // bc
