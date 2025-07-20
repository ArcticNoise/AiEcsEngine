#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ComponentStorage.hpp"
#include "EntityManager.hpp"
#include "Signature.hpp"
#include "SystemManager.hpp"

namespace x2d
{

class World final
{
public:
    World();

    [[nodiscard]] Entity CreateEntity();
    void DestroyEntity(Entity entity);

    template<class TComponent>
    typename ComponentStorage<TComponent>::Stored& AddComponent(Entity entity, const typename ComponentStorage<TComponent>::Stored& component)
    {
        auto& storage = GetOrCreateStorage<TComponent>();
        m_Signatures[GetIndex(entity)].set(ComponentTypeId<TComponent>());
        return storage.Add(entity, component);
    }

    template<class TComponent>
    void RemoveComponent(Entity entity)
    {
        auto* storage = GetStorage<TComponent>();
        if (storage == nullptr)
        {
            return;
        }
        storage->RemoveEntity(entity);
        m_Signatures[GetIndex(entity)].reset(ComponentTypeId<TComponent>());
    }

    template<class TComponent>
    [[nodiscard]] bool HasComponent(Entity entity) const
    {
        const auto* storage = GetStorage<TComponent>();
        return storage != nullptr && storage->Has(entity);
    }

    [[nodiscard]] const std::vector<Signature>& GetSignatures() const;
    [[nodiscard]] std::uint16_t GetGenerationForIndex(std::uint32_t index) const;

    [[nodiscard]] SystemManager& GetSystemManager();
    [[nodiscard]] const SystemManager& GetSystemManager() const;

    void EndFrame();

private:
    template<class TComponent>
    ComponentStorage<TComponent>& GetOrCreateStorage()
    {
        const std::size_t id = ComponentTypeId<TComponent>();
        auto it = m_Storages.find(id);
        if (it == m_Storages.end())
        {
            auto storage = std::make_unique<ComponentStorage<TComponent>>();
            auto* ptr = storage.get();
            m_Storages.emplace(id, std::move(storage));
            return *ptr;
        }
        return *static_cast<ComponentStorage<TComponent>*>(it->second.get());
    }

    template<class TComponent>
    ComponentStorage<TComponent>* GetStorage() const
    {
        const std::size_t id = ComponentTypeId<TComponent>();
        auto it = m_Storages.find(id);
        if (it == m_Storages.end())
        {
            return nullptr;
        }
        return static_cast<ComponentStorage<TComponent>*>(it->second.get());
    }

    EntityManager m_EntityManager;
    std::unordered_map<std::size_t, std::unique_ptr<IComponentStorage>> m_Storages;
    std::vector<Signature> m_Signatures;
    SystemManager m_SystemManager;
};

} // namespace x2d

