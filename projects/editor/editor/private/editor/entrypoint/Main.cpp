
#include <editor/PreCompiledHeader.hpp>

#include <editor/entrypoint/Main.hpp>



#if !BITCRAFTE_GAME_DEVELOPMENT_BUILD
static_assert( 0, "Editor cannot be compiled in non-development builds, Eg. shipping builds" );
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(
	int				argument_count,
	char		**	argument_strings
)
{
	auto engine_editor = bc::editor::EditorComponent();
	engine_editor.Run();

	return 0;
}
