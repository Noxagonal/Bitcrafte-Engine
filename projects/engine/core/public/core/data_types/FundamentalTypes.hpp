#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <cstdint>

// TODO: Add float16_t, float32_t and float64_t as f16, f32 and f64 when all major compilers support it.
//#include <stdfloat>



namespace bc {

using i8	= int8_t;
using i16	= int16_t;
using i32	= int32_t;
using i64	= int64_t;
using u8	= uint8_t;
using u16	= uint16_t;
using u32	= uint32_t;
using u64	= uint64_t;

using c8	= char8_t;
using c16	= char16_t;
using c32	= char32_t;

using f32	= float;
using f64	= double;

static_assert( sizeof( f32 ) == 4 );
static_assert( sizeof( f64 ) == 8 );

//using size	= size_t;

} // bc
