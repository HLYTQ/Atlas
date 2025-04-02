#pragma once

#include "spdlog/fmt/bundled/base.h"
#include <functional>
#include <string>

#include "Core.hpp"

namespace Atlas {

// ENUM----------------------------------------------------------------------------
enum class EventType {
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseScrolled,
    MouseMoved
};

enum EventCategory {
    None                     = 0,
    EventCategoryApplication = Bit(0),
    EventCategoryInput       = Bit(1),
    EventCategoryKeyboard    = Bit(2),
    EventCategoryMouse       = Bit(3),
    EventCategoryMouseButton = Bit(4)
};

#define EVENT_CLASS_TYPE(type)                        \
    static EventType GetStaticType() {                \
        return EventType::type;                       \
    }                                                 \
    virtual EventType GetEventType() const override { \
        return GetStaticType();                       \
    }                                                 \
    virtual const char* GetName() const override {    \
        return #type;                                 \
    }

#define EVENT_CLASS_CATEGORY(category)              \
    virtual int GetCategoryFlags() const override { \
        return category;                            \
    }

class Event {
    friend class EventDispatcher;

public:
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const    = 0;
    virtual int GetCategoryFlags() const   = 0;
    virtual std::string ToString() const {
        return GetName();
    }

    inline bool IsInCategory(EventCategory category) {
        return GetCategoryFlags() & category;
    }

    bool Handled = false;
};


class EventDispatcher {
private:
    Event& m_Event;

    template <typename T>
    using EventFn = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event) : m_Event(event) {}
    template <typename T>
    bool Dispatch(EventFn<T> func) {
        if (m_Event.GetEventType() == T::GetStaticType()) {
            m_Event.Handled = func(*(T*) &m_Event);
            return true;
        }
        return false;
    }
};

} // namespace Atlas
