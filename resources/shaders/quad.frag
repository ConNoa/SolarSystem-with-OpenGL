#version 150


in vec2 pass_TexCoord;
uniform bool grey_bool;
uniform sampler2D ColorTex;

out vec4 out_Color;

vec4 grey = vec4(0.299,0.587,0.114,1.0);
vec4 temp_color;
float luminance;


void main(void)
{
	if(grey_bool == false){
		out_Color = texture(ColorTex, pass_TexCoord);
	}
	else{
		temp_color = texture(ColorTex, pass_TexCoord);
		luminance = temp_color.x * 0.2126 + temp_color.y * 0.7152 + temp_color.z * 0.0722;
		out_Color = vec4(luminance);
	}
}
