#define GLEW_STATIC
#define NO_SDL_GLEXT

#include <iostream>
#include <fstream>

#include "GL/glew.h"

#include "SDL/SDL.h"

#include "math.h"

#define WIDTH	640
#define HEIGHT	480


const char* vertexSource =
	"#version 150\n"
	"in vec2 position;"
	"void main() {"
	"	gl_Position =	 vec4( position.y, position.x, 0.0, 1.0 );"
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

GLuint MakeShader(std::string file_name)
{
	GLenum shader_type;

	//get the shader type from extension
	std::string::size_type index;
	index = file_name.rfind('.');

	if(index != std::string::npos)
	{
		std::string extension = file_name.substr(index+1);
		
		//get enum from extension
		if(!extension.compare("glslv"))
		{
			shader_type = GL_VERTEX_SHADER;
		}
		else if(!extension.compare("glslf"))
		{
			shader_type = GL_FRAGMENT_SHADER;
		}
		else
		{
			std::cout << "File extension \"" << extension << "\" is not valid" << std::endl;
			return 0;
		}
		
	}
	else
	{
		std::cout << "No file extension" << std::endl;
		return 0;
	}
	
	
	//read in file
	std::string line,file;
	std::ifstream in(file_name.c_str());
	while(std::getline(in, line))
	{
 		file += line + "\n";
	}
	const char* shader_source = file.c_str();
	
	
	//compile shader and stuff
	GLuint shader;
	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader); 
	
	checkCompileStatus(shader);
	
	
	return shader;
}

	
 
void mainLoop() {
	
	float vertices[] = {	 0.0f,  0.5f,
				 0.5f, -0.5f,
				-0.5f, -0.5f }; 

	//a vertex buffer object id
	GLuint vbo;
	glGenBuffers( 1, &vbo );		

	//attribute array
	GLuint vao;
	glGenVertexArrays( 1, &vao );
	
	//bind attr array
	glBindVertexArray( vao );
	

	//bind it to array buffer
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
	
	
	//make shaders
	GLuint vertexShader = MakeShader("vertexShader.glslv");
	GLuint fragmentShader = MakeShader("fragmentShader.glslf");
	
	
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
	
	//////////////////
	
	
	
	//color
	GLuint uniColor = glGetUniformLocation(shaderProgram, "triColor" );
	
	glUniform3f( uniColor, 1.0, 0.0, 1.0 );
	
	//clear to dark green	
	glClearColor( 0.0f, 0.25f, 0.0f, 1.0f );
	
	while(true) {

		float time = (float)clock() / (float)CLOCKS_PER_SEC;
		glUniform3f( uniColor, ( sin( time * 4.0f ) + 1.0f ) / 2.0f, 0.0f, 0.0f );
	
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




