// Simple texture shader

#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;

void main(void) {
	v_Color = color;
	v_TexCoord = texCoord;
	gl_Position = u_ViewProjection * vec4(position, 1.0);

}

#type fragment
#version 330 core

layout(location = 0) out vec4 outColor;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color = vec4(1.0);
uniform float u_TilingFactor = 1.0;

void main(void) {
	//outColor = texture(u_Texture, v_TexCoord * u_TilingFactor) * v_Color;
	outColor = v_Color;
}