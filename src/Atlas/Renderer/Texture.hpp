#pragma once

#include "atlaspch.hpp"

namespace Atlas {

    class Texture {
    public:
        virtual void Bind(unsigned int slot = 0) const = 0;
        virtual unsigned int GetRendererID() const     = 0;
        virtual unsigned int GetWidth() const          = 0;
        virtual unsigned int GetHeight() const         = 0;
    };

    class Texture2D : public Texture {
    public:
        virtual const bool Loaded() noexcept = 0;
        static Ref<Texture2D> Create(const std::string& path);
    };

    class TextureCube : public Texture {
    public:
        virtual const bool Loaded() noexcept = 0;
        static Ref<TextureCube> Create(const std::string& path);
    };

    class TextureHDR : public Texture {
    public:
        virtual const bool Loaded() noexcept = 0;
        static Ref<TextureHDR> Create(const std::string& path);
    };

    class TextureSlotAlocator {
    public:
        uint32_t GetSlot() {
            for (int i = 0; i < 32; ++i) {
                if ((m_Slot & Bit(i)) == 0) {
                    m_Slot |= Bit(i);
                    return i;
                }
            }
            AT_CORE_ASSERT(false, "No empty slot to allocate!");
        }
        uint32_t ClearSlot(uint32_t index) {
            if ((m_Slot & Bit(index)) != 0) {
                m_Slot ^= Bit(index);
            }
            AT_CORE_ASSERT(false, "This is a empty")
        }

    private:
        uint32_t m_Slot = 0;
    };


} // namespace Atlas
