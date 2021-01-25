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
uniform vec3 Planet_Color;

uniform float ShadingMethod;

out vec3 pass_Normal_world;
out vec3 pass_Normal_view;
out vec3 pass_vert_Pos;
out vec3 pass_vert_Pos_view;
out float pass_ShadingMethod;

out vec3 pass_Color;
out vec2 pass_TexCoord;


void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix)*  vec4(in_Position, 1.0);
	vec4 vertPos4 = ModelMatrix * vec4(in_Position, 1.0);
	pass_vert_Pos = vec3(vertPos4).xyz/vertPos4.w;
	//pass_vert_Pos = vec3(ViewMatrix*vertPos4);

	vec4 vertPos4_view  = (ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	pass_vert_Pos_view = vec3(vertPos4_view).xyz/vertPos4_view.w;

	pass_Normal_world = (ModelMatrix * vec4(in_Normal, 0.0)).xyz;
	pass_Normal_view = vec3((ViewMatrix * ModelMatrix) * vec4(in_Normal, 0.0)).xyz;

	//pass_vert_Pos = vec3(ViewMatrix * vec4(vertPos4.xyz, 0.0)).xyz;
	pass_ShadingMethod = ShadingMethod;
	pass_Color = Planet_Color;
}
