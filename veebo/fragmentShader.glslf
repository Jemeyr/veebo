#version 150

uniform vec3 triColor;

out vec4 outColor;
void main() 
{
	outColor = vec4( triColor, 1.0 );
}
