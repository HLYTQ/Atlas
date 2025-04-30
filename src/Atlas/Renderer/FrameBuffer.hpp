#pragma once

#include "atlaspch.hpp"

namespace Atlas {

    class FrameBuffer {
    public:
        virtual void Bind() const {};
        virtual void Unbind() const {};
        virtual void BindIntermediate() const {};

        virtual const uint32_t GetWidth() const = 0;
        virtual const uint32_t GetHeight() const = 0;
        virtual void Resize(uint32_t width, uint32_t height) {};
        virtual unsigned int GetColorAttachment() = 0;

        static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);
 
    };

    class FrameBufferHDR {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        static Ref<FrameBufferHDR> Create();
    };

}