#version 150

in vec2 position;

void main() {
	gl_Position = vec4( position.y, position.x, 0.0, 1.0 );
}
