#include<glad/glad.h>
#include<iostream>
#include"FrameBuffer.hpp"
 
namespace Atlas {
FrameBuffer::FrameBuffer(uint32_t width, uint32_t height):m_Width(width),m_Height(height){
	Invaidate();
}
 
FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &m_FrameBufferID);
}
 
void FrameBuffer::Invaidate(){
    //创建缓冲区
	glCreateFramebuffers(1, &m_FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
    //生成三颜色通道纹理
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //生成对应的颜色缓冲区
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
    //生成深度纹理
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	//生成深度缓冲区
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
	if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
		std::cout << "Frame Buffer Compile ERROR!" << std::endl;
 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
 
void FrameBuffer::Resize(uint32_t width, uint32_t height){
    //窗口大小改变，重新设置缓冲区
	m_Width = width;
	m_Height = height;
 
	Invaidate();
}
 
void FrameBuffer::Bind(){
    //绑定帧缓冲区
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
}
 
void FrameBuffer::Unbind() {
    //解绑帧缓冲区
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Atlas
