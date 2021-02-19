
//Name: Arvin Lin 
//Email: alin11@syr.edu
//Assignment: Assignment 4
//Environment: Visual Studio 2019
//Date: April 9,2020
//References: None

//Interactions: 
//Press: 
//       Ecs: Close the window
//       1: Turn off/on Ceiling Light
//       2: Turn off/on Sconce Light
//       3: Turn off/on FlashLight 
//       a/A: Decrease/Increase Ambient
//       D/d: Close/Open Door
//       F: Flashlight Comes to the person
//       C: Randomizes a House for you (Floats above the house)
//       G: Turn off/on emissive light for the Witch Brew
//       H: Hat Floats
//       r: Resets everything to the original position but the door 
//       up_arrow: Moves Forward (Stops at the middle of the room)
//       down_arrow: Moves Backwards
//       right_arrow: Turns Right
//       left_arrow: Turns Left
//       House Madness: If you keep randomizing the house names and if you get to Gryffindor again where 
//                      it suddenly turns on the ceiling lights then use the left and right arrows to move the moon across the window. 
//                      You would need to do it multiple times until you get the right one. Sometimes it would just turn the ceiling light on. 
//                      But try it! Also if r does not work once then re-click and if you end up not seeing anything after moving with 
//                      the appeared object(house) then just click C again.


#include <iostream>
#include <cmath>
#include <chrono>
#include<sstream>


#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


#define PI 3.14159
using namespace std;

float plcmtRand = 0;
int seed = time(0);

//Global Variables
//I don't use these by you can
static float meX = 0, meY = 0, meZ = -100.0; //location of the monkey
static float angle = 0; //the monkey turning 
static float stepsize = 5.0;  //step size
static float turnsize = 10.0; //degrees to turn

static float t = 0.05; // Quadratic attenuation factor.
float turn = 180;


//Moon Variables
float moonSize = 1.5;
float moonX = 85.0;
float moonY = 45.0;
float moonZ = -48.0;
float moonR = 160.0;
float moonG = 160.0;
float moonB = 160.0;

//Star Variables
float starSize = 0.5;
float starX = 75.0;
float starY = 40.0;
float starZ = -40;
float starR = 255.0;
float starG = 255.0;
float starB = 51.0;

//Wall Variables
float wallX = 50.0;
float xWall = 20.0;
float wallY = 10.0;
float wallZ = -30.0;
float zWall = -70.0;

float wallR = 0.0;
float wallG = 153.0;
float wallB = 153.0;

float zWindowLeft = -60;
float zWindowRight = -40;
float yWindowTop = 40;
float yWindowBottom = 20;

float floorR = 168.0;
float floorG = 119.0;
float floorB = 90.0;

//Door Variables
float doorR = 139.0;
float doorG = 69.0;
float doorB = 19.0;

//Lights Variables
float xFlashlight = 0.0, yFlashlight = 0.0, zFlashlight = -80.0;
float xFLbody = 0.0, yFLbody = 3.0, zFLbody = -40.0;
float xHat = 5.0, yHat = 2.0, zHat = -40.0;

float sconceLoc[] = { 50.0, 10.0, -30.0 };
float lightPos[] = { 50.0, 10.0, -30.0, };
float xLights = -45;
float yLights = -7.2;
float zLights = -32;
float angleL = 180;
float spotAngle[] = { 45.0 };
float spotExp[] = { 3.0 };
float spotDirection[] = { 1.0, 0.0, 0.0 };
float sconceAmb[] = { 0.2, 0.2, 0.2, 1.0 };
float sconceDif[] = { 0.4, 0.4, 0.4, 1.0 };
float sconceSpec[] = { 0.2, 0.2, 0.2, 1.0 };
float lightAmb[] = { 0.7, 0.7, 0.7, 1.0 };
float lightDifAndSpec[] = { 0.5, 0.5, 0.5, 1.0 };

float bulbLoc[] = { 0.0, 50.0, -50.0 };
float xBulb = 0.0;
float yBulb = 50.0;
float zBulb = -50.0;
float radBulb = 1.5;
float bulbR = 255.0;
float bulbG = 255.0;
float bulbB = 255.0;

float bulbAmb[] = { 0.2, 0.2, 0.2, 1.0 };
float bulbDif[] = { 0.8, 0.8, 0.8, 1.0 };
float bulbSpec[] = { 0.2, 0.2, 0.2, 1.0 }; // change how light looks 

float flashR = 128.0;
float flashG = 128.0;
float flashB = 128.0;

float FLpos[] = { 0.0, 0.0, -80.0, 1.0 };
float sizeFLbody = 2.0;

//Brew Variables
float brewR = 76.0, brewG = 0.0, brewB = 153.0;
float xBrew = 10.0, yBrew = 4.0, zBrew = -40.0;
float xRing = -25.0, yRing = 0.0, zRing = -50.0;
float inRad = 0.7, outRad = 2.5;
float angleRing = 90.0;

float xBodyBrew = xRing, yBodyBrew = -1.5, zBodyBrew = zRing;
float brewRad = 3.0;

//Hat Variables

float hatR = 51, hatG = 25, hatB = 0;
float inRadH = 0.7, outRadH = 2.5;
float angleH = 270.0;

//Table Variables
float tableR = 102.0, tableG = 51.0, tableB = 0.0;
float xTable = 0.0, yTable = 0.0, zTable = -40.0;
float xScaleT = 10.0, yScaleT = 10.0, zScaleT = 1.0;
float xLeg = 10.0, yLeg = -5.0, zLeg = -40.0;
float legSize = 2.0;
float angleLeg = 180.0;
float angleTable = 90.0;
float tableSize = 2.0;

//Sword Variables
float base = 2.0, height = 3.0, slices1 = 5.0, stacks1 = 5.0;
float swordR = 192.0, swordG = 192.0, swordB = 192.0;
float handleR = 96.0, handleG = 96.0, handleB = 96.0;
float decorR = 102.0, decorG = 0.0, decorB = 0.0;
float xSwd = 0.0, ySwd = -5.0, zSwd = -60.0;
float xSwdscale = 0.4, ySwdscale = 3.0, zSwdscale = 0.2;
float xDecor = 0.3, yDecor = 0.3, zDecor = 1.0;
float angleSwd = 90.0;
float anglemt = 180.0;
float sizeSwd = 5.0;
float decorRad = 1.0;

//Crown Variables
float xCrown = 0.0, yCrown = 1.0, zCrown = -60;
float crownRad = 0.50, slices2 = 20.0, stacks2 = 20.0;
float crownR = 96.0, crownG = 96.0, crownB = 96.0;
float stoneR = 0.0, stoneG = 0.0, stoneB = 153.0;

//Herb Variables
float xPot = 30.0, yPot = -3.0, zPot = -45.0;
float xpotSca = 2.0, ypotSca = 2.5, zpotSca = 1.0;
float potR = 102, potG = 51.0, potB = 0.0;
float herbR = 0.0, herbG = 102.0, herbB = 0.0;
float xHerb = 1.0, yHerb = 10.0, zHerb = 1.0;
float radHerb = 2.0;
float potSize = 5;

//Rug Variables
float xBody = -30.0, yBody = -7.0, zBody = -55.0;
float xbodySc = 5.0, ybodySc = 2.0, zbodySc = 2.0;
float snakeR = 0.0, snakeG = 255.0, snakeB = 0.0;
float bodyRad = 1.0;
float bodyAngle = 270.0;

//Web Variables
float baseWeb = 5.0, heiWeb = 2.0;
float xWeb = 50.0, yWeb = 50.0, zWeb = -32.0;
float webR = 0.0, webG = 0.0, webB = 0.0;
float angleWeb = 90.0;


//Broom Variables
float xStick = 0.0, yStick = 0.0, zStick = -55.0;
float xstickSc = 1.0, ystickSc = 4, zstickSc = 1.0;
float broomR = 51.0, broomG = 25.0, broomB = 0.0;
float brushR = 153.0, brushG = 76.0, brushB = 0.0;
float brushRad = 2.0;
float brushAn = 90.0;

//Words Variables
float zWords = -40.0;
float xWords = 15.0;
float yWords = 15.0;

float angleF = 90.0;
float angleF1 = 90.0;

float globalEmis[] = { 0.0, 0.0, 0.0, 1.0 };
float globAmbStren = 0.4;



//Booleans
bool door = true;
bool ceilingIsLit = false;
bool sconceIsLit = false;
bool flashlightIsLit = false;
bool emissiveOn = false;


void writeBitMapString(void* font, const char* string)
{
    const char* c;
    for (c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

void drawSky() {
    //Draw Moon 

   //glDisable(GL_LIGHTING);

    float
        moonAmbAndDif[] = { moonR / 255.0, moonG / 255.0, moonB / 255.0, 1.0 },
        moonSpecular[] = { moonR / 255.0, moonG / 255.0, moonB / 255.0, 1.0 },
        moonEmis[] = { moonR / 255.0, moonG / 255.0, moonB / 255.0, 1.0 },
        moonShine[] = { 0.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, moonAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, moonSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, moonEmis);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, moonShine);

    //glColor3ub(moonR, moonG, moonB);

    glPushMatrix();
    glTranslatef(moonX, moonY, moonZ);
    glScalef(5.0, 5.0, 1.0);
    glutSolidSphere(moonSize, 20.0, 20.0);
    glPopMatrix();

    // Draw Stars 
    float
        starAmbAndDif[] = { starR / 255.0, starG / 255.0, starB / 255.0, 1.0 },
        starSpecular[] = { starR / 255.0, starG / 255.0, starB / 255.0, 1.0 },
        starEmis[] = { starR / 255.0, starG / 255.0, starB / 255.0, 1.0 },
        starShine[] = { 0.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, starAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, starSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, starEmis);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, starShine);

    //glColor3ub(starR, starG, starB);

    glPushMatrix();
    glTranslatef(starX, starY, starZ);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(starSize, 20.0, 20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(starX + 10, starY - 5, starZ);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(starSize, 20.0, 20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(starX + 20, starY + 5, starZ);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(starSize, 20.0, 20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(starX + 20, starY + 20, starZ - 10);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(starSize, 20.0, 20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(starX + 10, starY - 5, starZ - 20);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(starSize, 20.0, 20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(starX + 10, starY + 5, starZ - 20);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(starSize, 20.0, 20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(starX + 20, starY - 10, starZ - 10);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(starSize, 20.0, 20.0);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, globalEmis);
    //glutSwapBuffers();

}

void drawWall() {


    // Draw walls
    //glEnable(GL_LIGHTING);
    float
        wallAmbAndDif[] = { wallR / 255.0, wallG / 255.0, wallB / 255.0, 1.0 },
        wallSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
        wallShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, wallAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wallSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, wallShine);

    //Front Wall
    glNormal3f(0.0, 0.0, 1.0);

    glBegin(GL_POLYGON);
    glVertex3f(-wallX, -wallY, zWall);
    glVertex3f(-wallX, wallX, zWall);
    glVertex3f(-xWall, wallX, zWall);
    glVertex3f(-xWall, -wallY, zWall);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(-xWall, xWall, zWall);
    glVertex3f(-xWall, wallX, zWall);
    glVertex3f(xWall, wallX, zWall);
    glVertex3f(xWall, xWall, zWall);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(xWall, -wallY, zWall);
    glVertex3f(xWall, wallX, zWall);
    glVertex3f(wallX, wallX, zWall);
    glVertex3f(wallX, -wallY, zWall);
    glEnd();

    //Back Walls
    glFrontFace(GL_CW);
    glNormal3f(0.0, 0.0, -1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-wallX, -wallY, wallZ);
    glVertex3f(-wallX, wallX, wallZ);
    glVertex3f(wallX, wallX, wallZ);
    glVertex3f(wallX, -wallY, wallZ);
    glEnd();
    glFrontFace(GL_CCW);

    //Right Side Walls
    glNormal3f(-1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-wallX, -wallY, wallZ);
    glVertex3f(-wallX, wallX, wallZ);
    glVertex3f(-wallX, wallX, zWall);
    glVertex3f(-wallX, -wallY, zWall);
    glEnd();

    //Left Side Wall with Window

    //section1 of left side wall
    glFrontFace(GL_CW);
    glNormal3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(wallX, -wallY, zWall); // 1
    glVertex3f(wallX, wallX, zWall); // 2
    glVertex3f(wallX, wallX, zWindowLeft); // 3
    glVertex3f(wallX, -wallY, zWindowLeft); // 4
    glEnd();
    glFrontFace(GL_CCW);
    //section 2
    glBegin(GL_POLYGON);
    glVertex3f(wallX, wallX, zWall); // 3
    glVertex3f(wallX, yWindowTop, zWindowLeft); // 5
    glVertex3f(wallX, yWindowTop, zWindowRight); // 6
    glVertex3f(wallX, wallX, zWindowRight); // 7
    glEnd();
    //section 3
    glBegin(GL_POLYGON);
    glVertex3f(wallX, -wallY, zWindowLeft); // 4
    glVertex3f(wallX, -wallY, zWindowRight); // 8
    glVertex3f(wallX, yWindowBottom, zWindowRight); // 9
    glVertex3f(wallX, yWindowBottom, zWindowLeft); // 10
    glEnd();
    //section 4
    glBegin(GL_POLYGON);
    glVertex3f(wallX, wallX, zWindowRight); // 7
    glVertex3f(wallX, -wallY, zWindowRight); // 8
    glVertex3f(wallX, -wallY, wallZ); // 12
    glVertex3f(wallX, wallX, wallZ); // 11
    glEnd();

    //ceiling
    float
        ceilingAmbAndDif[] = { (wallR + 10) / 255.0, (wallG + 50) / 255.0, (wallB + 50) / 255.0, 1.0 },
        ceilingSpecular[] = { (wallR + 10) / 255.0, (wallG + 50) / 255.0, (wallB + 50) / 255.0, 1.0 },
        ceilingShine[] = { 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ceilingAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ceilingSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, ceilingShine);

    glColor3ub(wallR + 10, wallG + 50, wallB + 50);
    glNormal3f(0.0, -1.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex3f(-wallX, wallX, wallZ);
    glVertex3f(-wallX, wallX, zWall);
    glVertex3f(wallX, wallX, zWall);
    glVertex3f(wallX, wallX, wallZ);
    glEnd();




}

void drawDoor() {

    float
        doorAmbAndDif[] = { doorR / 255.0, doorG / 255.0, doorB / 255.0, 1.0 },
        doorSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        doorShine[] = { 10.0 };


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, doorAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, doorSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, doorShine);


    glPushMatrix();

    glTranslatef(xWall, wallY, -70.0);
    glRotatef(turn, 0, 1, 0);
    glTranslatef(xWall, -wallY, 70.0);

    glBegin(GL_POLYGON);
    glVertex3f(-xWall, -wallY, zWall);
    glVertex3f(-xWall, xWall, zWall);
    glVertex3f(xWall, xWall, zWall);
    glVertex3f(xWall, -wallY, zWall);
    glEnd();
    glPopMatrix();
    
}

void drawFloor() {
    // Draw Floor
    float
        floorAmbAndDif[] = { floorR / 255.0, floorG / 255.0, floorB / 255.0, 1.0 },
        floorSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        floorShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floorSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, floorShine);

    glColor3ub(floorR, floorG, floorB);
    glNormal3f(0.0, 1.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex3f(-wallX, -wallY, wallZ);
    glVertex3f(-wallX, -wallY, zWall);
    glVertex3f(wallX, -wallY, zWall);
    glVertex3f(wallX, -wallY, wallZ);
    glEnd();
}

//House Objects 
void drawLights() {
    float
        sconceAmbAndDif[] = { 0.0 / 255.0, .0 / 255.0, 150.0 / 255.0, 1.0 },
        sconceSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        sconceShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sconceAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sconceSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, sconceShine);



    glDisable(GL_LIGHTING);



        glLightfv(GL_LIGHT2, GL_AMBIENT, sconceAmb);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, sconceDif);
        glLightfv(GL_LIGHT2, GL_SPECULAR, sconceSpec);

        //SpotLight
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, spotAngle);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
        glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, spotExp);
        glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec);
        glLightfv(GL_LIGHT1, GL_POSITION, lightDifAndSpec);


        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, t);
   
        // Draw sconce
        glPushMatrix();
        glLightfv(GL_LIGHT2, GL_POSITION, sconceLoc);
        glTranslatef(xLights, yLights, zLights);
        glRotatef(angleL, 0, 1, 0);
        glutWireCone(3, 1, 10, 10);
        glPopMatrix();

        glEnable(GL_LIGHTING);
        
        

        //Draw Ceiling light bulb 


        float
            bulbAmbAndDif[] = { bulbR / 255.0, bulbG / 255.0, bulbB / 255.0, 1.0 },
            bulbSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
            bulbShine[] = { 10.0 };

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, bulbAmbAndDif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bulbSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, bulbShine);

        glDisable(GL_LIGHTING);
        // Turn on OpenGL lighting.



        glLightfv(GL_LIGHT5, GL_AMBIENT, bulbAmb);
        glLightfv(GL_LIGHT5, GL_DIFFUSE, bulbDif);
        glLightfv(GL_LIGHT5, GL_SPECULAR, bulbSpec);



        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, t);


        glColor3ub(bulbR, bulbG, bulbB);


        glPushMatrix();
        glLightfv(GL_LIGHT0, GL_POSITION, bulbLoc);
        glTranslatef(xBulb, yBulb, zBulb);
        glutSolidSphere(radBulb, 20.0, 20.0);
        glPopMatrix();

        glEnable(GL_LIGHTING);

        //Draw flashlight


        float
            flashAmbAndDif[] = { flashR / 255.0, flashG / 255.0, flashB / 255.0, 1.0 },
            flashSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
            flashShine[] = { 10.0 };

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, flashAmbAndDif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, flashSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, flashShine);

        glColor3ub(flashR, flashG, flashB);

        glDisable(GL_LIGHTING);



       glLightfv(GL_LIGHT4, GL_POSITION, FLpos);
        glLightfv(GL_LIGHT4, GL_SPOT_CUTOFF, spotAngle);

        glLightfv(GL_LIGHT4, GL_SPOT_EXPONENT, spotExp);
        glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmb);
        glLightfv(GL_LIGHT4, GL_DIFFUSE, lightDifAndSpec);
        glLightfv(GL_LIGHT4, GL_POSITION, lightDifAndSpec);
        glEnable(GL_LIGHT4);
        glPushMatrix();



        //Flashlight Body
        glPushMatrix();
        glTranslatef(xFLbody - 3, yFLbody, zFLbody);
        glRotatef(angleF1, 1, 0, 0);
        glScalef(-2.0, 1.0, 1.0);
        glutSolidCube(sizeFLbody);
        glPopMatrix();

        glPushMatrix();
        glLightfv(GL_LIGHT4, GL_POSITION, FLpos);
        glTranslatef(xFLbody, yFLbody, zFLbody);
        glRotatef(angleF, 0, -1, 0);
        glutSolidCone(2.0, 3.0, 10.0, 10.0);
        glPopMatrix();

        glPopMatrix();
        glEnable(GL_LIGHTING);
}

void draWitchBrew() {


    float
        brewAmbAndDif[] = { brewR / 255.0, brewG / 255.0, brewB / 255.0, 1.0 },
        brewSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        brewEmis[] = { brewR / 255.0, brewG / 255.0, brewB / 255.0, 1.0 },
        brewShine[] = { 10.0 };

    if (emissiveOn) {

        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, brewEmis);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brewAmbAndDif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, brewSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, brewShine);
    }

    glPushMatrix();

    //Ring of the Brew
    glPushMatrix();
    glTranslatef(xRing, yRing, zRing);
    glRotatef(angleRing, 1, 0, 0);
    glutSolidTorus(inRad, outRad, 20.0, 20.0);
    glPopMatrix();

    //Body of the Brew
    glPushMatrix();
    glTranslatef(xBodyBrew, yBodyBrew, zBodyBrew);
    glutSolidSphere(brewRad, 20.0, 20.0);
    glPopMatrix();
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, globalEmis);

}
void drawHat() {
   


    float
        hatAmbAndDif[] = { hatR / 255.0, hatG / 255.0, hatB / 255.0, 1.0 },
        hatSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        hatEmis[] = { hatR / 255.0, hatG / 255.0, hatB / 255.0, 1.0 },
        hatShine[] = { 5.0 };


    //Material of Hat
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, hatAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, hatSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, hatShine);
    //Cone Part of the Hat
    glPushMatrix();
    glTranslatef(xHat, yHat, zHat);
    glPushMatrix();

    glRotatef(angleH, 1, 0, 0);
    glutSolidCone(2.5, 8.0, 10.0, 10.0);
    glPopMatrix();
    //Rim Part 
    glPushMatrix();

    glRotatef(angleH, 1, 0, 0);
    glutSolidTorus(inRadH, outRadH, 20.0, 20.0);
    glPopMatrix();

    glPopMatrix();
}
void drawTable() {


    float
        tableAmbAndDif[] = { tableR / 255.0, tableG / 255.0, tableB / 255.0, 1.0 },
        tableSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        tableShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tableAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tableSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, tableShine);

    //Draw Surface 
    glColor3ub(tableR, tableG, tableB);
    glPushMatrix();
    glTranslatef(xTable, yTable, zTable);
    glRotatef(angleTable, 1, 0, 0);
    glScalef(xScaleT, yScaleT, zScaleT);
    glutSolidCube(tableSize);
    glPopMatrix();

    //Draw Legs
    glPushMatrix();
    glTranslatef(xLeg, yLeg, zLeg);
    glScalef(xScaleT - 9, yScaleT - 6, zScaleT);
    glRotatef(angleLeg, 0.0, -1.0, 0);
    glutSolidCube(legSize);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xLeg, yLeg, -35.0);
    glScalef(xScaleT - 9, yScaleT - 6, zScaleT);
    glRotatef(angleLeg, 0.0, -1.0, 0);
    glutSolidCube(legSize);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-xLeg, yLeg, -35.0);
    glScalef(xScaleT - 9, yScaleT - 6, zScaleT);
    glRotatef(angleLeg, 0.0, -1.0, 0);
    glutSolidCube(legSize);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-xLeg, yLeg, zLeg);
    glScalef(xScaleT - 9, yScaleT - 6, zScaleT);
    glRotatef(angleLeg, 0.0, -1.0, 0);
    glutSolidCube(legSize);
    glPopMatrix();


}

//Gryffindor
void drawSword() { 

    float
        metalAmbAndDif[] = { swordR / 255.0, swordG / 255.0, swordB / 255.0, 1.0 },
        metalSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        metalShine[] = { 5.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, metalAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, metalSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, metalShine);

   
    //tip
    glPushMatrix();
    glTranslatef(xSwd, ySwd, zSwd);
    glRotatef(angleSwd, 1, 0, 0);
    glutSolidCone(base, height, slices1, stacks1);
    glPopMatrix();
    
    //metal part
    
    glPushMatrix();
    glTranslatef(xSwd, ySwd+7, zSwd);
    ///rotate
    glRotatef(angleSwd+90, 0, -1, 0);
    glScalef(xSwdscale+0.3, ySwdscale-0.3, zSwdscale);
    glutSolidCube(sizeSwd);
    glPopMatrix();


    float
        handleAmbAndDif[] = { handleR / 255.0, handleG / 255.0, handleB / 255.0, 1.0 },
        handleSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        handleShine[] = { 5.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, handleAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, handleSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, handleShine);

  
     //handling part
    glPushMatrix();
    glTranslatef(xSwd, ySwd+10, zSwd-3);
    glRotatef(angleSwd, 1, 0, 0);
    glScalef(xSwdscale+0.6, ySwdscale-2.9, zSwdscale+0.3);
    glutSolidCube(sizeSwd);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xSwd, ySwd + 12, zSwd - 3);
    ///rotate
    glRotatef(angleSwd + 90, 0, -1, 0);
    glScalef(xSwdscale + 0.2, ySwdscale -2.5, zSwdscale);
    glutSolidCube(sizeSwd);
    glPopMatrix();


    float
        decorAmbAndDif[] = { decorR / 255.0, decorG / 255.0, decorB / 255.0, 1.0 },
        decorSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        decorShine[] = { 5.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, decorAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, decorSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, decorShine);

  

    glPushMatrix();
    glTranslatef(xSwd, ySwd + 11, zSwd - 3);
    glScalef(xDecor, yDecor, zDecor);
    glutSolidSphere(decorRad, 30.0, 30.0);
    glPushMatrix();


}

void drawCrown() {

    float
        crownAmbAndDif[] = { crownR / 255.0, crownG / 255.0, crownB / 255.0, 1.0 },
        crownSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        crownShine[] = { 5.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, crownAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, crownSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, crownShine);

    glBegin(GL_POLYGON);
    glVertex3f(2.5, 0.0, -60.0);
    glVertex3f(2.5, 2.5, -60.0);
    glVertex3f(1.0, 1.0, -60.0);
    glVertex3f(0.0, 2.5, -60.0);
    glVertex3f(-1.0, 1.0, -60.0);
    glVertex3f(-2.5, 2.5, -60.0);
    glVertex3f(-2.5, 0.0, -60.0);
    glEnd();

    float
        stoneAmbAndDif[] = { stoneR / 255.0, stoneG / 255.0, stoneB / 255.0, 1.0 },
        stoneSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        stoneShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, stoneAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, stoneSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, stoneShine);

    glPushMatrix();
    glTranslatef(xCrown, yCrown, zCrown);
    glutSolidSphere(crownRad, slices2, stacks2);
    glPopMatrix();
    //glEnable(GL_LIGHTING);
}

void drawHerb() {

    float
        potAmbAndDif[] = { potR / 255.0, potG / 255.0, potB / 255.0, 1.0 },
        potSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        potShine[] = { 5.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, potAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, potSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, potShine);


    //Pot
    glPushMatrix();
    glTranslatef(xPot, yPot-2, zPot);
    glScalef(xpotSca, ypotSca, zpotSca);
    glutSolidCube(potSize);
    glPopMatrix();

    float
        herbAmbAndDif[] = { herbR / 255.0, herbG / 255.0, herbB / 255.0, 1.0 },
        herbSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        herbShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, herbAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, herbSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, herbShine);

    //Herb
    glPushMatrix();
    glTranslatef(xPot, yPot+5, zPot);
    glScalef(xHerb, yHerb-2, zHerb);
    glutSolidSphere(radHerb, 20.0, 20.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(xPot-3, yPot + 13, zPot);
    glScalef(xHerb+1.0, yHerb-9.0, zHerb);
    glutSolidSphere(radHerb, 20.0, 20.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(xPot + 3, yPot + 10, zPot);
    glScalef(xHerb + 1.0, yHerb - 9.0, zHerb);
    glutSolidSphere(radHerb, 20.0, 20.0);
    glPopMatrix();


}

void drawRug() {


    float
        snakeAmbAndDif[] = { snakeR / 255.0, snakeG / 255.0, snakeB / 255.0, 1.0 },
        snakeSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        snakeShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, snakeAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, snakeSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, snakeShine);

    
    glPushMatrix();
    glTranslatef(xBody+30.0, yBody-1, zBody-5);
    glScalef(xbodySc, ybodySc-1, zbodySc-1);
    glutSolidSphere(bodyRad, 20.0, 20.0);
    glPopMatrix();

}

void drawWeb()
{


    float
        webAmbAndDif[] = { webR / 255.0, webG / 255.0, webB / 255.0, 1.0 },
        webSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        webShine[] = { 3.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, webAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, webSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, webShine);
    glPushMatrix();
    glTranslatef(xWeb, yWeb, zWeb);
    glRotatef(angleWeb, 1, 0, 0);
    glutWireCone(baseWeb, heiWeb, 10.0, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-xWeb, yWeb, zWeb);
    glRotatef(angleWeb, 1, 0, 0);
    glutWireCone(baseWeb, heiWeb, 10.0, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-xWeb+5, yWeb-20, zWeb-25);
    glRotatef(angleWeb, -1, 0, 0);
    glutWireCone(baseWeb, heiWeb, 10.0, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(xWeb-5, yWeb-10, zWeb -30);
    glRotatef(angleWeb, -1, 0, 0);
    glutWireCone(baseWeb, heiWeb, 10.0, 10.0);
    glPopMatrix();

}

void drawBroom()
{


    float
        broomAmbAndDif[] = { broomR / 255.0, broomG / 255.0, broomB / 255.0, 1.0 },
        broomSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        broomShine[] = { 5.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, broomAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, broomSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, broomShine);
    //stick
    glPushMatrix();
    glTranslatef(xStick, yStick, zStick);
    glScalef(xstickSc, ystickSc, zstickSc);
    glutSolidCube(2);
    glPopMatrix();

    float
        brushAmbAndDif[] = { brushR / 255.0, brushG/ 255.0, brushB / 255.0, 1.0 },
        brushSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        brushShine[] = { 5.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brushAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, brushSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, brushShine);

    //brush
    glPushMatrix();
    glTranslatef(xStick, yStick-5, zStick);
    glScalef(xstickSc, ystickSc-2.5, zstickSc);
    glutSolidSphere(brushRad, 20.0, 20.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(xStick, yStick-7, zStick);
    glRotatef(brushAn, 1, 0, 0);
    glutSolidCone(1.5, 2.0, 10.0, 10.0);
    glPopMatrix();

}



// Drawing routine.
void drawScene(void)
{
    float  globAmb[] = { globAmbStren, globAmbStren, globAmbStren, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(135.0, 1.0, 10.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //gluLookAt goes here
    gluLookAt(meX, meY, meZ, meX + sin(angle * PI / 180), meY, meZ + cos(angle * PI / 180), 0, 1, 0);

    glColor4f(0.0, 0.0, 0.0, 1.0); 


    if (ceilingIsLit) {
        glEnable(GL_LIGHT0); glEnable(GL_LIGHT5);
    }
    else { glEnable(GL_LIGHT0); glDisable(GL_LIGHT5); }
    
    if (sconceIsLit) {
        glEnable(GL_LIGHT1); glEnable(GL_LIGHT2);
    }
    else { glDisable(GL_LIGHT1); glDisable(GL_LIGHT2); }

    if (flashlightIsLit) {
        glEnable(GL_LIGHT4);
    }
    else { glDisable(GL_LIGHT4); }

    drawSky();
    drawWall();
    drawFloor();
    drawDoor();
    drawLights();
    draWitchBrew();
    drawHat();
    drawTable();
    drawWeb();
    
 
    glDisable(GL_LIGHTING);
   
    if (plcmtRand == 1) {
        glDisable(GL_LIGHTING);
        glColor3ub(255.0, 255.0, 0.0); //Yellow
        glRasterPos3f(xWords, yWords, zWords);
        writeBitMapString(GLUT_BITMAP_TIMES_ROMAN_24, "HufflePuff");
        glEnable(GL_LIGHTING);
        drawSky();
        drawWall();
        drawFloor();
        drawDoor();
        drawLights();
        draWitchBrew();
        drawHat();
        drawTable();
        drawWeb();
        drawHerb();
    }

    else if (plcmtRand == 2) {
        glDisable(GL_LIGHTING);
        glColor3ub(0.0, 0.0, 255.0); //Blue
        glRasterPos3f(xWords, yWords, zWords);
        writeBitMapString(GLUT_BITMAP_TIMES_ROMAN_24, "Ravenclaw");
        glEnable(GL_LIGHTING);
        drawSky();
        drawWall();
        drawFloor();
        drawDoor();
        drawLights();
        draWitchBrew();
        drawHat();
        drawTable();
        drawWeb();
        drawCrown();

    }
   
    //glRasterPos3f(15.0, 15.0, -40.0);
    else if (plcmtRand == 3) {
        glDisable(GL_LIGHTING);
        glColor3ub(255.0, 0.0, 0.0); //Red
        glRasterPos3f(xWords, yWords, zWords);
        writeBitMapString(GLUT_BITMAP_TIMES_ROMAN_24, "Gryffindor");
        glEnable(GL_LIGHTING);
        drawSky();
        drawWall();
        drawFloor();
        drawDoor();
        drawLights();
        draWitchBrew();
        drawHat();
        drawTable();
        drawWeb();
        drawSword();

        
    }
   
    //glRasterPos3f(15.0, 15.0, -40.0);
    else if (plcmtRand == 4) {
        glDisable(GL_LIGHTING);
        glColor3ub(0.0, 255.0, 0.0); //Green
        glRasterPos3f(xWords, yWords, zWords);
        writeBitMapString(GLUT_BITMAP_TIMES_ROMAN_24, "Slytherin");
        glEnable(GL_LIGHTING);
        drawSky();
        drawWall();
        drawFloor();
        drawDoor();
        drawLights();
        draWitchBrew();
        drawHat();
        drawTable();
        drawWeb();
        drawRug();
        drawBroom();

    }
    else {
        drawSky();
        drawWall();
        drawFloor();
        drawDoor();
        drawLights();
        draWitchBrew();
        drawHat();
        drawTable();
        drawWeb();
    }
    
    glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
    glClearColor(0.10, 0.169, 0.222, 0.0); // night sky
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    
}


// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(135.0, 1.0, 10.0, 90.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'A':
        if (globAmbStren <= 1.0) {
            globAmbStren = globAmbStren + 0.1;
        }
        glutPostRedisplay();
        break;
    case 'a':
        if (globAmbStren > 0.2 ) {
            globAmbStren = globAmbStren - 0.1;
        }
        glutPostRedisplay();
        break;
    case 'D':
        if (turn < 180) turn = turn + 5;;
        glutPostRedisplay();
        break;
    case 'd':
        if (turn > 90) turn = turn - 5;;
        glutPostRedisplay();
        break;
    case 'F':
        zFLbody = (meZ+5) + stepsize * cos(angle * PI / 180);
        xFLbody = (meX+5)+stepsize * sin(angle * PI / 180);

        glutPostRedisplay();
        break;

    case 'C':

        srand(seed++);
        plcmtRand = (rand() % 4) + 1;

        glutPostRedisplay();
        break;
    case 'G':
        emissiveOn = !emissiveOn;
        glutPostRedisplay();
        break;
    case 'H':
        yHat = 5.0;
        glutPostRedisplay();
        break;

    case '1':
        ceilingIsLit = !ceilingIsLit;
        glutPostRedisplay();
        break;
    case '2':

        sconceIsLit = !sconceIsLit;
        glutPostRedisplay();
        break;
    case '3':
        flashlightIsLit = !flashlightIsLit;
        glutPostRedisplay();
        break;
    case 'r':
        xFLbody = 0.0;
        yFLbody = 3.0;
        zFLbody = -40.0;
        yHat = 2.0;
        ceilingIsLit = false;
        sconceIsLit = false;
        flashlightIsLit = false;
        plcmtRand = 0;
        emissiveOn = false;
        globAmbStren = 0.4;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void specialKeyInput(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP: //forward
        if (meZ + stepsize * cos(angle * PI / 180) <= -65){
        meZ += stepsize * cos(angle * PI / 180); //z
        meX += stepsize * sin(angle * PI / 180); //x
       }
        break;
    case GLUT_KEY_DOWN: //back
        if (meZ + stepsize * cos(angle * PI / 180) >= -100) {
            meZ -= stepsize * cos(angle * PI / 180); //z
            meX -= stepsize * sin(angle * PI / 180); //x
        }
        break;
    case GLUT_KEY_RIGHT: //turn right
        angle += turnsize;
        angleF = angle + turnsize;
        angleF1 = angle + turnsize;

        break;
    case GLUT_KEY_LEFT: //turn left
        angle -= turnsize;
        angleF = angle - turnsize;
        angleF1 = angle - turnsize;
        break;

    }//end switch
    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "\t Press:" << endl;
    cout << "\t\t Ecs: Close the window" << endl;
    cout << "\t\t 1: Turn off/on Ceiling Light" << endl;
    cout << "\t\t 2: Turn off/on Sconce Light" << endl;
    cout << "\t\t 3: Turn off/on FlashLight " << endl;
    cout << "\t\t a/A: Decrease/Increase Ambient" << endl;
    cout << "\t\t D/d: Close/Open Door" << endl;
    cout << "\t\t F: Flashlight Comes to the person" << endl;
    cout << "\t\t C: Randomizes a House for you (Floats above the house)" << endl;
    cout << "\t\t G: Turn off/on emissive light for the Witch Brew" << endl;
    cout << "\t\t H: Hat Floats" << endl;
    cout << "\t\t r: Resets everything to the original position but the door " << endl;
    cout << "\t\t up_arrow: Moves Forward (Stops at the middle of the room)" << endl;
    cout << "\t\t down_arrow: Moves Backwards" << endl;
    cout << "\t\t right_arrow: Turns Right" << endl;
    cout << "\t\t left_arrow: Turns Left" << endl;
    cout << "\t\t House Madness : If you keep randomizing the house namesand if you get to Gryffindor again where" << endl;
    cout << "\t\t\t\t it suddenly turns on the ceiling lights then use the left and right arrows" << endl;
    cout << "\t\t\t\t to move the moon across the window." << endl;
    cout << "\t\t\t\t You would need to do it multiple times until you get the right one. Sometimes it would" << endl;
    cout << "\t\t\t\t just turn the ceiling light on." << endl;
    cout << "\t\t\t\t But try it! Also if r does not work once then re-click and " << endl;
    cout << "\t\t\t\t if you end up not seeing anything after moving with the appeared object" << endl;
    cout << "\t\t\t\t (house) then just click C again. " << endl;
}


// Main routine.
int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(750, 750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("House of Madness");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glutMainLoop();

    return 0;
}