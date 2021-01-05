#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform mat4 NormalMatrix;
uniform vec3 Planet_Color;


out vec3 pass_Normal_world;
out vec3 pass_Normal_view;
out vec3 pass_vert_Pos;
out vec3 pass_vert_Pos_view;

out vec3 pass_Color;


void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix)*  vec4(in_Position, 1.0);

	vec4 vertPos4 = ModelMatrix * vec4(in_Position, 1.0);
	pass_vert_Pos = vec3(vertPos4)/vertPos4.w;

	vec4 vertPos4_view  = ViewMatrix * ModelMatrix * vec4(in_Position, 1.0);
	pass_vert_Pos_view = vec3(vertPos4_view)/vertPos4_view.w;

	pass_Normal_world = (ModelMatrix * vec4(in_Normal, 0.0)).xyz;
	pass_Normal_view = (ViewMatrix * ModelMatrix * vec4(in_Normal, 0.0)).xyz;

	//pass_vert_Pos = vec3(ViewMatrix * vec4(vertPos4.xyz, 0.0)).xyz;

	pass_Color = Planet_Color;
}
