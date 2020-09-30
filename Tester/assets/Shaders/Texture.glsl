#type vertex
#version 330 core
		
layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTexScale;
	
uniform mat4 uViewProjection;

out vec2 vTexCoords;
out vec4 vColor;
out float vTexIndex;
out float vTexScale;

void main()
{
	vTexScale = aTexScale;
	vTexIndex = aTexIndex;
	vTexCoords = aTexCoord;
	vColor = aColor;
	gl_Position = uViewProjection * aPosition;
}

#type fragment
#version 330 core
		
layout(location = 0) out vec4 color;

in vec2 vTexCoords;
in vec4 vColor;
in float vTexIndex;
in float vTexScale;

uniform sampler2D uTexture[32];

void main()
{
	color = texture(uTexture[int(vTexIndex)], vTexCoords * vTexScale) * vColor;
}