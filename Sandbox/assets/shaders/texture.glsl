// Simple texture shader

#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 fragTexCoord;

void main(void) {
	fragTexCoord = texCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);

}

#type fragment
#version 330 core

layout(location = 0) out vec4 outColor;

in vec2 fragTexCoord;

uniform sampler2D u_Texture;

void main(void) {
	outColor = texture(u_Texture, fragTexCoord);
}