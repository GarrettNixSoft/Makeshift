#pragma once

namespace Makeshift {

	enum class ShaderDataType {
		None = 0,
		Float, Int, Bool,
		Vec2, Vec3, Vec4,
		Mat3, Mat4,
		Vec2i, Vec3i, Vec4i
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Bool:		return 1;
			case ShaderDataType::Vec2:		return 4 * 2;
			case ShaderDataType::Vec3:		return 4 * 3;
			case ShaderDataType::Vec4:		return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Vec2i:		return 4 * 2;
			case ShaderDataType::Vec3i:		return 4 * 3;
			case ShaderDataType::Vec4i:		return 4 * 4;
		}

		MK_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct BufferElement {
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : type(type), name(name), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {

		}

		uint32_t getComponentCount() const {
			switch (type) {
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Bool:		return 1;
				case ShaderDataType::Vec2:		return 2;
				case ShaderDataType::Vec3:		return 3;
				case ShaderDataType::Vec4:		return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Vec2i:		return 2;
				case ShaderDataType::Vec3i:		return 3;
				case ShaderDataType::Vec4i:		return 4;
			}

			MK_CORE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements) : elements{ elements } {
			calculateOffsetsAndStride();
		}

		inline uint32_t getStride() const { return stride; }
		inline const std::vector<BufferElement>& bufferElements() const { return elements; };

		std::vector<BufferElement>::iterator begin() { return elements.begin(); }
		std::vector<BufferElement>::iterator end() { return elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements.end(); }
	private:
		void calculateOffsetsAndStride() {
			uint32_t offset = 0;
			stride = 0; // in case of recalc
			for (auto& element : elements) {
				element.offset = offset;
				offset += element.size;
				stride += element.size;
			}
		}
	private:
		std::vector<BufferElement> elements;
		uint32_t stride = 0;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const BufferLayout& getLayout() const = 0;
		virtual void setLayout(const BufferLayout& newLayout) = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};

}