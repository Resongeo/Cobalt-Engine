// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include <cstdlib>

// Debug operator new[] for EASTL
void* operator new[](const size_t size, const char*, int, unsigned int, const char*, int) {
    return malloc(size);
}

// Regular operator new[] for EASTL
// ReSharper disable once CppParameterMayBeConst
void* operator new[](size_t size, size_t alignment, size_t, const char*, int, unsigned, const char*, int) {
    return malloc(size);
}
