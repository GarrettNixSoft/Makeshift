#include "mkpch.hpp"
#include "OpenGLFramebuffer.hpp"

#include <glad/glad.h>

namespace Makeshift {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec) {
		invalidate();
	}
	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &m_RendererId);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::invalidate() {
		if (m_RendererId) {
			glDeleteFramebuffers(1, &m_RendererId);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		MK_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		glViewport(0, 0, m_Specification.width, m_Specification.height);
	}

	void OpenGLFramebuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height) {
		m_Specification.width = width;
		m_Specification.height = height;
		invalidate();
	}

}