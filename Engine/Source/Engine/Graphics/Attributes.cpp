//
// Created by sbene on 10/18/2025.
//

#include "Engine/Graphics/Attributes.hpp"

namespace Cobalt::Engine
{
    Attribute::Attribute(const AttributeDataType data_type) : type(data_type) {
        switch (type) {
            case AttributeDataType::Bool: size = 1; break;
            case AttributeDataType::Int: size = 4; break;
            case AttributeDataType::Int2: size = 4 * 2; break;
            case AttributeDataType::Int3: size = 4 * 3; break;
            case AttributeDataType::Int4: size = 4 * 4; break;
            case AttributeDataType::Float: size = 4; break;
            case AttributeDataType::Float2: size = 4 * 2; break;
            case AttributeDataType::Float3: size = 4 * 3; break;
            case AttributeDataType::Float4: size = 4 * 4; break;
            case AttributeDataType::Mat4: size = 4 * 4 * 4; break;
            default: break;
        }
    }

    auto Attribute::element_count() const -> u32 {
        switch (type) {
            case AttributeDataType::Bool: return 1;
            case AttributeDataType::Int: return 1;
            case AttributeDataType::Int2: return 2;
            case AttributeDataType::Int3: return 3;
            case AttributeDataType::Int4: return 4;
            case AttributeDataType::Float: return 1;
            case AttributeDataType::Float2: return 2;
            case AttributeDataType::Float3: return 3;
            case AttributeDataType::Float4: return 4;
            case AttributeDataType::Mat4: return 4 * 4;
            default: return 0;
        }
    }

    auto AttributeLayout::create(const InitializerList<Attribute>& attributes) -> void {
        m_attributes = attributes;
        m_stride = 0;
        u32 offset = 0;

        for (auto& attribute : m_attributes) {
            attribute.offset = offset;
            offset += attribute.size;
            m_stride += attribute.size;
        }
    }

    auto AttributeLayout::attributes() -> Vector<Attribute>& {
        return m_attributes;
    }

    auto AttributeLayout::stride() -> u32 {
        return m_stride;
    }

    auto AttributeLayout::begin() -> Vector<Attribute>::iterator {
        return m_attributes.begin();
    }
    auto AttributeLayout::end() -> Vector<Attribute>::iterator {
        return m_attributes.end();
    }
    auto AttributeLayout::begin() const -> Vector<Attribute>::const_iterator {
        return m_attributes.begin();
    }
    auto AttributeLayout::end() const -> Vector<Attribute>::const_iterator {
        return m_attributes.end();
    }
} // namespace Cobalt::Engine
