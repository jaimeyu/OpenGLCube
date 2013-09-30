//***********************************************************************
/* Written by Jaime Yu
* ID: 5428998
* Assignment 4 for COMP 471
* Fall 2008
*/
//***********************************************************************


/*
IMPORTANT NOTE!!
OpenGL32.lib GLu32.lib GLaux.lib add this to the object link in project-> settings

  this is because i'm using the CBMPLoader library
  makes life easier since he already wrote a class file to 
  convert bmps into a texture
  
	why reinvent the wheel. 
	
	  honestly, i could just steal the code but that isn't honest. 
	  
		btw: http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=06
		he also has code available to do this.
		
*/

#include <iostream>
#include <windows.h>
#include <math.h>

#include "CBMPLoader.h"
#include <GL/glut.h>
using namespace std;


void myinit(void)
{


/*
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 10.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 0.0, 5.0, -3.0, 0.0 };
	GLfloat ambiant[] = { 10.0, 10.0, 10.0, 10.0 };

    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiant );


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
*/


}








//***********************************************************************
// constants
// to do for future assignments

const float PI = 3.14159;               //TO THE 5TH DECIMAL! This will cause 
//errors since its not PRECISE!
const int screen_orig = 300;    //The screen starts off as a SQUARE
const int orig_fov = 15;                //FOV angle range is 1 <fov <89
//anything else and it causes errors

//***********************************************************************


//***********************************************************************
//GLOBAL VARIABLES

int rot = 0;                                    //amount of rotation applied to cube
bool viewAngleYZ = true;                //needed to figure out whether camera is on Z or ZY axis.

float add_me = 0;

float zoomZ = 1;                                //field of view according to z location
float fovyZoom = orig_fov;              //field of view according to y to x angle

int angleX = 0;
int angleY = -90;                               //its 90 because 0 deg is pointing to x-z
int angleZ = 0;
int old_angleZ = 0;

GLfloat fovTemp;
GLfloat screen_w = screen_orig;
GLfloat screen_h = screen_orig;

//camera locations
float loc_x = 0;
float loc_y = 0;
float loc_z = 5;

float old_loc_x, old_loc_y, old_loc_z = 0;



//camera is looking at
float lookAtX = 0;
float lookAtY = 0;
float lookAtZ = 0;
float old_lookAtX, old_lookAtY, old_lookAtZ = 0;

//obsolete upvector data sets -- CODE REMOVED!!
float upVectorX = 0;                    //camera roll on Z axis
float upVectorY = 0;
float upVectorZ = 0;                    //probably not used

//rotation states
bool rot_stop = true;                   //is rotating?
bool rot_way = false;                   //which way is it rotating?

//anti aliasing -- CODE REMOVED!!!
bool AA = false;

//Camera Vectors
float vectorX = 0;
float vectorY = 0;
float vectorZ = -1;

//Textures
//CBMPLoader Texture; 

CBMPLoader front;
CBMPLoader back;
CBMPLoader rightt;	//ok the two 't' is beccause right and left are already used.
CBMPLoader leftt;	//i don't really care to find out where
CBMPLoader top;
CBMPLoader bottom;

GLuint texturelist[6];

//light

bool islightOn = false;

//END GLOBAL VARIABLES
//***********************************************************************


//***********************************************************************
//FUNCTIONS

//cube rotation
void rotate(int);               //rotate cube by int amounts
void idleRotate(); //allows the cube to rotate without user interaction


//objects
void diplayCubeTexture();		//draw a cube with TEXTURES
void displayCube();             //draw the cube WITHOUT TEXTURES
void displayAxis();             //draw the axis
void displayCubeMess(); //draws all the cubes in space
void displaySky();              //draws the sky, obsolete -- CODE REMOVED!!


//draw to screen
void objects();                 //call this to draw objects on screen
void screen();                  //perspective and camera position
void resize(int, int);  //when the user changes window size, it keeps
//the proper aspect ratio

//inputs
void keyboard(unsigned char,int,int); //kb cmds
void mouseButton(int, int, int, int); //ms cmds
void kbArrows(int, int, int);	//arrow keys

//instructions
void printInstructions();       //shows the instructions to the applicaton

//textures
void loadTextures();             //This loads several textures into memory so 
//they can be referenced

//old code
void old_camera_loc();  //CODE REMOVED!!
//end old code

//DIRECTIONAL VECTORS (VECTORS ALREADY HAVE DIRECTION!)
void calculateVector();                 //calculates the users inputs into vectors
void calculateLookAtPosition(); //calculates where the camera should be looking
void calculateZedPlane();               //OBSOLETE -- CODE EXIST!!
void calculateLocation();               //caculates where the camera should be



//lighting
void setupLight();
void cubeMaterial();


//debug code
void debugRawData();

//END FUNCTIONS
//***********************************************************************


//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************

//APPLICATION CODE STARTS HERE
//
//
//
//


int main(int argc, char** argv)
{
	
	system("cls");
	//***********************************************************************
	//Introduction
	//
	//
	cout << "*****************************************************************"
		<<endl;
	cout << "Unit Cube application written by Jaime Yu, ID: 5428998. FALL 2008" ;
	cout << endl;
	cout << "Assignment 03 " <<endl;
	cout << endl;
	cout << "*****************************************************************"
		<<endl;
	
	printInstructions();
	//end introduction
	//***********************************************************************
	
	//***********************************************************************
	//initiations of glut
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ); //color | double buffer | ignores things you cannot see.
	
	//***********************************************************************
	
	
	//***********************************************************************
	
	//initialize windows
	glutInitWindowSize( screen_w, screen_h);
	glutInitWindowPosition( 100, 100);
	
	glutCreateWindow( "Jaime Yu, 5428998, COMP 471 Assignment 2" );
	
	
	//***********************************************************************
	
	
	
	//***********************************************************************
	
	   loadTextures();				//load textures into memory
	   

		







       glutReshapeFunc(resize);
       glutMouseFunc(mouseButton); //catch mouse clicks
       glutKeyboardFunc(keyboard); //catch kb keys
	   
       glutDisplayFunc(objects); //draw objects
       glutIdleFunc(idleRotate); //rotate objects when idle
	   glutSpecialFunc(kbArrows);//kb arrows	
	   
	   
       //***********************************************************************
       //***********************************************************************
	   
	   
       //***********************************************************************
	   
       //***********************************************************************
	   
       glutMainLoop(); //main loop
	   
	   
       //***********************************************************************
       //The application will never ever arrive here.
       //So don't code here!
	   
       cout << "MAGIC!?" << endl;
	   
       return 0;
}


//***********************************************************************



//***********************************************************************
void idleRotate()
{
	
	if ((rot_stop == false) && rot_way == true)
		rotate(1);
	else if ((rot_stop == false) && rot_way == false)
		rotate(-1);
	
	
	
	
}

//***********************************************************************



//***********************************************************************

void rotate( int angle)
{
	rot += angle;   //  increment the rotation angle
	rot %=360;              //  rot mod 360 keeps it within the ranges 0 - 359
}

//***********************************************************************

//***********************************************************************

void resize(int w, int h)
{
	screen_w = w; //i'm going for simplicity
	screen_h = h; //just updates global variables
	
	cout << "Someone changed the size of the window! It is now " << w <<
		"x" << h << "px." <<endl;
	
	
}

//***********************************************************************


//***********************************************************************

void screen()
{
	glEnable(GL_DEPTH_TEST); //enable buffering, i have it set to double buffering
	
	glViewport(0, 0, screen_w, screen_h);
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	
	
	//teacher's resize function modified for this application
	GLdouble left,right,bottom,top,scale=tan(fovyZoom*PI/180);
	left=-screen_w*scale/screen_orig; right = screen_w*scale/screen_orig;
	bottom = -screen_h*scale/screen_orig; top = screen_h*scale/screen_orig;
	glFrustum(left,right,bottom,top, 1.0, 250.0);
	//end teachers
	
	
	
	
	glMatrixMode( GL_MODELVIEW );
	
	glClearColor( 0, 0, 0, 0 ); //bkg is now blk
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
		GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glLoadIdentity(); // clear modelview
	
	
	//********************************************************************************************
	
	
	
	//*********************************************************
	
	//old_camera_loc();
	
	//camera roll
	glRotatef(angleZ, 0,0,1);
	
	
	if (viewAngleYZ == false) //camera is looking @ 45deg angle to center on Y-Z axis//the X-Y-Z 45 deg view is cooler though
	{
		
		loc_x = 0;
		loc_y = 5;
		loc_z = 5;
		
		lookAtX = 0;
		lookAtY = 0;
		lookAtZ = 0;
		
	}
	
	
	gluLookAt( loc_x, loc_y,loc_z , lookAtX, lookAtY, lookAtZ, 0, 1,0);
	//camera goes to 0,0,1.5 and looks at 0,0,0
	
	
	
}

//***********************************************************************

void old_camera_loc() // obsolete, replaced by vector functions
{
	// the following code moves the point where the camera looks at
	//
	// loc is where the camera is located
	// the addition part is the relative position(to the center of the camear)
	// of what the camera is looking at.
	//
	// the reason you see so many PI/180 is because the trig functions are in radians! NOT deg!
	
	lookAtX = loc_x + ( cos( angleY*PI/180 ) *cos( angleX*PI/180 )  );
	
	lookAtY = loc_y + ( sin( angleX*PI/180 ) ) ;
	lookAtZ = loc_z + ( sin( angleY*PI/180 ) *cos( angleX*PI/180 )  );
	
	
	
	
	//*********************************************************
	
	
	if (viewAngleYZ == false) //camera is looking @ 45deg angle to center on Y-Z axis//the X-Y-Z 45 deg view is cooler though
	{
		
		loc_x = 0;
		loc_y = 5;
		loc_z = 5;
		
		lookAtX = 0;
		lookAtY = 0;
		lookAtZ = 0;
		
	}
	
	
}

//***********************************************************************

void objects()
{

	screen();


	setupLight();
	
  

	glPushMatrix();
	
	displayAxis(); //draw AXIS OF EVIL
	
	glPopMatrix();
	
	
	glPushMatrix();
	
	//glShadeModel(GL_SMOOTH);
	
	glRotatef(rot,0.5,-0.5,0.5); //rotates the cube by rot aruond the axis of (0.5,-0.5, 0.5) and (-0.5, 0.5,-0.5).
	
	
	//displayCube(); //draw unit cube
	diplayCubeTexture();
	
	
	glPopMatrix();
	
	
	glPushMatrix(); //draws the mess of cubes in space

	//	glShadeModel(GL_SMOOTH);

	displayCubeMess();
	
	glPopMatrix();
	
	
	glPushMatrix(); //draws the wiresphere for that cool matrix look...but mostly so the user can easily figure out his spatial surroundings
	
	//glShadeModel(GL_SMOOTH);

	glutWireSphere(200,50,50);
	
	glPopMatrix();
	
	
	
	glutSwapBuffers(); //buffer swap!
	glutPostRedisplay();
	
}


//***********************************************************************




void displayCubeMess()
{
	glPushMatrix();
	glTranslatef(0,0,-10);
	
	displayCube();
	
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslatef(0,0,10);
	
	displayCube();
	
	glPopMatrix();
	
	
	
	glPushMatrix();
	glTranslatef(10,10,0);
	
	displayCube();
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-10,10,0);
	
	displayCube();
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(10,-10,0);
	
	displayCube();
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-10,-10,0);
	
	displayCube();
	
	glPopMatrix();
	
	
	
	glPushMatrix();
	glTranslatef(10,10,10);
	
	displayCube();
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-10,10,10);
	
	displayCube();
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(10,-10,10);
	
	displayCube();
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-10,-10,10);
	
	displayCube();
	
	glPopMatrix();
	
}




//***********************************************************************
void displayAxis()
{
	
	glLineWidth (6.0);//FSM causes all LINES called to be 6pxs -- fixedat end of func
	
		


	glBegin(GL_LINES);
	
	
	
	// x axis
	glColor3f(1, 0, 0); //red
	glVertex3f(0, 0, 0);
	glVertex3f(10000, 0, 0);
	
	// y axis
	glColor3f(0, 1, 0); //green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10000, 0);
	
	// z axis
	glColor3f(0, 0, 1); //blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10000);
	
	glEnd();
	
	glLineWidth (1.0); // all other lines will be 1px now.
	
}

//***********************************************************************



//***********************************************************************
void diplayCubeTexture()
{
	//Build the square faces
	
	/* 
	texture cube
	i basically copied the old cube and mapped textures to it
	
	*/


	glEnable(GL_TEXTURE_2D);	   
	   
		glBindTexture(GL_TEXTURE_2D, texturelist[0]);
		glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	
	//front ****************************


	   glColor3f( 1, 1, 1 ); //white; helps makes the pic clear
   
	   glTexCoord2f(0.0, 0.0);
	   glVertex3f( -0.5, -0.5, 0.5);
       
	   glTexCoord2f(1.0, 0.0);
	   glVertex3f( 0.5, -0.5, 0.5);
       
	   glTexCoord2f(1.0, 1.0);
	   glVertex3f( 0.5, 0.5, 0.5);
       
	   glTexCoord2f(0.0, 1.0);
	   glVertex3f( -0.5, 0.5, 0.5);

	   glEnd();
       //front ****************************

       //back  ****************************



	   glBindTexture(GL_TEXTURE_2D, texturelist[1]);


	glBegin(GL_QUADS);
	   glColor3f( 1, 1, 1 ); //white 

	   glTexCoord2f(0.0, 0.0);
	   glVertex3f( -0.5, -0.5, -0.5);
       
	   glTexCoord2f(1.0, 0.0);
	   glVertex3f( 0.5, -0.5, -0.5);
       
	   glTexCoord2f(1.0, 1.0);
	   glVertex3f( 0.5, 0.5, -0.5);
       
	   glTexCoord2f(0.0, 1.0);
	   glVertex3f( -0.5, 0.5, -0.5);
		
	   glEnd();
       //back  ****************************


	   	//glDisable(GL_TEXTURE_2D);		//VERY IMPORTANT! YOU NEED THIS   

	   
       //top   ****************************

	   		glBindTexture(GL_TEXTURE_2D, texturelist[4]);

       	glBegin(GL_QUADS);

		
	   glColor3f( 1, 1, 1  ); //Yellow top

	   glTexCoord2f(0.0, 0.0);
       glVertex3f( -0.5, 0.5, 0.5);

	   glTexCoord2f(1.0, 0.0);
       glVertex3f( 0.5, 0.5, 0.5);

	   glTexCoord2f(1.0, 1.0);
       glVertex3f( 0.5, 0.5, -0.5);

	   glTexCoord2f(0.0, 1.0);
       glVertex3f( -0.5, 0.5, -0.5);

	   glEnd();
       //top   ****************************
	   
	   
       //bottom****************************


	   		glBindTexture(GL_TEXTURE_2D, texturelist[5]);

	   	glBegin(GL_QUADS);


		   glColor3f( 1, 1, 1  ); //PURPLE bottom
		   
		   glTexCoord2f(0.0, 0.0);
		   glVertex3f( -0.5, -0.5, 0.5);
       
		   glTexCoord2f(1.0, 0.0);
		   glVertex3f( 0.5, -0.5, 0.5);
       
		   glTexCoord2f(1.0, 1.0);
		   glVertex3f( 0.5, -0.5, -0.5);
       
		   glTexCoord2f(0.0, 1.0);
		   glVertex3f( -0.5, -0.5, -0.5);


	   glEnd();
       //bottom****************************
	   
       //right ****************************
	   
	   		glBindTexture(GL_TEXTURE_2D, texturelist[2]);

	   
	   glBegin(GL_QUADS);



       glColor3f( 1, 1, 1  );  //Fuscia? RIGHT

	   glTexCoord2f(0.0, 0.0);
       glVertex3f( 0.5, -0.5, 0.5);

	   glTexCoord2f(1.0, 0.0);
       glVertex3f( 0.5, -0.5, -0.5);

	    glTexCoord2f(1.0, 1.0);
       glVertex3f( 0.5, 0.5, -0.5);

	   glTexCoord2f(0.0, 1.0);
       glVertex3f( 0.5, 0.5, 0.5);

	   glEnd();
       //right ****************************
	   
       //left  ****************************
	   
	   		glBindTexture(GL_TEXTURE_2D, texturelist[3]);

	   
	   glBegin(GL_QUADS);

       glColor3f( 1, 1, 1  ); //WHITE LEFT

	   glTexCoord2f(0.0, 0.0);
       glVertex3f( -0.5, -0.5, 0.5);

	   glTexCoord2f(1.0, 0.0);
       glVertex3f( -0.5, -0.5, -0.5);

	    glTexCoord2f(1.0, 1.0);
       glVertex3f( -0.5, 0.5, -0.5);

	   glTexCoord2f(0.0, 1.0);
       glVertex3f( -0.5, 0.5, 0.5);
       glEnd();
       //left  ****************************


	   glFlush();
	   	   glDisable(GL_TEXTURE_2D);

	   
}

void displayCube()
{
	
	//Build the square faces
	
	/* fun facts!
	* The cube is a unit cube, which has has length, width, depth of (1,1,1).
	* The exact center of the cube is at (0,0,0).
	* Hence all the 0.5 that you see below.
	*
	* Important note
	* THE CUBE DOES NOT CHANGE POSITION.
	* I HAVE NOT USED ANY TRANSFORM FUNCTIONS ON IT.
	* ONLY THE CAMERA MOVES.
	* Technically speaking, you can animate the camera to rotate around the cube.
	*/
	glBegin(GL_QUADS);
	
	//front ****************************
	glColor3f( 0, 1, 0 ); //GREEN FRONT
	glVertex3f( -0.5, -0.5, 0.5);
	glVertex3f( 0.5, -0.5, 0.5);
	glVertex3f( 0.5, 0.5, 0.5);
	glVertex3f( -0.5, 0.5, 0.5);
	//front ****************************
	
	//back  ****************************
	glColor3f( 1, 0, 0 ); //RED BACK
	glVertex3f( -0.5, -0.5, -0.5);
	glVertex3f( 0.5, -0.5, -0.5);
	glVertex3f( 0.5, 0.5, -0.5);
	glVertex3f( -0.5, 0.5, -0.5);
	//back  ****************************
	
	//top   ****************************
	glColor3f( 1, 1, 0 ); //Yellow top
	glVertex3f( -0.5, 0.5, 0.5);
	glVertex3f( 0.5, 0.5, 0.5);
	glVertex3f( 0.5, 0.5, -0.5);
	glVertex3f( -0.5, 0.5, -0.5);
	//top   ****************************
	
	
	//bottom****************************
	glColor3f( 0.5, 0, 0.5 ); //PURPLE bottom
	glVertex3f( -0.5, -0.5, 0.5);
	glVertex3f( 0.5, -0.5, 0.5);
	glVertex3f( 0.5, -0.5, -0.5);
	glVertex3f( -0.5, -0.5, -0.5);
	//bottom****************************
	
	//right ****************************
	glColor3f( 0, 1, 1 );  //Fuscia? RIGHT
	glVertex3f( 0.5, -0.5, 0.5);
	glVertex3f( 0.5, -0.5, -0.5);
	glVertex3f( 0.5, 0.5, -0.5);
	glVertex3f( 0.5, 0.5, 0.5);
	//right ****************************
	
	//left  ****************************
	glColor3f( 1, 1, 1 ); //WHITE LEFT
	glVertex3f( -0.5, -0.5, 0.5);
	glVertex3f( -0.5, -0.5, -0.5);
	glVertex3f( -0.5, 0.5, -0.5);
	glVertex3f( -0.5, 0.5, 0.5);
	//left  ****************************
	glEnd();
	
}

//***********************************************************************


void calculateVector()
{
	//takes the amount of user wants to rotate the axis
	//then creates a vector out of the angles
	
	vectorX = cos( angleY*PI/180 ) * cos( angleX*PI/180 );
	
	vectorY =                                                sin( angleX*PI/180 );
	
	vectorZ = sin( angleY*PI/180 ) * cos( angleX*PI/180 );
}

void calculateZedPlane() //obsolete
{
/*
if (angleZ !=0)
{
vectorX *= sin( angleZ*PI/180 );

  vectorY *= cos( -angleZ*PI/180 );
  
	vectorZ *= sin( angleZ*PI/180 );
	
}*/
}

void calculateLookAtPosition()
{
	//the vector is always 1 unit away from the origin
	//so i add it to where ever the camera is located
	
	lookAtX = loc_x + vectorX;
	lookAtY = loc_y + vectorY;
	lookAtZ = loc_z + vectorZ;
	
	
}

void calculateLocation()
{
	//i'm adding a little bit of the vector to my current location
	//the amount is defined in the keyboard cases
	//isn't it great vectors contain direction data:D
	
	loc_x += zoomZ*vectorX;
	loc_y += zoomZ*vectorY;
	loc_z += zoomZ*vectorZ;
	
	
	
	
}


//***********************************************************************
void keyboard(unsigned char key,int x,int y)
{
	
	switch(key)
	{
	case 27: //the ESC key.
		cout << "Exiting application!"<<endl;
		//exit();
		break;
	case 'r':case 'R': //stops rotation and resets the camera
		cout << "Reseting and stopping rotation!" <<endl;
		rot_stop = true;
		rot = 0; //The cube resets its rotation and camera
		zoomZ = 1; //field of view according to z location
		fovyZoom = orig_fov; //field of view according to y to x angle
		
		angleX = 0;
		angleY = -90;
		angleZ = 0;
		
		loc_x = 0; //camera locations
		loc_y = 0;
		loc_z = 5;
		
		lookAtX = 0; //camera is looking at
		lookAtY = 0;
		lookAtZ = 0;
		viewAngleYZ = true;
		
		
		break;
	case 'z':case '-': //zoom into the cube
		cout << "Zooming Out. FOVY = " << fovyZoom <<endl;
		if (fovyZoom != 89)
			fovyZoom++;
		else
			cout << "You can't zoom out anymore without inverting the viewport" <<endl;
		break;
	case 'x':case '+':
		cout << "Zooming in. FOVY = " << fovyZoom <<endl;
		if (fovyZoom != 1)
			fovyZoom--;
		else
		{
			cout << "You can't zoom in anymore without inverting the viewport" <<endl;
			fovyZoom = 1; //making sure the viewport doesn't invert
		}
		break;
		
	case 'h':case 'H': //show instructions
		system("cls"); //clear screen
		printInstructions();
		break;
		
	case 'f':case 'F':      //move fwd
		
		zoomZ = 0.2;
		
		
		calculateVector();
		
		calculateLocation();
		
		calculateLookAtPosition();
		
		
		break;
	case 'b':case 'B':      //move bkwd
		
		zoomZ = -0.2;
		
		calculateVector();
		
		calculateLocation();
		
		calculateLookAtPosition();
		
		break;
		
	case 'a':      //rotate to the left
		
		angleY--;
		
		angleY %= 360;
		
		calculateVector();
		
		calculateLookAtPosition();
		
		
		break;
	case 'd':      //rotate to the right
		angleY++;
		
		angleY %= 360;
		
		calculateVector();
		
		calculateLookAtPosition();
		
		
		
		break;
		
		//up down
	case 'w':      //rotate up
		
		angleX++;
		
		angleX %= 360;
		
		
		calculateVector();
		calculateZedPlane();
		
		calculateLookAtPosition();
		
		
		break;
	case 's':      //rotate down
		
		
		angleX--;
		
		angleX %= 360;
		
		
		
		calculateVector();
		calculateZedPlane();
		calculateLookAtPosition();
		
		
		break;
		
	case '[':               //rotate camera itself to the left
		
		
		angleZ++;
		angleZ %= 360;
		
		break;
	case ']':               //rotate camera itself to the right
		angleZ--;
		angleZ %= 360;
		
		break;

	case 'l': case 'L':
		
		if (islightOn == false)
			islightOn = true;
		else
			islightOn = false;
		
		if (islightOn == true)
		{	
			glEnable(GL_LIGHT1);								// Enable Light One
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHT1);								// Enable Light One
			glDisable(GL_LIGHTING);			
		}

		break;
	default:                //stop pressing the wrong keys. here have the instructions
		cout << "Press 'h' for help. " <<endl;
		debugRawData();
		break;
}


debugRawData();

}

void debugRawData()
{
	//DEBUGGING RAW DATA.
	cout << endl;
	
	cout << "******************************DEBUG************************************"
		<<endl;
	
	
	cout << "angleX: " << angleX << "               angleY: " << angleY << "                angleZ: "
		<< angleZ <<endl;
	
	cout << "vectorX: " << vectorX << "     vectorY: " << vectorY <<
		"       vectorZ: " << vectorZ <<endl;
	
	cout << "loc_X: " << loc_x << "         loc_Y: " << loc_y << "          loc_Z: " <<
		loc_z <<endl;
	
	cout << "lookAtX: " << lookAtX << "     lookAtY: " << lookAtY <<
		"       lookAtZ: " << lookAtZ <<endl;
	
	
	cout <<endl;
	
	
}


void kbArrows(int key, int x, int y)
{
	
	switch(key)
	{
	case GLUT_KEY_LEFT:     //rotate to the left
		
		angleY--;
		
		angleY %= 360;
		
		calculateVector();
		
		calculateLookAtPosition();
		
		
		break;
		
	case GLUT_KEY_RIGHT :      //rotate to the right
		angleY++;
		
		angleY %= 360;
		
		calculateVector();
		
		calculateLookAtPosition();
		
		
		
		break;
		
		//up down
	case GLUT_KEY_UP:      //rotate up
		
		angleX++;
		
		angleX %= 360;
		
		
		calculateVector();
		calculateZedPlane();
		
		calculateLookAtPosition();
		
		
		break;
	case GLUT_KEY_DOWN:      //rotate down
		
		
		angleX--;
		
		angleX %= 360;
		
		
		
		calculateVector();
		calculateZedPlane();
		calculateLookAtPosition();
		
		
		break;
		
	default:                //stop pressing the wrong keys. here have the instructions
		cout << "Press 'h' for help. " <<endl;
		debugRawData();
		break;
	}
	idleRotate();
	objects();
	debugRawData();
	
}

//***********************************************************************



//***********************************************************************

void mouseButton(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) //a mouse button is pressed down!
	{
		if (button == GLUT_LEFT_BUTTON) //check if its the left button,right button don't do anything
		{
			if (viewAngleYZ == true)
			{
				cout << "Changing to 45 degree angle. Camera now located at [0,"
					<< loc_z << "," << loc_z << "]." <<endl;
				
				
				old_loc_x =     loc_x;
				old_loc_y =     loc_y;
				old_loc_z =     loc_z;
				
				
				old_lookAtX = lookAtX;
				old_lookAtY = lookAtY;
				old_lookAtZ = lookAtZ;
				
				old_angleZ = angleZ;
				angleZ = 0;
				
				
				
				
				viewAngleYZ = false; //view on ZY axis
			}
			else
			{
				cout << "Changing to head on view. Camera now located at [0," <<
					loc_z << "," << loc_z << "]."  <<endl;
				
				loc_x = old_loc_x;
				loc_y = old_loc_y;
				loc_z = old_loc_z;
				
				lookAtX = old_lookAtX;
				lookAtY = old_lookAtY;
				lookAtZ = old_lookAtZ;
				
				angleZ = old_angleZ;
				
				viewAngleYZ = true; //view only on Z axis
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			
			//the code below could possibly be compressed into a mathmatical equation with %
			if (rot_stop == true && rot_way == false)
			{
				cout << "Rotating!" <<endl;
				rot_stop = false;
				rot_way = true;
			}
			else if (rot_stop == true && rot_way == true)
			{
				cout << "Rotating!" <<endl;
				rot_stop = false;
				rot_way = false;
			}
			else
			{
				cout << "Rotation stopped!" <<endl;
				rot_stop = true;
			}
		}
	}
	
}

//***********************************************************************


//***********************************************************************
void printInstructions()
{
	cout << "Instructions: " <<endl;
	cout << "              -The escape key exits the application." <<endl;
	cout << "              -The r key resets the cube." <<endl;
	cout << "              -The + key zooms in." <<endl;
	cout << "              -The - key zooms out." <<endl;
	cout << "              -The f key moves the camera forward." <<endl;
	cout << "              -The b key moves the camera backwards." <<endl;
	cout << "              -The w key makes the camera the look up." <<endl;
	cout << "              -The s key makes the camera the look down." <<endl;
	cout << "              -The a key makes the camera look left." <<endl;
	cout << "              -The d key makes the camera look right." <<endl;
	cout << "              -The [ key makes the camera roll left. " <<endl;
	cout << "              -The ] key makes the camera roll right." <<endl;
	cout << "              -The h key displays help." <<endl;
	cout << "              -The left mouse click relocates the camera to [0,5,5]." <<endl;
	cout << "              -The right mouse changes the rotation states. " <<endl;
	cout << endl;
	cout << "Cube color code:    | Axis color code:    |"<<endl;
	cout << "Green  = Front      | Green  = Y axis     |"<<endl;
	cout << "Red    = Back       | Red    = X Axis     |"<<endl;
	cout << "Yellow = Top        | Blue   = Z Axis     |"<<endl;
	cout << "Purple = Bottom     |                     |"<<endl;
	cout << "Aqua   = Right      |                     |"<<endl;
	cout << "White  = Left       |                     |"<<endl;
	cout << "*******************************************************************"
		<<endl;
}

//***********************************************************************

void loadTextures()		//IMPRTANT: LOAD THIS AT BEGINING OF APPLICATION!
{
	
   //glShadeModel(GL_FLAT);
	
//here we're loading the bmps into memory
//the thing is that the library i'm using seems to be written for single textures
//so what i'm doing is using his library to create a pointer to the converted images
//and then using that pointer in my code

	front.LoadBitmap("front.bmp");
	back.LoadBitmap("back.bmp");
	
	rightt.LoadBitmap("right.bmp");
	leftt.LoadBitmap("left.bmp");
	top.LoadBitmap("top.bmp");
	bottom.LoadBitmap("bottom.bmp");
	
//end loading bmps into memory

//store the pointers in my opengl app

	// Generate the texture id
	// i have 6 textures and i'm storing them in an array of 6 reference-able memory spaces
	glGenTextures(6, texturelist);
	


//Load the BACK picture into opengl
	glBindTexture(GL_TEXTURE_2D, texturelist[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, back.imageWidth,
		back.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		back.image);
//end BACK load
	
	//the reason back is in front of front is for debugging purposes

//Load the FRONT picture into opengl
	glBindTexture(GL_TEXTURE_2D, texturelist[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, front.imageWidth,
		front.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		front.image);
//end FRONT load

//Load the RIGHT picture into opengl
	glBindTexture(GL_TEXTURE_2D, texturelist[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, rightt.imageWidth,
		rightt.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		rightt.image);
//end RIGHT load


//Load the LEFT picture into opengl
	glBindTexture(GL_TEXTURE_2D, texturelist[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, leftt.imageWidth,
		leftt.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		leftt.image);
//end LEFT load

//Load the TOP picture into opengl
	glBindTexture(GL_TEXTURE_2D, texturelist[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, top.imageWidth,
		top.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		top.image);
//end TOP load

//Load the BOTTOM picture into opengl
	glBindTexture(GL_TEXTURE_2D, texturelist[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bottom.imageWidth,
		bottom.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		bottom.image);
//end BOTTOM load

  
}

void setupLight()
{	
	
	glPushMatrix();


glEnable(GL_COLOR_MATERIAL);

glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);


	GLfloat LightAmbient[]=		{ 0.2f, 0.2f, 0.4f, 1.0f };
	GLfloat LightDiffuse[]=		{ 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightPosition[]=	{ 0.0f, 20.0f, 6.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light


	

	glPopMatrix();



														
/*

if (viewAngleYZ == true)
{	
	glEnable(GL_LIGHT1);								// Enable Light One
	glEnable(GL_LIGHTING);
}
else
{
	glDisable(GL_LIGHT1);								// Enable Light One
	glDisable(GL_LIGHTING);

}
		;
  */
//	
	
	/*
	glPushMatrix();
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//myinit();
   GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
   GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat no_shininess[] = { 0.0 };
   GLfloat low_shininess[] = { 5.0 };
   GLfloat high_shininess[] = { 100.0 };
   GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};
	
   
   GLfloat ambient[] = { 1, 1, 1, 1.0 };
   GLfloat diffuse[] = { 1.0, 1.0, 1.0, 2.0 };
   GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat position[] = { 0.0, 3.0, 6.0, 0.0 };
   GLfloat lmodel_ambient[] = { 1, 1, 1, 1.0 };
   GLfloat local_view[] = { 0.0 };

   glClearColor(1, 1, 1, 1.0);
  glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);


	glPopMatrix();

	   glPushMatrix();

	   glColor3f(1,1,1);
   glTranslatef (-1.75, 0, 0.0);
   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
   glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
   glutSolidSphere(1.0, 16, 16);
  
   glFlush();
   glPopMatrix();
*/
}

void cubeMaterial()
{

		//materials

		GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
	    GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
	    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	    GLfloat no_shininess[] = { 0.0 };
	    GLfloat low_shininess[] = { 5.0 };
	    GLfloat high_shininess[] = { 100.0 };
	    GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};
	 
   //end materials
   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
   glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

}