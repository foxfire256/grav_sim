#version 150

// Two sided, per vertex phong

uniform mat4 MV;
uniform mat4 MVP;
uniform vec4 light_pos;

uniform vec3 La, Ld, Ls;
uniform vec3 Ka, Kd, Ks;
uniform float shininess;
uniform mat3 normal_matrix;

in vec3 vertex;
in vec3 normal;

smooth out vec3 front_color;
smooth out vec3 back_color;

vec3 phong(vec4 position, vec3 normal)
{
	vec3 s = normalize(vec3(light_pos - position));
	vec3 v = normalize(-position.xyz);
	vec3 r = reflect(-s, normal);
	vec3 ambient = La * Ka;
	float s_dot_n = max(dot(s, normal), 0.0);
	vec3 diffuse = Ld * Kd * s_dot_n;
	vec3 specular = vec3(0.0);

	if(s_dot_n > 0.0)
	{
		specular = Ls * Ks * pow(max(dot(r, v), 0.0), shininess);
	}

	return ambient + diffuse + specular;
}

void main()
{
	vec3 tnorm = normalize(normal_matrix * vec3(normal));
	vec4 eye_coords = MV * vec4(vertex.xyz, 1.0);
	
	front_color = phong(eye_coords, tnorm);
	back_color = phong(eye_coords, -tnorm);

	gl_Position = MVP * vec4(vertex.xyz, 1.0);
	//gl_Position = P * MV * vec4(vertex.xyz, 1.0);
}
