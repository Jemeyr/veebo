#version 150

out vec4 outColor;

uniform vec3 triColor;

void main() 
{
	outColor = vec4( triColor.x, 1.0, 0.0, 1.0 );
}
