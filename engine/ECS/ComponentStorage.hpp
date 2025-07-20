#pragma once

#include <vector>
#include <algorithm>
#include <type_traits>
#include <cstdint>

#include "Entity.hpp"
#include "TypeTraits.hpp"

namespace x2d
{

namespace detail
{
template<class T>
struct StorageType
{
    using Type = T;
};

template<class T>
struct StorageType<OneFrame<T>>
{
    using Type = T;
};
} // namespace detail

class IComponentStorage
{
public:
    virtual ~IComponentStorage() = default;
    virtual void RemoveEntity(Entity entity) = 0;
    virtual void ClearIfOneFrame() = 0;
};

template<class TComponent>
class ComponentStorage final : public IComponentStorage
{
public:
    using Stored = typename detail::StorageType<TComponent>::Type;
    static_assert(std::is_trivial_v<Stored> && std::is_standard_layout_v<Stored>, "Component must be POD");

    Stored& Add(Entity entity, const Stored& component)
    {
        const std::uint32_t index = GetIndex(entity);
        if (m_Sparse.size() <= index)
        {
            m_Sparse.resize(index + 1, InvalidIndex);
        }
        const std::uint32_t denseIndex = static_cast<std::uint32_t>(m_Dense.size());
        m_Dense.push_back(component);
        m_Entities.push_back(entity);
        m_Sparse[index] = denseIndex;
        return m_Dense.back();
    }

    bool Has(Entity entity) const
    {
        const std::uint32_t index = GetIndex(entity);
        if (index >= m_Sparse.size())
        {
            return false;
        }
        const std::uint32_t denseIndex = m_Sparse[index];
        return denseIndex != InvalidIndex && denseIndex < m_Dense.size() && m_Entities[denseIndex] == entity;
    }

    Stored& Get(Entity entity)
    {
        const std::uint32_t denseIndex = m_Sparse[GetIndex(entity)];
        return m_Dense[denseIndex];
    }

    void Remove(Entity entity)
    {
        const std::uint32_t index = GetIndex(entity);
        if (index >= m_Sparse.size())
        {
            return;
        }
        const std::uint32_t denseIndex = m_Sparse[index];
        if (denseIndex == InvalidIndex || m_Entities[denseIndex] != entity)
        {
            return;
        }
        const std::uint32_t last = static_cast<std::uint32_t>(m_Dense.size() - 1);
        if (denseIndex != last)
        {
            m_Dense[denseIndex] = m_Dense[last];
            m_Entities[denseIndex] = m_Entities[last];
            m_Sparse[GetIndex(m_Entities[denseIndex])] = denseIndex;
        }
        m_Dense.pop_back();
        m_Entities.pop_back();
        m_Sparse[index] = InvalidIndex;
    }

    void RemoveEntity(Entity entity)
    {
        Remove(entity);
    }

    void ClearIfOneFrame() override
    {
        if constexpr (IsOneFrame<TComponent>::value)
        {
            m_Dense.clear();
            m_Entities.clear();
            std::fill(m_Sparse.begin(), m_Sparse.end(), InvalidIndex);
        }
    }

private:
    static constexpr std::uint32_t InvalidIndex = 0xFFFFFFFFU;
    std::vector<Stored> m_Dense{};
    std::vector<Entity> m_Entities{};
    std::vector<std::uint32_t> m_Sparse{};
};

} // namespace x2d

