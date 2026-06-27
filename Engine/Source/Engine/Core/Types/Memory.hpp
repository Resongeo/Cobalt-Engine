// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <memory>

namespace Cobalt
{
    template <typename T>
    using Box = std::unique_ptr<T>;

    template <typename T>
    using Rc = std::shared_ptr<T>;

    namespace Memory
    {
        template <typename T, typename... Args>
        constexpr auto MakeBox(Args&&... args) -> Box<T> {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        constexpr auto MakeRc(Args&&... args) -> Rc<T> {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    } // namespace Memory
} // namespace Cobalt

