#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

out vec3 pass_Normal;
out vec3 pass_vert_pos;
out vec3 pass_vert_pos_world;
out vec2 pass_TexCoord;

//we tried to follow wikipedia: https://en.wikipedia.org/wiki/Blinn–Phong_shading_model
void main(void)
{
	pass_TexCoord = in_TexCoord.xy;
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);

	vec4 vertPos4 =   ModelMatrix *  vec4(in_Position, 1.0); //modelview= view*model?

	pass_vert_pos = vec3(vertPos4) / vertPos4.w;
	//with camera rotation
	pass_vert_pos_world = vec3(ViewMatrix * vertPos4);
	pass_Normal = vec3(NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	//without camera rotation
	pass_vert_pos = vec3(ViewMatrix * vec4(vertPos4.xyz,0.0)).xyz;
}
