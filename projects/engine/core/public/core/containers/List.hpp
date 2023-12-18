#pragma once

#include <core/diagnostic/assertion/Assert.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>

#define BI_CONTAINER_IMPLEMENTATION_NORMAL 1
#include <core/containers/backend/ListImpl.hpp>
#undef BI_CONTAINER_IMPLEMENTATION_NORMAL
