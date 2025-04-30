#pragma once

#include "Event.hpp"
#include <string>
#include <vector>

namespace Atlas {

class FileDropEvent : public Event {
public:
    FileDropEvent(std::vector<std::string>&& filepaths) {
        m_Filepaths = std::move(filepaths);
    }

    EVENT_CLASS_TYPE(FileDrop)
    EVENT_CLASS_CATEGORY(EventCategoryFileDrop)

    std::vector<std::string> GetFilepaths() {
        return m_Filepaths;
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "FileDrop: " << m_Filepaths[0] << "\n";
        return ss.str();
    }

private:
    std::vector<std::string> m_Filepaths;
};

} // namespace Atlas
