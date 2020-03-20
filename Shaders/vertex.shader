#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 p;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * vec4(position, 1.0);
   v_TexCoord=texCoord;
   //p = wLookAt + wRight * cCamWindowVertex.x + wUp * cCamWindowVertex.y;
}






