// Simple flat color shader

#type vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main(void) {
	gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);

}

#type fragment
#version 330 core

layout(location = 0) out vec4 outColor;

uniform vec4 u_Color;

void main(void) {
	outColor = u_Color;
}