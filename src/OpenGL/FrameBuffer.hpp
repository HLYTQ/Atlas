#pragma once

namespace Atlas { 

class FrameBuffer {
public:
	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();
	void Invaidate();
	void Resize(unsigned int width, unsigned int height);
	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }
	inline unsigned int GetColorAttachment() const { return m_ColorAttachment; }
 
	void Bind();
	void Unbind();
private:
	unsigned int m_FrameBufferID, m_ColorAttachment, m_DepthAttachment;
	unsigned int m_Width, m_Height;
};

} // namespace Atlas
