#pragma once

#include "Atlas/KeyCodes.hpp"
#include <glm/glm.hpp>

namespace Atlas {

class Input {
public:
    virtual ~Input() = default;
    static bool IsKeyPressed(const KeyCode key);
    static bool IsMouseButtonPressed(const MouseCode button);
    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};

} // namespace Atlas
