
#include <editor/PreCompiledHeader.hpp>

#include <editor/entrypoint/Main.hpp>



int main(
	int				argument_count,
	char		**	argument_strings
)
{
	auto engine_editor = bc::editor::EditorComponent();
	engine_editor.Run();

    return 0;
}
