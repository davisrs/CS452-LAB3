//Create a 3D POLYHEDRON NOT A CUBE -X
//Use your own header file for shading the polygon.	-X
//Use keyboard controls to rotate, translate, and scale the object.
//Use W,A,S,D to translate the object up, left, down, and right respectively.
//Use I,J,K,L to rotate the object up, left, down, and right repsectively.
//Use E,Q to scale the object smaller and bigger respectively.

#include "ShaderLoader.cpp"
#include "Angel.h"
#include <cstdlib>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

using namespace std;

void rotate(GLuint locate);

GLfloat pit,yaw,scalar=1;
glm::vec3 cubeTran;

GLuint vaoID, vboID[2];//vao and vbo names
GLuint eboID;
GLuint program;
				//x	y	z	//Vertex Array Specifies the points of a shape
GLfloat vertexarray[]={	-0.5f,0.5f,	0.0f,	//0
				0.5f,	0.5f,	0.0f,	//1
                       	0.5f,	0.5f,	0.0f,	//2
                       -0.5f,-0.5f,	0.0f,	//3
                       	0.0f,	0.0f,	0.5f	//4
                       };
                       //r	g	b	alpha	//color array
GLfloat colorarray[]={	1.0f,	1.0f,	0.0f,	1.0f,	//0-yellow
                       	0.0f,	1.0f,	0.0f,	1.0f,	//1-green
                       	1.0f,	0.0f,	1.0f,	1.0f,	//2-purple
                       	0.5f,	0.5f,	1.0f,	1.0f,	//3-indigo
                       	1.0f,	0.5f,	0.5f,	1.0f	//4-peach
                       };

GLubyte elems[]={	0,1,2,	//base
			1,3,2,	//base
			0,2,4,	//l
			0,1,4,	//t
			1,3,4,	//r
			2,3,4
};

void init(){
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, 600, 600);
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

//No idea what this does
  glGenBuffers(1,&eboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elems),elems,GL_STATIC_DRAW);

  program = InitShader("vertexshader.glsl", "fragmentshader.glsl");//Load shaders and use the resulting shader program
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void drawscene(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glUseProgram(program);
  glm::mat4 trans;
  trans=glm::translate(trans,cubeTran);//translate the cube
  trans=glm::rotate(trans,pit,glm::vec3(1,0,0));//rotate the cube around the x axis
  trans=glm::rotate(trans,yaw,glm::vec3(0,1,0));//rotate the cube arround the y axis
  trans=glm::scale(trans,glm::vec3(scalar));//scaling the cube
  GLint tempLoc = glGetUniformLocation(program,"modelMatrix");//Matrix that handle the transformations
  glUniformMatrix4fv(tempLoc,1,GL_FALSE,&trans[0][0]); 
  glDrawElements(GL_TRIANGLES,sizeof(elems),GL_UNSIGNED_BYTE,NULL);
  glFlush();
  glutSwapBuffers();
  glutPostRedisplay();
}

void mousepress(int button, int state, int x, int y){
  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    exit(0);
  else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
    drawscene();
  }
}

void mousemove(int x, int y){//passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed. 
	if(x>1){
		yaw+=(x-300)/10.0;
		printf("\nIncYaw\n");
	}
	if(y>1){
	pit+=(y-300)/10.0;
	printf("\nIncPitch\n");
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
		printf("\nUp\n");
		break;
	case 's':
		cubeTran.y-=2;
		printf("\nDown\n");
		break;
	case 'a':
		cubeTran.x-=2;
		printf("\nLeft\n");
		break;
	case 'd':
		cubeTran.x+=2;
		printf("\nRight\n");
		break;
	case 'e':
		scalar+=.1f;
		printf("\nScaleUp\n");
		break;
	case 'q':
		scalar-=.1f;
		printf("\nScaleDown\n");
		break;
	case 'i':
		pit+=2;
		printf("\nIncPitch\n");
		break;
	case 'k':
		pit-=2;
		printf("\nDecPitch\n");
		break;
	case 'j':
		yaw+=2;
		printf("\nIncYaw\n");
		break;
	case 'l':
		yaw-=2;
		printf("\nDecYaw\n");
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
  glutMotionFunc(mousemove);
  glutMainLoop();//sets opengl state in a neverending loop
  return 0;
}
