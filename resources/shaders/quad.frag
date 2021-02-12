#version 150


in vec2 pass_TexCoord;
in vec4 pass_PP_Mode;

uniform sampler2D ColorTex;

out vec4 out_Color;

const float pixelsize_x = 1.0 / 640;
const float pixelsize_y = 1.0 / 480;
const mat3  kernel      = mat3(1.0/16, 1.0/8, 1.0/16, 1.0/8, 1.0/4, 1.0/8, 1.0/16, 1.0/8,1.0/16);


vec4 grey = vec4(0.299,0.587,0.114,1.0);
vec4 temp_color;
vec2 texcoord = pass_TexCoord;
float luminance;


void main(void)
{
	out_Color = texture(ColorTex, pass_TexCoord);

	if(pass_PP_Mode.y == 1){
		texcoord.x =  1-texcoord.x;
		out_Color = texture(ColorTex, texcoord);
			}


	if(pass_PP_Mode.z == 1){
		texcoord.y =  1-texcoord.y;
		out_Color = texture(ColorTex, texcoord);
			}
	if(pass_PP_Mode.w == 1){
		vec4 sum = vec4(0);
		int i = 0;
		for(int x = -1; x<= 1; x++){
			int j = 0;
			for(int y = -1; y<= 1; y++){
				sum += texture(ColorTex, vec2(texcoord.x + x* pixelsize_x, texcoord.y + y* pixelsize_y))*kernel[i][j];
				++j;
			}
			++i;
		}
		out_Color= sum;
	}


	if(pass_PP_Mode.x == 1){
		temp_color = texture(ColorTex, pass_TexCoord);
		luminance = temp_color.x * 0.2126 + temp_color.y * 0.7152 + temp_color.z * 0.0722;
		out_Color = vec4(luminance, luminance, luminance, 0);
	}
}
