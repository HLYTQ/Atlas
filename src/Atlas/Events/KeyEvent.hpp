#pragma once

#include <sstream>

#include "Event.hpp"

namespace Atlas {

class KeyEvent : public Event {
public:
    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    inline int GetKeyCode() const {
        return m_KeyCode;
    }

protected:
    int m_KeyCode;

    KeyEvent(int keycode) : m_KeyCode(keycode) {}
};


class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

    EVENT_CLASS_TYPE(KeyPressed)

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << ")";
        return ss.str();
    }
    inline int GetRepeatCount() const {
        return m_RepeatCount;
    }

private:
    int m_RepeatCount;
};


class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyReleased)

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_KeyCode;
        return ss.str();
    }
};
} // namespace Atlas
