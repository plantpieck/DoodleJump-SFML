#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>
#include <stdexcept>

template <typename Resource, typename Identifier>
class ResourceManager {
private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;

public:
    void load(Identifier id, const std::string& filename) {
        auto resource = std::make_unique<Resource>();
        if (!resource->loadFromFile(filename)) {
            throw std::runtime_error("ResourceManager::load - Failed to load " + filename);
        }
        mResourceMap.insert(std::make_pair(id, std::move(resource)));
    }

    Resource& get(Identifier id) {
        auto found = mResourceMap.find(id);
        return *found->second;
    }
};