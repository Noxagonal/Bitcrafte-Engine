#pragma once

#include <core/diagnostic/assertion/Assert.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>

#define BC_CONTAINER_IMPLEMENTATION_NORMAL 1
#include <core/containers/backend/ArrayImpl.hpp>
#undef BC_CONTAINER_IMPLEMENTATION_NORMAL
