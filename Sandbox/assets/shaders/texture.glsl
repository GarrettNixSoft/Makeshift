// Simple texture shader

#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;
layout(location = 4) in float tilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

void main(void) {
	v_Color = color;
	v_TexCoord = texCoord;
	v_TexIndex = texIndex;
	v_TilingFactor = tilingFactor;
	gl_Position = u_ViewProjection * vec4(position, 1.0);

}

#type fragment
#version 330 core

layout(location = 0) out vec4 outColor;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main(void) {
	// TODO: tiling factor in vertex buffer
	outColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;

}