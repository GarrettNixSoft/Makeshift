#pragma once

#include "Makeshift/Core/Core.hpp"

namespace Makeshift {

	enum class FramebufferTextureFormat {

		None = 0,

		// Color formats
		RGBA8,
		RGBA16F,

		// Depth/stencil formats
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8

	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: textureFormat(format) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrapping modes
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attach)
			: attachments(attach) {}

		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification {
		uint32_t width, height;
		FramebufferAttachmentSpecification attachments;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t getColorAttachmentRendererId(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}