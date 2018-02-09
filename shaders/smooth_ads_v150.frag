#version 150
smooth in vec3 intensity;
uniform vec4 color;

out vec4 out_color;

void main()
{
	vec3 c = color.rgb * intensity;
	out_color = vec4(c, 1.0);
}
