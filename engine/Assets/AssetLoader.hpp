#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace x2d {

class AssetLoader final {
  public:
    explicit AssetLoader(std::size_t threadCount = std::thread::hardware_concurrency());
    ~AssetLoader();

    template <class T, class LoaderFunc>
    std::shared_future<std::shared_ptr<T>> Load(const std::string &path, LoaderFunc loader) {
        const std::size_t hash = std::hash<std::string>{}(path);
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            auto it = m_JobsMap.find(hash);
            if (it != m_JobsMap.end()) {
                auto job = std::static_pointer_cast<Job<T>>(it->second);
                return job->future;
            }
        }

        auto task = std::make_shared<std::packaged_task<std::shared_ptr<T>()>>(
            [loader, path]() { return std::make_shared<T>(loader(path)); });
        auto future = task->get_future().share();
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            auto job = std::make_shared<Job<T>>();
            job->future = future;
            m_JobsMap[hash] = job;
            m_Jobs.emplace([task]() { (*task)(); });
        }
        m_Condition.notify_one();
        return future;
    }

  private:
    struct IJob {
        virtual ~IJob() = default;
    };

    template <class T> struct Job : IJob {
        std::shared_future<std::shared_ptr<T>> future;
    };

    void Worker();

    std::vector<std::thread> m_Threads;
    std::queue<std::function<void()>> m_Jobs;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    bool m_Stop{false};

    std::unordered_map<std::size_t, std::shared_ptr<IJob>> m_JobsMap;
};

} // namespace x2d
