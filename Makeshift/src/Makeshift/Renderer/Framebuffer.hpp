#pragma once

#include "Makeshift/Core/Core.hpp"

namespace Makeshift {

	struct FramebufferSpecification {
		uint32_t width, height;
		//FramebufferFormat format = ;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer {
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual uint32_t getColorAttachmentRendererId() const = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}