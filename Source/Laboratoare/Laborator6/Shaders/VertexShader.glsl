#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 1) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
out vec2 frag_texture;

void main()
{
	// TODO: send output to fragment shader
    frag_color = v_color;
	frag_normal = v_normal;
	frag_texture = v_texture;

	// TODO: compute gl_Position
	vec3 v_final = v_position + vec3(sin(time), 0, 0);
    gl_Position = Projection * View * Model * vec4(v_final, 1.0);
}
