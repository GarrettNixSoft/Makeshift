// Simple texture shader

#type vertex
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;
layout(location = 4) in float tilingFactor;
layout(location = 5) in int entityId;

layout(std140, binding = 0) uniform Camera {
	mat4 u_ViewProjection;
};

struct VertexOutput {
	vec4 color;
	vec2 texCoord;
	float tilingFactor;
};

layout(location = 0) out VertexOutput v_Output;
layout(location = 3) out flat float v_TexIndex;
layout(location = 4) out flat int v_EntityId;

void main(void) {
	v_Output.color = color;
	v_Output.texCoord = texCoord;
	v_Output.tilingFactor = tilingFactor;
	v_TexIndex = texIndex;
	v_EntityId = entityId;

	gl_Position = u_ViewProjection * vec4(position, 1.0);

}

#type fragment
#version 450 core

layout(location = 0) out vec4 outColor;
layout(location = 1) out int color2;

struct VertexOutput {
	vec4 color;
	vec2 texCoord;
	float tilingFactor;
};

layout(location = 0) in VertexOutput v_Input;
layout(location = 3) in flat float v_TexIndex;
layout(location = 4) in flat int v_EntityId;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main(void) {
	outColor = v_Input.color * texture(u_Textures[int(v_TexIndex)], v_Input.texCoord * v_Input.tilingFactor);
	// TODO: use a switch case for GPU compatibility

	color2 = v_EntityId;
}
