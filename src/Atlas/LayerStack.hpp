#pragma once

#include <vector>

#include "Layer.hpp"

namespace Atlas {

class LayerStack {
public:
    LayerStack() = default;
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PopLayer(Layer* layer);
    void PushOverLay(Layer* overlay);
    void PopOverLay(Layer* overlay);

    std::vector<Layer*>::iterator begin() {
        return m_Layers.begin();
    }
    std::vector<Layer*>::iterator end() {
        return m_Layers.end();
    }
    std::vector<Layer*>::const_iterator begin() const {
        return m_Layers.begin();
    }
    std::vector<Layer*>::const_iterator end() const {
        return m_Layers.end();
    }
    std::vector<Layer*>::reverse_iterator rbegin() {
        return m_Layers.rbegin();
    }
    std::vector<Layer*>::reverse_iterator rend() {
        return m_Layers.rend();
    }
    std::vector<Layer*>::const_reverse_iterator rbegin() const {
        return m_Layers.rbegin();
    }
    std::vector<Layer*>::const_reverse_iterator rend() const {
        return m_Layers.rbegin();
    }

private:
    std::vector<Layer*> m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};

} // namespace Atlas
