#pragma once

#include <sstream>

#include "Event.hpp"

namespace Atlas {

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_MouseX << ',' << m_MouseY;
        return ss.str();
    }

    inline float GetX() const {
        return m_MouseX;
    }
    inline float GetY() const {
        return m_MouseY;
    }

private:
    float m_MouseX, m_MouseY;
};


class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
        return ss.str();
    }

    inline float GetXOffset() const {
        return m_XOffset;
    }
    inline float GetYOffset() const {
        return m_YOffset;
    }

private:
    float m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event {
public:
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    inline int GetMouseButton() const {
        return m_Button;
    }

protected:
    int m_Button;

    MouseButtonEvent(int button) : m_Button(button) {}
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    EVENT_CLASS_TYPE(MouseButtonPressed)

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_Button;
        return ss.str();
    }
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    EVENT_CLASS_TYPE(MouseButtonReleased)

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_Button;
        return ss.str();
    }
};

} // namespace Atlas
