#version 150

smooth in vec3 front_color;
smooth in vec3 back_color;

out vec4 out_color;

void main()
{
	if(gl_FrontFacing)
	{
		out_color = vec4(front_color, 1.0);
	}
	else
	{
		out_color = vec4(back_color, 1.0);
	}
}
