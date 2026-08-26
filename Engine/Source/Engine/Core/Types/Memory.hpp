// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <EASTL/unique_ptr.h>
#include <EASTL/shared_ptr.h>
#include <rpmalloc.h>

namespace Cobalt
{
    template <typename T>
    using Box = eastl::unique_ptr<T>;

    template <typename T>
    using Rc = eastl::shared_ptr<T>;

    namespace Memory
    {
        inline auto Init() -> void {
            rpmalloc_linker_reference();
        }

        template <typename T, typename... Args>
        constexpr auto MakeBox(Args&&... args) -> Box<T> {
            return eastl::make_unique<T>(std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        constexpr auto MakeRc(Args&&... args) -> Rc<T> {
            return eastl::make_shared<T>(std::forward<Args>(args)...);
        }
    } // namespace Memory
} // namespace Cobalt

