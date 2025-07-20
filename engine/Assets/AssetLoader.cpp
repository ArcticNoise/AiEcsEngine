#include "AssetLoader.hpp"

namespace x2d {

AssetLoader::AssetLoader(std::size_t threadCount) {
    threadCount = threadCount == 0 ? 1 : threadCount;
    for (std::size_t i = 0; i < threadCount; ++i) {
        m_Threads.emplace_back(&AssetLoader::Worker, this);
    }
}

AssetLoader::~AssetLoader() {
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Stop = true;
    }
    m_Condition.notify_all();
    for (auto &t : m_Threads) {
        t.join();
    }
}

void AssetLoader::Worker() {
    for (;;) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_Condition.wait(lock, [this] { return m_Stop || !m_Jobs.empty(); });
            if (m_Stop && m_Jobs.empty()) {
                return;
            }
            job = std::move(m_Jobs.front());
            m_Jobs.pop();
        }
        job();
    }
}

} // namespace x2d
