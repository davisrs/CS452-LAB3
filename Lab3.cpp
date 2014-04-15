//Create a 3D POLYHEDRON NOT A CUBE -X
//Use your own header file for shading the polygon.	-X
//Use keyboard controls to rotate, translate, and scale the object.
//Use W,A,S,D to translate the object up, left, down, and right respectively.
//Use I,J,K,L to rotate the object up, left, down, and right repsectively.
//Use E,Q to scale the object smaller and bigger respectively.

#include "ShaderLoader.cpp"
#include "Angel.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

using namespace std;

void rotate(GLuint locate);

GLuint vaoID, vboID[2];//vao and vbo names
GLuint eboID;
GLuint program;
			//x	y	z	//Vertex Array Specifies the points of a shape
GLfloat vertexarray[]={	-0.5f,	0.5f,	0.0f,	//0
			0.5f,	0.5f,	0.0f,	//1
                       	0.5f,	0.5f,	0.0f,	//2
                       	-0.5f,	-0.5f,	0.0f,	//3
                       	0.0f,	0.0f,	0.5f,	//4
                       };
                       //r	g	b	alpha	//color array
GLfloat colorarray[]={	1.0f,	1.0f,	0.0f,	1.0f,	//0-yellow
                       	0.0f,	1.0f,	0.0f,	1.0f,	//1-green
                       	1.0f,	0.0f,	1.0f,	1.0f,	//2-purple
                       	0.5f,	0.5f,	1.0f,	1.0f,	//3-indigo
                       	1.0f,	0.5f,	0.5f,	1.0f,	//4-peach
                       };

GLfloat pit,yaw,scalar=1;
glm::vec3 cubeTran;

void init(){

  program = InitShader("vertexshader.glsl", "fragmentshader.glsl");//Load shaders and use the resulting shader program
  
  //Create a vertex array object
  glGenVertexArrays(1, &vaoID);//generates 1 vertex array objects for vaoID
  glBindVertexArray(vaoID);//binds VAO
  
  glGenBuffers(2, vboID);//generates 2 vertex buffer objects that are associated with vboID
  glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertexarray),vertexarray,GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);//defines an array of vertex attribute data
    
  glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(colorarray),colorarray,GL_STATIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void drawscene(){
  glClear(GL_COLOR_BUFFER_BIT);
  glm::mat4 trans;
 
  //trans= glm::translate(glm::mat4(1.0f));

  trans=glm::translate(trans,cubeTran);//translate the cube
  trans=glm::rotate(trans,pit,glm::vec3(1,0,0));//rotate the cube around the x axis
  trans=glm::rotate(trans,yaw,glm::vec3(0,1,0));//rotate the cube arround the y axis
  trans=glm::scale(trans,glm::vec3(scalar));//scaling the cube
  glDrawArrays(GL_POLYGON,0,3);
  GLint tempLoc = glGetUniformLocation(program,"modelMatrix");//Matrix that handle the transformations
  glUniformMatrix4fv(tempLoc,1,GL_FALSE,&trans[0][0]);
  glDrawElements(GL_POLYGON,24,GL_UNSIGNED_BYTE,NULL);
  glFlush();
}

void mousepress(int button, int state, int x, int y){
  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    exit(0);
  else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
    drawscene();
  }
}

void keypress(unsigned char key, int x, int y){
	switch( key ) {
	case 27://escape
		fprintf(stderr, "Gave up already?\n");
		exit( EXIT_SUCCESS );
		break;
	case 'w':
		cubeTran.y+=2;
		break;
	case 's':
		cubeTran.y-=2;
		break;
	case 'a':
		cubeTran.x-=2;
		break;
	case 'd':
		cubeTran.x+=2;
		break;
	case 'e':
		scalar+=.1f;
		break;
	case 'q':
		scalar-=.1f;
		break;
	case 'i':
		pit+=2;
		break;
	case 'k':
		pit-=2;
		break;
	case 'j':
		yaw+=2;
		break;
	case 'l':
		yaw-=2;
		break;
	}
}

int main(int argc,char ** argv){

  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Lab 3 - Not a cube");//creates the window with the specified name
  
  //initializes glew
  glewExperimental=GL_TRUE;
  if(glewInit()){//initialize glew, if it fails...
	fprintf(stderr, "Unable to initalize GLEW");//Yell at programmer GLEW not working
	exit(EXIT_FAILURE);//kill process, exit in FAILURE if GLEW doesn't work'
  }
  
  glutInitContextVersion(4, 3);//specifies the version of opengl
  glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_COMPATIBILITY_PROFILE);//specifies what profile your using
	
  init();//Call init function

  //returns what version of opengl and glsl your computer can use
  const GLubyte* version=glGetString(GL_SHADING_LANGUAGE_VERSION);
  fprintf(stderr,"Opengl glsl version %s\n", version);

  version=glGetString(GL_VERSION);
  fprintf(stderr,"Opengl version %s\n", version);

  glutDisplayFunc(drawscene);//displays callback draws the shapes
  glutMouseFunc(mousepress);//get mouse inputs
  glutKeyboardFunc(keypress);//get keyboard inputs
  glutMainLoop();//sets opengl state in a neverending loop
  return 0;
}
