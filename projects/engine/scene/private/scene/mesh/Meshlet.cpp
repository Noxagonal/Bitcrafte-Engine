
#include <scene/PreCompiledHeader.hpp>
#include <scene/mesh/Meshlet.hpp>
#include <core/math/VectorMath.hpp>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::scene::Meshlet bc::scene::MakeMeshlet(
	const bc::List<Vertex>		&	vertex_list,
	bc::ListView<u32>				vertex_index_list
)
{
	BAssert( !vertex_index_list.IsEmpty(), U"Cannot create meshlet, vertex index list was empty" );
	BAssert( vertex_index_list.Size() <= internal_::MESHLET_MAX_INDICES, U"Cannot create meshlet, vertex index list was too large" );

	auto mean_normal	= math::Vec4f32();
	auto center_point	= math::Vec4f32();

	for( auto & i : vertex_index_list )
	{
		mean_normal		+= vertex_list[ i ].normal;
		center_point	+= vertex_list[ i ].position;
	}

	mean_normal		= mean_normal.Normalized();
	center_point	= center_point / vertex_index_list.Size();

	auto mean_normal_cone_dot	= 1.0f;
	auto radius					= 0.0f;

	for( auto & i : vertex_index_list )
	{
		mean_normal_cone_dot	= std::min( mean_normal_cone_dot, math::Dot( mean_normal, vertex_list[ i ].normal ) );
		radius					= std::max( radius, math::Distance( center_point, vertex_list[ i ].position ) );
	}

	auto result = Meshlet();
	result.mean_normal				= mean_normal;
	result.center_point				= center_point;
	result.mean_normal_cone_dot		= mean_normal_cone_dot;
	result.radius					= radius;
	result.vertex_count				= u32( vertex_index_list.Size() );
	for( u64 i = 0; i < vertex_index_list.Size(); i++ )
	{
		result.vertex_index_list[ i ] = vertex_index_list[ i ];
	}
	return result;
}
