#version 150

#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO

//Matrix Uniforms as specified with glUniformMatrix4fv
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

smooth out vec3 pass_TexCoord;

void main(void)
{
	mat3 view_3 = mat3(ViewMatrix);
	gl_Position = ProjectionMatrix * mat4(view_3) * vec4(in_Position, 1.0);
	pass_TexCoord = in_Position;
}
