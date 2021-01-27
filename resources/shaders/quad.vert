#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;

//uniform bool grey_bool;
uniform bool horizont_bool;
uniform bool vert_bool;
//uniform bool blur_bool;


out vec2 pass_TexCoord;
//out bool grey_bool;

void main(void)
{

gl_Position = vec4(in_Position, 1.0);

	if(horizont_bool == false){
		if(vert_bool == false){
			pass_TexCoord = in_TexCoord;
		}
		else{
			pass_TexCoord = in_TexCoord * vec2(1.0,-1.0);
		}
	}
	else{
		if(vert_bool == false){
			pass_TexCoord = in_TexCoord * vec2(-1.0,1.0);
		}
		else{
			pass_TexCoord = in_TexCoord * vec2(-1.0,-1.0);
		}
	}

//	pass_TexCoord = in_TexCoord;

}
