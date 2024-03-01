
#include <scene/PreCompiledHeader.hpp>
#include <scene/mesh/Mesh.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::scene::Mesh::Mesh()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::scene::Mesh::Mesh(
	const List<Vertex>		&	vertex_list,
	const List<Meshlet>		&	meshlet_list
) :
	vertex_list( vertex_list ),
	meshlet_list( meshlet_list )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::scene::Mesh::Mesh(
	const List<Vertex>		&	vertex_list,
	const List<u32>			&	index_list
) :
	vertex_list( vertex_list ),
	index_list( index_list )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::scene::Mesh::~Mesh()
{}