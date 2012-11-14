#version 150

out vec4 outColor;
void main() 
{
	outColor = vec4( gl_FragCoord.y/640, 0.0, 0.0, 1.0 );
}
