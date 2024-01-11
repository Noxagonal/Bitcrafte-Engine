
#if BC_CONTAINER_IMPLEMENTATION_NORMAL

#define BC_CONTAINER_NAMESPACE_START
#define BC_CONTAINER_NAMESPACE_END
#define BC_CONTAINER_NOEXCEPT
#define BC_CONTAINER_NAME( container_name ) container_name
#define BC_CONTAINER_VALUE_TYPENAME typename
#define BC_CONTAINER_IS_DEFAULT_CONSTRUCTIBLE ::std::is_default_constructible_v
#define BC_CONTAINER_IS_COPY_CONSTRUCTIBLE ::std::is_copy_constructible_v
#define BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE ::std::is_move_constructible_v
#define BC_CONTAINER_IS_COPY_ASSIGNABLE ::std::is_copy_assignable_v
#define BC_CONTAINER_IS_MOVE_ASSIGNABLE ::std::is_move_assignable_v

#define BC_ContainerAssert( condition, fail_message, ... ) BAssert( ( condition ), bc::diagnostic::MakePrintRecord_AssertText( fail_message, __VA_ARGS__ ) )

#elif BC_CONTAINER_IMPLEMENTATION_SIMPLE

#define BC_CONTAINER_NAMESPACE_START namespace internal {
#define BC_CONTAINER_NAMESPACE_END } // internal
#define BC_CONTAINER_NOEXCEPT noexcept
#define BC_CONTAINER_NAME( container_name ) Simple##container_name
#define BC_CONTAINER_VALUE_TYPENAME ::bc::utility::SimpleContainerAllowedValueType
#define BC_CONTAINER_IS_DEFAULT_CONSTRUCTIBLE ::std::is_nothrow_default_constructible_v
#define BC_CONTAINER_IS_COPY_CONSTRUCTIBLE ::std::is_nothrow_copy_constructible_v
#define BC_CONTAINER_IS_MOVE_CONSTRUCTIBLE ::std::is_nothrow_move_constructible_v
#define BC_CONTAINER_IS_COPY_ASSIGNABLE ::std::is_nothrow_copy_assignable_v
#define BC_CONTAINER_IS_MOVE_ASSIGNABLE ::std::is_nothrow_move_assignable_v

#define BC_ContainerAssert( condition, fail_message, ... ) assert( ( condition ) && fail_message )

#else
#error "container implementation not defined"
#endif
