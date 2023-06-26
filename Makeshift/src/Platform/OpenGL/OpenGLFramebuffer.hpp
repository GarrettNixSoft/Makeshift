#pragma once

#include "Makeshift/Renderer/Framebuffer.hpp"

namespace Makeshift {

	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void invalidate();

		virtual void bind() override;
		virtual void unbind() override;

		virtual uint32_t getColorAttachmentRendererId() const override { return m_ColorAttachment; };

		virtual const FramebufferSpecification& getSpecification() const override { return m_Specification; };
	private:
		uint32_t m_RendererId;
		uint32_t m_ColorAttachment, m_DepthAttachment;

		FramebufferSpecification m_Specification;
	};

}