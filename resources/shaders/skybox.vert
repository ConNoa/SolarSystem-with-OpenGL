#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
//found tthis on https://gamedev.stackexchange.com/questions/60313/implementing-a-skybox-with-glsl-version-330

//Matrix Uniforms as specified with glUniformMatrix4fv
layout(location = 0) in vec4 in_Position;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

smooth out vec3 pass_cam_dir;

void main(void)
{
	mat4 inverseProjection = inverse(ProjectionMatrix);
	mat3 inverseModelview  = transpose(mat3(ViewMatrix));
	vec3 unprojected       = (inverseProjection * in_Position).xyz;
	pass_cam_dir           = inverseModelview * unprojected;
	gl_Position = in_Position;
}
