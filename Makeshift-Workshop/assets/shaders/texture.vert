#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;
layout(location = 4) in float tilingFactor;

uniform mat4 u_ViewProjection;

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out float v_TexIndex;
layout(location = 3) out float v_TilingFactor;

void main(void) {
	v_Color = color;
	v_TexCoord = texCoord;
	v_TexIndex = texIndex;
	v_TilingFactor = tilingFactor;
	gl_Position = u_ViewProjection * vec4(position, 1.0);

}