#define GLEW_STATIC
#define NO_SDL_GLEXT

#include <iostream>

#include "GL/glew.h"

#include "SDL/SDL.h"

#define WIDTH	640
#define HEIGHT	480


const char* vertexSource =
	"#version 150\n"
	"in vec2 position;"
	"void main() {"
	"	gl_Position = vec4( position.y, position.x, 0.0, 1.0 );"
	"}";
const char* fragmentSource =
	"#version 150\n"
	"out vec4 outColor;"
	"void main() {"
	"	outColor = vec4( gl_FragCoord.y/640, 0.0, 0.0, 1.0 );"
	"}";

void endProgram(int code) {SDL_Quit();	exit(code);}


void checkCompileStatus( GLuint shader )
{

	
	GLint status;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
	
	if( status != GL_TRUE )
	{
		char buffer[513];
		glGetShaderInfoLog( shader, 512, NULL, buffer );
		
		std::cout << "Jeremy, did you forget to use optirun?\n";
		std::cout << "ABORTING:\n";
		std::cout << buffer;
		
		endProgram(0);
		
	}
}


void handleKeys(SDL_keysym* keysym, bool state) {
	switch(keysym->sym) {
		case SDLK_ESCAPE:	endProgram(0); break;
	}
}


// Process SDL events
void processEvents() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:	handleKeys(&event.key.keysym, true );	break;
			case SDL_KEYUP  :	handleKeys(&event.key.keysym, false);	break;
			case SDL_QUIT   :	endProgram(0);	break;
		}
	}
}

 
void mainLoop() {
	
	
	
	float vertices[] = { 0.0f,  0.5f,
						 0.5f, -0.5f,
						-0.5f, -0.5f }; 
	//a vertex buffer object id
	GLuint vbo;
	glGenBuffers( 1, &vbo );		

	//attribute array
	GLuint vao;
	glGenVertexArrays( 1, &vao );
	
	glBindVertexArray( vao );
	


	//bind it to array buffer
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
	
	
	//shadorz
	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader, 1, &vertexSource, NULL );
	glCompileShader( vertexShader );
	
	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShader, 1, &fragmentSource, NULL );
	glCompileShader( fragmentShader );
		
	
	//complain about having out of date glsl
	checkCompileStatus( vertexShader );
	checkCompileStatus( fragmentShader );
	

	
	//shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );
	
	//bind output
	glBindFragDataLocation( shaderProgram, 0, "outColor" );
	
	//link program
	glLinkProgram( shaderProgram );
	glUseProgram( shaderProgram );

	//attribute
	GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );	
	
	//link it up
	glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( posAttrib );
	
	
	
	
	glClearColor( 0.0f, 0.25f, 0.0f, 1.0f );
	
	while(true) {
	
		processEvents();
	
 		//graphics happen here
		glClear( GL_COLOR_BUFFER_BIT );
		
		// Draw a triangle from the 3 vertices
		glDrawArrays( GL_TRIANGLES, 0, 3 );

		// Swap buffers TODO
		SDL_GL_SwapBuffers();
	}
}
 
// Init everything
int main(int argc, char* argv[]) {

	
	SDL_Init(SDL_INIT_VIDEO);


	SDL_WM_SetCaption( "VeeBO", NULL );
	
	SDL_SetVideoMode(WIDTH, HEIGHT, 0, SDL_OPENGL | SDL_GL_DOUBLEBUFFER);
	
	glViewport(0, 0, WIDTH, HEIGHT);
	
	glewInit();


	
	mainLoop();
	return 0;
}




