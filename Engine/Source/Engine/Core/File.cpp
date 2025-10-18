// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/File.hpp"

#include <fstream>

namespace Cobalt::Engine
{
    auto File::read(const Filepath& path) -> String {
        if (std::ifstream stream(path, std::ios::in, std::ios::binary); stream)
        {
            String result = {};

            stream.seekg(0, std::ios::end);
            result.resize(stream.tellg());

            stream.seekg(0, std::ios::beg);
            stream.read(result.data(), result.size());
            stream.close();

            return result;
        }

        return {};
    }
}
