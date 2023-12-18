
#if BI_CONTAINER_IMPLEMENTATION_NORMAL

#define BC_CONTAINER_NOEXCEPT
#define BC_CONTAINER_NAME( container_name ) container_name
#define BC_CONTAINER_VALUE_TYPENAME typename

#define BC_ContainerAssert( condition, fail_message, ... ) BAssert( ( condition ), bc::diagnostic::MakePrintRecord_AssertText( fail_message, __VA_ARGS__ ) )

#elif BI_CONTAINER_IMPLEMENTATION_SIMPLE

#define BC_CONTAINER_NOEXCEPT noexcept
#define BC_CONTAINER_NAME( container_name ) Simple##container_name
#define BC_CONTAINER_VALUE_TYPENAME bc::container_bases::SimpleListAllowedValueType

#define BC_ContainerAssert( condition, fail_message, ... ) assert( ( condition ) && fail_message )

#else
#error "container implementation not defined"
#endif
