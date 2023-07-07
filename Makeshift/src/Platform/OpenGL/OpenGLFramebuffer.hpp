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

		virtual void resize(uint32_t width, uint32_t height) override;
		virtual int readPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual uint32_t getColorAttachmentRendererId(uint32_t index) const override {
			MK_CORE_ASSERT(index < m_ColorAttachments.size(), "Attachment out of bounds");
			return m_ColorAttachments[index];
		};

		virtual const FramebufferSpecification& getSpecification() const override { return m_Specification; };
	private:
		uint32_t m_RendererId = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};

}