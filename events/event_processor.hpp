#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

struct EventProcessor
{
private:
    std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> subscriptions_;

public:
    template<typename T>
    void subscribe(std::function<void(const T&)>);

    template<typename T>
    void raise(const T&);
};

template<typename T>
inline void EventProcessor::subscribe(std::function<void(const T&)> callback)
{
    auto wrapper = [callback](const void* event)
    {
        callback(*static_cast<const T*>(event));
    };
    subscriptions_[typeid(T)].emplace_back(std::move(wrapper));
}

template <typename T>
inline void EventProcessor::raise(const T& event)
{
    auto it = subscriptions_.find(typeid(T));
    if (it != subscriptions_.end())
    {
        for (auto& callback : it->second)
        {
            callback(&event);
        }
    }
}
