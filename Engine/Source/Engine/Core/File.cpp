// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/File.hpp"

#include <fstream>

namespace Cobalt
{
    auto File::Read(const Filepath& path) -> String {
        if (std::ifstream stream(path, std::ios::in | std::ios::binary); stream) {
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

    auto File::Exists(const Filepath& path) -> bool {
        return std::filesystem::exists(path);
    }
} // namespace Cobalt
