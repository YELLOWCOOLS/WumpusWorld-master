#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>

#define UP 1
#define DOWN 2
#define LEFT 3 
#define RIGHT 4
GLfloat ctrlpoints[4][3] = 
{
        { -8.0, -8.0, 0.0}, { -8.0,8.0 , 0.0}, 
        {4.0, -8.0, 0.0}, {8.0, 8.0, 0.0}
};

// Status Variables
GLint   moves = 0;
GLbyte  gameOver = false;
GLbyte  EnableLight = true; 
GLbyte won=false;
GLbyte start=true;
GLbyte rules=false;


// Player Variables
GLint   _x       = 0;
GLint   _z       = 0;
GLbyte  direction  = 0;

// Screen variables
GLint   _w       = 800;
GLint   _h       = 550;
GLint   _iw     = 0;
GLint   _ih     = 0;
GLint   _fw     = 160;
GLint   _fh     = 160;

//Variables for the Camera Angle
static GLfloat view_rotx=45.0F ;
static GLfloat view_roty=0.0F ;
static GLfloat view_rotz=0.0F ;

static GLfloat headRotation=90.0F ;
static GLfloat zoom=-300.0F ;

//Configure the lightning
void initLight()
{
    //Add ambient light
    GLfloat ambientColor[] = {0.3f, 0.4f, 0.8f, 1.0f}; 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; 
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    //Add directed light
    GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f}; 
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}

//initialize the first configurations
void Initialize(void) 
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7f, 0.9f, 1.0f, 1.0f); //Change the background to sky blue
    if(EnableLight){
        glEnable(GL_COLOR_MATERIAL); //Enable color
        glEnable(GL_LIGHTING); //Enable lighting
        glEnable(GL_LIGHT0); //Enable light #0
        glEnable(GL_LIGHT1); //Enable light #1
        glEnable(GL_NORMALIZE); //Automatically normalize normals
    }
}

//Configure window resize
void Resize (int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (double)w / (double)h, 1, 800.0);
}

void Write(char *string)
{
    while(*string) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}
void BigWrite(char *string)
{
    while(*string) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
}
//This Function will rotate the object according to the Angles
void ManipulateViewAngle() 
{
    glRotatef(view_rotx,1.0,0.0,0.0);//Rotate Arround X axis
    glRotatef(view_roty,0.0,1.0,0.0);//Rotate Arround Y axis
    glRotatef(view_rotz,0.0,0.0,1.0);//Rotate Arround Z axis
}
//This Function will reset the Player Position and location...
void Reset(){
    _x       = 0;
    _z       = 0;
    direction  = 0;
    moves=0;
    gameOver = false;
    view_rotx=45.0F ;
    view_roty=0.0F ;
    view_rotz=0.0F ;
    headRotation=90.0F ;
    zoom=-300.0F ;
	won=false;

}

//Display a welcome screen
void WelcomeScreen()
{

    glTranslatef (-60.0, 40.0, zoom);
	 glColor3f(0.30,0.30,1.00);
    glRasterPos2f(-20, 50);
    BigWrite("WUMPUS WORLD");
    glColor3f(0.36,0.25,0.20);
    glRasterPos2f(0, 0);
	 Write("Select 'r' to read rules of the game.");
	 glRasterPos2f(0, -20);
     Write("Select 'p' to play");
	 glRasterPos2f(0, -40);
     Write("Select 'Esc' button to exit at any time.");
	glColor3f(0.62352,0.372549,0.623529);
	 glRasterPos2f(60, -80);
	  Write("Done By :");
	 glRasterPos2f(90, -100);
    Write("GREESHMA R");
	 glRasterPos2f(90, -115);
    Write("MEGHNA NATRAJ");
	 glRasterPos2f(90, -130);
    Write("VARSHA CHANDAN");


    
}
int CheckGrid()
{
	 int ret=1;
	 if(_z==0) 
	 {
		  if(_x==0) ret=1;
		  else if(_x==40) ret=2;
		  else if(_x==80) ret=3;
          else ret=4;
	 }
	 else if(_z==40) 
	 {
		  if(_x==0) ret=5;
		  else if(_x==40) ret=6;
		  else if(_x==80) ret=7;
          else ret=8;
	 }
	 else if(_z==80) 
	 {
		  if(_x==0) ret=9;
		  else if(_x==40) ret=10;
		  else if(_x==80) ret=11;
          else ret=12;
	 }
	 else 
	 {
		  if(_x==0) ret=13;
		  else if(_x==40) ret=14;
		  else if(_x==80) ret=15;
          else ret=16;
	 }
	 return ret;

}


void Finish()
{
    char tmp_str[40]; 
	int pos;
	
   

   glColor3f(0.30,0.30,1.00);
    glRasterPos2f(20,60);
	BigWrite("GAME OVER");

    glColor3f(0.329412,0.329412,0.329412);
    glRasterPos2f(-60,20);
    pos=CheckGrid();
    
    if(won==true)
	{
          glRasterPos2f(-10,20);
		  BigWrite("You WON!! Congratulations.");
	}
	else if(pos==3||pos==11) BigWrite("You LOST!! Try not falling into the PIT again !!");
	else BigWrite("You LOST!! You were eaten up by the evil WUMPUS! ");
    
    glRasterPos2f(20, -10);
    sprintf(tmp_str, "Moves : %d ",moves);
	BigWrite(tmp_str);

    glColor3f(0.90,0.90,0.90);
    glRasterPos2f(-30, -50);
    BigWrite("To start playing again please press 's'.");

}
void Rules()
{
	
	glTranslatef (-60.0, 40.0, zoom);
    glColor3f(0.30,0.30,1.00);
    glRasterPos2f(0, 70);
    BigWrite("RULES OF THE GAME");

    glColor3f(0.329412,0.329412,0.329412);
    glRasterPos2f(-120, 40);
    Write("1.The WUMPUS WORLD is a cave consisting of rooms connected by passageways.");
    glRasterPos2f(-120, 20);
    Write("2.Lurking somewhere in the cave is WUMPUS who eats anyone who enters its room.");
	glRasterPos2f(-110, 5);
    Write("If there is a WUMPUS in the adjacent room , the player senses STENCH (Yellow).");
	glRasterPos2f(-120, -20);
    Write("3.Some rooms contain bottomless PITS which will trap anyone who wanders into it.");
    glRasterPos2f(-110, -35);
    Write("If there is a PIT in the adjacent room , the player senses BREEZE (Blue).");
    glRasterPos2f(40, -70);
	BigWrite("GOAL");
	  glRasterPos2f(-120, -90);
	Write("The player must find his/her way to find the precious HEAP OF GOLD hidden in a room.");
    	glColor3f(0.62352,0.372549,0.623529);
	glRasterPos2f(20, -120);
    Write("PRESS 'p' TO PLAY.");

}
void DrawItems()
{
	float angle;
	int pos=CheckGrid();
	glTranslatef(-20, 0, -20); //TO SHIFT THE ENTIRE BOARD POSITION
	 glColor3f(1.0f,0.0f, 0.0f);
     ManipulateViewAngle();
     //PLANE
     glPushMatrix();
        glTranslatef(80, -16.0, 80);
        glScalef(165,5,165);
		glColor3f(0.2f, 0.3f, 0.2f);
        glutSolidCube(1);
      glPopMatrix();

	  glPushMatrix();
     //LINES ON GRID
    glColor3f(0,0,0);
    glLineWidth(3);
    glTranslatef(0,-12.5,0);//translate before drawing lines
   //vertical
   glBegin(GL_LINES);
	   glVertex3f(40,0,0);
	   glVertex3f(40,0,160);
     glEnd();
	    glBegin(GL_LINES);
	   glVertex3f(80,0,0);
	   glVertex3f(80,0,160);
     glEnd();
	    glBegin(GL_LINES);
	   glVertex3f(120,0,0);
	   glVertex3f(120,0,160);
     glEnd();
	 //horizontal
	    glBegin(GL_LINES);
	   glVertex3f(0,0,40);
	   glVertex3f(160,0,40);
     glEnd();
     glBegin(GL_LINES);
	    glVertex3f(0,0,80);
	    glVertex3f(160,0,80);
     glEnd();
	    glBegin(GL_LINES);
	   glVertex3f(0,0,120);
	   glVertex3f(160,0,120);
     glEnd();
	 glPopMatrix();
   
	glPushMatrix();
   glColor3f(0,0,0);
   glLineWidth(3);
   glTranslatef(0,-12.5,0);//translat

	 //PLAYER
   if(pos!=13)
   {
	glTranslatef(20,12.5,20);//translate back after drawing lines
    glColor3f(1.0,1.0,0.0);//Color it Yellow
    glTranslatef(_x,-10.0,_z);//Give it the location according to _x & _z
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(10,20,20);//Draw the head as a sphere a litle bit bigger than the body spheres
    glRotatef(headRotation, 0.0, 1.0, 0.0);
    glColor3f(1.0,1.0,0.0);
    glTranslatef(0,0.0,6.0);
    glScalef(0.8,1.0,1.0);
    //glutSolidCone(6,10,20,20);
    glColor3f(0,0,0);
    glTranslatef(-4.0,10.0,0.0);
    glScalef(0.3,0.3,0.3);
    glutSolidSphere(10,20,20);
    glTranslatef(26.0,0.0,0.0);
    glutSolidSphere(10,20,20);
 
   }
    glPopMatrix();
    glPushMatrix();
 
    if(pos==9)
	{
      //WUMPUS
	  gameOver=true;
      glColor3f(1,0,0);
	  glTranslatef(20,-5,105);//Give it the location according to _x & _z
	  glScalef(1,1,1);
      glutSolidSphere(10,20,20);//Draw the head as a sphere a litle bit bigger than the body spheres
      glRotatef(headRotation, 0.0, 1.0, 0.0);
      //Horns
      glColor3f(0,0,0);
      glTranslatef(-4.0,8.0,-2.0);
      glScalef(0.3,0.3,0.3);
      glRotatef(-90, 1, 0, 0);
      glutSolidCone(10,30,20,20);
      glRotatef(90, 1, 0, 0);
      glTranslatef(26.0,0.0,0.0);
      glRotatef(-90, 1, 0, 0);
      glutSolidCone(10,30,20,20);
	  glRotatef(90, 1, 0, 0);
	  //Eyes
	  glColor3f(1,1,1);
      glTranslatef(-26.0,-5,27);
      glutSolidSphere(10,20,20);
      glTranslatef(26.0,0,0);
      glutSolidSphere(10,20,20);
      glRotatef(-headRotation, 0.0, 1.0, 0.0);
      glPopMatrix();
	 }
     else if(pos==13)
	 {
       //GOLD
	   gameOver=true;
	   won=true;
	   glPushMatrix();
       glTranslatef(20,-5,145);
	   glColor3f(0.85f,0.85f,0.10f);
	   glutSolidCube(13);
	   glPopMatrix();
	   glPointSize(5.0);
		
	   glBegin(GL_POINTS);
	    glVertex3f(15,20,160);
		glVertex3f(15,30,150);

		glVertex3f(20,30,152);
		glVertex3f(20,20,159);
	    glVertex3f(20,30,162);
		glVertex3f(20,20,163);

		glVertex3f(25,20,148);
		glVertex3f(25,30,154);
		glVertex3f(25,30,162);
		
		
		glVertex3f(30,30,148);
        glVertex3f(30,20,147);
	     glVertex3f(30,30,158);

		glVertex3f(35,20,151);
		glVertex3f(35,30,152);
		
	   glEnd();
	 }
     else if(pos==11)
	 {
	   //PIT
	   //pit at 80,80
		gameOver=true;
	    glTranslatef(100,-5,105);
        glColor3f(0.35f,0.16f,0.04f);
        glBegin(GL_QUAD_STRIP);
        for(int i=0;i<=600;i=i+50)
		{
			 angle=(2*3.14*i)/600;
			 glVertex3f(0.1*cos(angle),0,0.1*sin(angle));
			 glVertex3f(15*cos(angle),0,15*sin(angle));			 
		}
		glEnd();
	 }

      else if(pos==3)
	 {    
     //pit at 80,0
	  gameOver=true;
	  glTranslatef(100,-5,105);
      glTranslatef(0,0,-78);
      glColor3f(0.35f,0.16f,0.04f);
      glBegin(GL_QUAD_STRIP);
      for(int i=0;i<=600;i=i+50)
		{
			 angle=(2*3.14*i)/600;
			 glVertex3f(0.1*cos(angle),0,0.1*sin(angle));
			 glVertex3f(15*cos(angle),0,15*sin(angle));			 
		}
      glEnd();
	 }
     if(pos==2||pos==4||pos==7||pos==12||pos==10||pos==15)
	 {
	   int i;
	   if(pos==2)glTranslatef(60,0,20);
	   else if(pos==4)glTranslatef(140,0,20);
	   else if(pos==7)glTranslatef(100,0,60);
	   else  if(pos==12)glTranslatef(140,0,100);
	   else  if(pos==10)glTranslatef(60,0,100);
	   else  glTranslatef(100,0,140);
       
	    glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++) glEvalCoord1f((GLfloat) i/30.0);
        glEnd();

        glTranslatef(0,5,0);
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++) glEvalCoord1f((GLfloat) i/30.0);
        glEnd();
		if(pos==2)glTranslatef(-60,-5,-20);
	   else if(pos==4)glTranslatef(-140,-5,-20);
	   else if(pos==7)glTranslatef(-100,-5,-60);
	   else  if(pos==12)glTranslatef(-140,-5,-100);
	   else  if(pos==10)glTranslatef(-60,-5,-100);
	   else  glTranslatef(-100,-5,-140);

	 }
	 if(pos==5||pos==10)
	 {
 
		 int i;
		 if(pos==5)glTranslatef(20,5,60);
		 else glTranslatef(60,5,100);

		 glRotatef(50,0,0,1);
		 glColor3f(1.0, 1.0, 0.0);
         glBegin(GL_LINE_STRIP);
         for (i = 0; i <= 30; i++) glEvalCoord1f((GLfloat) i/30.0);
         glEnd();

        glTranslatef(10,0,0);
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++) glEvalCoord1f((GLfloat) i/30.0);
        glEnd();

        glTranslatef(10,10,10);
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++) glEvalCoord1f((GLfloat) i/30.0);
        glEnd();
		
	 }
     glPopMatrix();
}

void GameStatus()
{

    char tmp_str[40];
	glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-10,-8);
    Write("START");

    //Print the status of the game on the screen
    glColor3f(0.0, 0.0, 1.0);
	glRasterPos2f(40,40);
	sprintf(tmp_str, "Moves : %d ",moves);
    BigWrite(tmp_str);
 

}
//This Function will move the Player according to the directions taken from the Keyboard keys
void Run(int value)
{
	 moves+=1;
     switch(direction)
	 {
      case RIGHT:  headRotation =90;
                   _x += 40;    
                    if(_x > _fw-1) _x = _iw+120;
		            break;
      case LEFT :  headRotation =-90;
                   _x -= 40;    
                   if(_x < 0) _x = _fw-160;
                    break;
      case DOWN:    headRotation =0;
                  _z += 40;    
                  if(_z > _fh-1) _z = _ih+120;
                  break;
      case UP:  headRotation =180;
                  _z -= 40;    
                  if(_z < 0) _z = _fh-160;
                  break;
    }

}



void Display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor (1.0,0.8,1.0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(EnableLight) initLight();
	if(start==true) WelcomeScreen();
	else if(rules==true) Rules();
    else if(!gameOver)
	{
        glTranslatef (-60.0, 40.0, zoom);
		GameStatus();
        DrawItems();
    }
	else if(start==false)
	{
		    glTranslatef (-60.0, 40.0, zoom);
		    _sleep(2000);
			Finish();
	  } 	
        

     // Updates the screen
    glutPostRedisplay();
    glutSwapBuffers();
}

void Special(int key, int x, int y)
{
	if(!gameOver)
	{

      switch(key)
	   {
            case GLUT_KEY_RIGHT     :direction = RIGHT;Run(0);break;
            case GLUT_KEY_LEFT      :direction = LEFT;Run(0);break;
            case GLUT_KEY_UP        :direction = UP;Run(0);break;
            case GLUT_KEY_DOWN      :direction = DOWN;Run(0);break;
    }
	}
}
//This is for special viewing
void keyboard (unsigned char key, int x, int y)
{
    switch (key) 
	{
      //All Rotation Over X, Y, Z angles
      case 'x'    : view_rotx +=2 ;glutPostRedisplay();break;
      case 'X'    : view_rotx -=2 ;glutPostRedisplay();break;
      case 'y'    : view_roty +=2 ;glutPostRedisplay();break;
      case 'Y'    : view_roty -=2 ;glutPostRedisplay(); break;
      case 'z'    : view_rotz +=2 ;glutPostRedisplay();break;
      case 'Z'    : view_rotz -=2 ;glutPostRedisplay();break;
      case '+'    : zoom++ ;glutPostRedisplay();break;
      case '-'    : zoom-- ;glutPostRedisplay();break;
      case 's'    : Reset() ;glutPostRedisplay();break;
	  case 'p'    : Reset() ;start=false;rules=false;glutPostRedisplay();break;
	  case 'r'    : start=false;rules=true;glutPostRedisplay();break;
      case 27     : exit(0);break;
      default     : break;
    }
}

int main(void)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(_w,_h);
    glutInitWindowPosition(80,80);
    glutCreateWindow("Wumpus World");

    glShadeModel(GL_FLAT);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);

    glutSpecialFunc(Special);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(Display);
    glutReshapeFunc(Resize);
    
    Initialize();
    glutMainLoop();
}