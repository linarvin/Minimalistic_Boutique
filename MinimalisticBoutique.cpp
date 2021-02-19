//Name: Arvin Lin 
//Email: alin11@syr.edu
//Assignment: Final Project
//Environment: Visual Studio 2019
//Date: May 1,2020
//References: None

//Interactions: 
//Press: 
//       Ecs: Close the window
//       +/-: Increase/Decrease on the Y-axis
//       0: Turn on Ceiling Light
//       9: Turn off Ceiling Light
//       1: Pattern on the Skirt is Koch, watch how the leaves turn on the tree if its grown for you
//       2: Pattern on the Skirt is KachiVariant, watch how the leaves turn on the tree if its grown for you
//       D/d: Put the Skirt on Avatar/Mannequin 
//       F/f: Put Pants on the Avatar/Mannequin
//       S/s: Put Shirt on the Avatar/Mannequin
//       W/w, E/e Q/q: Adjust clothing (W: Shirt, E: Pants, Q: Dress) by moving up and down on Avatar/ Mannequin
//       K/k: Increase/decrease the Fractals of the Koch or KachiVariant, watch how the leaves turn on the tree if its grown for you
//       O/o: Open or Close Door
//       T/t: Grow/De-Grow the Tree
//       r: Resets everything to the original position but the door 
//       up_arrow: Moves Forward (Stops at the middle of the room)
//       down_arrow: Moves Backwards
//       right_arrow: Turns Right
//       left_arrow: Turns Left
//       Right-Click on Track Pad: A menu show up and you can change the Skin Tone or Quit


#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <fstream>
#include<sstream>


#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


#define PI 3.14159
#define KOCH 0
#define KOCHVARIANT 1
#define TREE 2

#define SHADE1 3
#define SHADE2 4
#define SHADE3 5



#define ROOT3 1.73205081
#define ONEBYROOT3 0.57735027

#define RATIO 0.85 // Growth ratio = length of tree sub-branch to length of branch.
#define ANGLE 40 // Angle between the two tree sub-branches.


using namespace std;



//Global Variables

//Variables for the Pattern and Tree
static int maxLevel = 0; // Recursion level.
static int maxLevelT = 0;
static int shape = KOCH; // Shape index.
static int Tshape = TREE;

class Source; // Make source class visible.

//Variables for gluQUadrics 
static GLUquadricObj* qobj; // Create a pointer to a quadric object.


//Variables for Texture 
static unsigned int texture[5]; // Array of texture indices.
float rowsAndCol = 1.0;
float numOftextures = 5.0;

//Variables to walk around
static float meX = 0, meY = 10, meZ = -100.0; 
static float angle = 0; 
static float UpAndDown = 5.0;
static float stepsize = 5.0;  //step size
static float turnsize = 5.0; //degrees to turn

//Light Variables
float globAmbStren = 0.4;
float  globAmb[] = { globAmbStren, globAmbStren, globAmbStren, 1.0 };
static float t = 0.05; // Quadratic attenuation factor.
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

bool ceilingIsLit = false;

//Location of Texture 
float xSky = 200.0, ySky = 250.0, ysky = -10.0, zSky = -20.0, ZSky = -100.0;



//Wall Variables 

float zWall = -70.0, Zwall = -30.0, xWall = -35.0, yWall = -10.0, mWall = 15.0, Ywall = 60.0;

//Door Variables
float turn = 180;
float mdoor = mWall, xdoor = yWall, ydoor = yWall, zdoor = zWall;

//Clothes Variables 

float bottomRad = 5.75, waistRad = 2.0, longc = 10.0;
float xDress = -15.0, yDress = -5.5, zDress = -50.0;
float topRad = 3.75, shirtEndrad = topRad, longS = 3.0, xShirt = 15.0, yShirt = 8.0, zShirt = -50.0;
float sleevesRtop = 1.5, sleevesREnd = sleevesRtop, longeves = 2.0, xSleeves = 11.0, ySleeves = 7.5, zSleeves = -50.0;
float xPant = 15.0, yPant = 0.0, zPant = -50.0, pantRad = 2.5, chestRad = 2.5, radBase = 1.5;



//Variables for the Avatar

float xAvatar = 0.0, yAvatar = 0.0, zAvatar = -50.0, AvatarRad = 2.5, chestRad3 = 2.5, AvatarBase = 1.5;
float baseRadA = 1.0, topRadA = baseRadA, heightA = 7.0;
float xlegAvatar = -2.2, ylegAvatar = 0.0, zlegAvatar = zAvatar, legBaseRad = 2.0, legHei = 10.0;
float xAvatarArm = -4.0, yAvatarArm = 7.5, zAvatarArm = zAvatar, armBaseRad = 1.0, armHei = 7.0;
float xHeadA = xAvatar, yHeadA = 13.0, zHeadA = zAvatar, headRadA = 3.0;
float headR = 141.0, headG = 85.0, headB = 36.0;
float skinR = headR, skinG = headG, skinB = headB;

//Variables for Menu
static int id1;
int SkinToneMenu;
int Texture;


// Sequel class.
class Sequel
{
public:
    Sequel() { coords.clear(); v.clear(); }
    void drawKochOrVariant(); // Routine to draw Sequel object in case it represents 
                              // a Koch snowflake or variant Koch snowflake.
    void drawTree(); // Routine to draw Sequel object in case it represents a tree.

    friend class Source;

private:
    vector<float> coords; // Vector of x, y co-ordinates of points specifying sequel object.
    vector<Source> v; // Vector of associated source objects to be produced recursively.
};

// Routine to draw Sequel object in case it represents a Koch snowflake or variant Koch snowflake.
void Sequel::drawKochOrVariant()
{

    glPushMatrix();
    glTranslatef(xDress, yDress+3, zDress-5);
    glScalef(0.03, 0.03, 1.0);

   glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 5; i++)
        glVertex2f(coords[2 * i], coords[2 * i + 1]);
    glEnd();
    glPopMatrix();
}

// Routine to draw Sequel object in case it represents a tree.
void Sequel::drawTree()
{


    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 3; i++)
        glVertex2f(coords[2 * i], coords[2 * i + 1]);
    glEnd();


}

// Source class.
class Source
{
public:
    Source() { }
    Source(float coordsVal[4])
    {
        for (int i = 0; i < 4; i++) coords[i] = coordsVal[i];
    }

    void draw(); // Routine to draw source line segment.

    Sequel sourceToSequelKoch(); // Routine to generate Sequel object in case of Koch snowflake.
    void produceKoch(int level); // Recursive routine to produce Koch snowflake.

    Sequel sourceToSequelKochVariant(); // Routine to generate Sequel object in case of variant Koch snowflake.
    void produceKochVariant(int level); // Recursive routine to produce variant Koch snowflake.

    Sequel sourceToSequelTree(); // Routine to generate Sequel object in case of tree.
    void produceTree(int level); // Recursive routine to produce tree.

    friend class Sequel;

private:
    float coords[4]; // x, y co-ordinates of a line segment endpoints.
};

// Routine to draw source line segment.
void Source::draw()
{

    glBegin(GL_LINES);
    for (int i = 0; i < 2; i++)
        glVertex2f(coords[2 * i], coords[2 * i + 1]);
    glEnd();
 
}

// Routine to generate Sequel object in case of Koch snowflake.
Sequel Source::sourceToSequelKoch()
{
    float x0, y0, x1, y1, coordsVal[10], coordsVal1[4];
    int i, j;
    Source s;
    Sequel seq = *new Sequel();

    x0 = coords[0]; y0 = coords[1]; x1 = coords[2]; y1 = coords[3];

    // From the co-ordinates of the two segment endpoints calculate the co-ordinates of 
    // the 5 vertices of the Koch polyline.
    coordsVal[0] = x0;
    coordsVal[1] = y0;
    coordsVal[2] = 0.66666667 * x0 + 0.33333333 * x1;
    coordsVal[3] = 0.66666667 * y0 + 0.33333333 * y1;
    coordsVal[4] = 0.5 * (x0 + x1) - 0.5 * ONEBYROOT3 * (y1 - y0);
    coordsVal[5] = 0.5 * (y0 + y1) + 0.5 * ONEBYROOT3 * (x1 - x0);
    coordsVal[6] = 0.33333333 * x0 + 0.66666667 * x1;
    coordsVal[7] = 0.33333333 * y0 + 0.66666667 * y1;
    coordsVal[8] = x1;
    coordsVal[9] = y1;

    // Enter Koch polyline vertices into sequel object.
    for (i = 0; i < 10; i++) seq.coords.push_back(coordsVal[i]);

    // Specify all 4 segments of the Koch polyline for recursive production.
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            coordsVal1[j] = coordsVal[2 * i + j];
        }
        s = *new Source(coordsVal1);
        seq.v.push_back(s);
    }

    return seq;
}

// Recursive routine to produce Koch snowflake.
void Source::produceKoch(int level)
{
    glColor3ub(0.0, 0.0, 0.0);

    // Only sequels at given level are drawn.
    if (maxLevel == 0) this->draw();
    else if (maxLevel == 1) this->sourceToSequelKoch().drawKochOrVariant();
    else if (level < maxLevel - 1)
        for (int i = 0; i < 4; i++) this->sourceToSequelKoch().v[i].produceKoch(level + 1);
    else this->sourceToSequelKoch().drawKochOrVariant();
}

// Routine to generate Sequel object in case of variant Koch snowflake.
Sequel Source::sourceToSequelKochVariant()
{
    float x0, y0, x1, y1, coordsVal[10], coordsVal1[4];
    int i;
    Source s;
    Sequel seq = *new Sequel();

    x0 = coords[0]; y0 = coords[1]; x1 = coords[2]; y1 = coords[3];

    // From the co-ordinates of the two segment endpoints calculate the co-ordinates of 
    // the 5 vertices of the Koch polyline.
    coordsVal[0] = x0;
    coordsVal[1] = y0;
    coordsVal[2] = 0.66666667 * x0 + 0.33333333 * x1;
    coordsVal[3] = 0.66666667 * y0 + 0.33333333 * y1;
    coordsVal[4] = 0.5 * (x0 + x1) - 0.5 * ONEBYROOT3 * (y1 - y0);
    coordsVal[5] = 0.5 * (y0 + y1) + 0.5 * ONEBYROOT3 * (x1 - x0);
    coordsVal[6] = 0.33333333 * x0 + 0.66666667 * x1;
    coordsVal[7] = 0.33333333 * y0 + 0.66666667 * y1;
    coordsVal[8] = x1;
    coordsVal[9] = y1;

    // Enter Koch polyline vertices into sequel object.
    for (i = 0; i < 10; i++) seq.coords.push_back(coordsVal[i]);

    // Specify the segment joining the first vertex of the Koch polyline to the middle vertex
    // and the segment joining the middle vertex to the last vertex for recursive production.
    for (i = 0; i < 2; i++)
    {
        coordsVal1[0] = coordsVal[4 * i];
        coordsVal1[1] = coordsVal[4 * i + 1];
        coordsVal1[2] = coordsVal[4 * i + 4];
        coordsVal1[3] = coordsVal[4 * i + 5];
        s = *new Source(coordsVal1);
        seq.v.push_back(s);
    }

    return seq;
}

// Recursive routine to produce variant Koch snowflake.
void Source::produceKochVariant(int level)
{
    glColor3ub(0.0, 0.0, 0.0);

    // Only sequels at given level are drawn.
    if (maxLevel == 0) this->draw();
    else if (maxLevel == 1) this->sourceToSequelKochVariant().drawKochOrVariant();
    else if (level < maxLevel - 1)
        for (int i = 0; i < 2; i++) this->sourceToSequelKochVariant().v[i].produceKochVariant(level + 1);
    else this->sourceToSequelKochVariant().drawKochOrVariant();
}

// Routine to generate Sequel object in case of tree.
Sequel Source::sourceToSequelTree()
{
    float x0, y0, x1, y1, coordsVal[10], coordsVal1[4];
    int i, j;
    Source s;
    Sequel seq = *new Sequel();


    x0 = coords[0]; y0 = coords[1]; x1 = coords[2]; y1 = coords[3];
    

    // From the co-ordinates of the two segment endpoints calculate the co-ordinates of
    // the 3 vertices of the sub-branch V-shape.
    coordsVal[0] = x1 + RATIO * cos((PI / 180.0) * ANGLE / 2.0) * (x1 - x0) - RATIO * sin((PI / 180.0) * ANGLE / 2.0) * (y1 - y0);
    coordsVal[1] = y1 + RATIO * cos((PI / 180.0) * ANGLE / 2.0) * (y1 - y0) + RATIO * sin((PI / 180.0) * ANGLE / 2.0) * (x1 - x0);
    coordsVal[2] = x1;
    coordsVal[3] = y1;
    coordsVal[4] = x1 + RATIO * cos((PI / 180.0) * ANGLE / 2.0) * (x1 - x0) + RATIO * sin((PI / 180.0) * ANGLE / 2.0) * (y1 - y0);
    coordsVal[5] = y1 + RATIO * cos((PI / 180.0) * ANGLE / 2.0) * (y1 - y0) - RATIO * sin((PI / 180.0) * ANGLE / 2.0) * (x1 - x0);

   
    // Enter V-shape vertices into sequel object.

    for (i = 0; i < 6; i++) seq.coords.push_back(coordsVal[i]);

    // Specify both sub-branch segments of the V-shape for recursive production.
    for (i = 0; i < 2; i++)
    {
        coordsVal1[0] = coordsVal[2];
        coordsVal1[1] = coordsVal[3];
        for (j = 2; j < 4; j++)
        {
            coordsVal1[j] = coordsVal[4 * i + j - 2];
        }
        s = *new Source(coordsVal1);
        seq.v.push_back(s);
    }

    return seq;
}

// Routine to draw leaf called by the following routine to produce tree.
void drawLeaf(float x, float y)
{
    glColor3ub(0.0, 255.0, 0.0);
    

    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glRotatef(rand() % 360, 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 2.0);
    glVertex2f(0.0, 4.0);
    glVertex2f(-1.0, 2.0);
    glEnd();
    glPopMatrix();

}

// Recursive routine to produce tree.
void Source::produceTree(int level)
{
    glColor3ub(0.4*255, 0.5*255, 0.5*255);

    // Branches are thinner up the tree.
    glLineWidth(2 * (maxLevelT - level));

    // Source and sequels at all prior levels are drawn (different from Kock and Koch variant).
    if (maxLevelT == 0) this->draw();
    else if (maxLevelT == 1) { this->draw(); this->sourceToSequelTree().drawTree(); }
    else if (level < maxLevelT)
    {
        if (level == 0) this->draw();
        this->sourceToSequelTree().drawTree();
        for (int i = 0; i < 2; i++) this->sourceToSequelTree().v[i].produceTree(level + 1);
    }

    // Embellish with leaves.
    if (level == maxLevelT - 1)
    {
        drawLeaf(this->sourceToSequelTree().coords[0], this->sourceToSequelTree().coords[1]);
        drawLeaf(this->sourceToSequelTree().coords[4], this->sourceToSequelTree().coords[5]);
    }

    // Restore line width.
    glLineWidth(1.0);
}


// Struct of bitmap file.
struct BitMapFile
{
    int sizeX;
    int sizeY;
    unsigned char* data;
};

void writeBitMapString(void* font, const char* string)
{
    const char* c;
    for (c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile* getBMPData(string filename)
{
    BitMapFile* bmp = new BitMapFile;
    unsigned int size, offset, headerSize;

    // Read input file name.
    ifstream infile(filename.c_str(), ios::binary);

    // Get the starting point of the image data.
    infile.seekg(10);
    infile.read((char*)&offset, 4);

    // Get the header size of the bitmap.
    infile.read((char*)&headerSize, 4);

    // Get width and height values in the bitmap header.
    infile.seekg(18);
    infile.read((char*)&bmp->sizeX, 4);
    infile.read((char*)&bmp->sizeY, 4);

    // Allocate buffer for the image.
    size = bmp->sizeX * bmp->sizeY * 24;
    bmp->data = new unsigned char[size];

    // Read bitmap data.
    infile.seekg(offset);
    infile.read((char*)bmp->data, size);

    // Reverse color from bgr to rgb.
    int temp;
    for (int i = 0; i < size; i += 3)
    {
        temp = bmp->data[i];
        bmp->data[i] = bmp->data[i + 2];
        bmp->data[i + 2] = temp;
    }

    return bmp;
}

// Load external textures.
void loadExternalTextures()
{
    // Local storage for bmp image data.
    BitMapFile* image[5];

    // Load the textures.
    image[0] = getBMPData("Textures/brick.bmp");
    image[1] = getBMPData("Textures/sky.bmp");
    image[2] = getBMPData("Textures/grass.bmp");
    image[3] = getBMPData("Textures/hardWood.bmp");
    image[4] = getBMPData("Textures/wallpaper.bmp");
 


    // Bind Brick image to texture index[0]. 
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
    // Bind Sky image to Texture index[1]
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);

    // Bind Grass image to texture index[2]
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2]->sizeX, image[2]->sizeY, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image[2]->data);

    // Bind Hardwood image to texture index[3]
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[3]->sizeX, image[3]->sizeY, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image[3]->data);
    // Bind Wallpaper image to texture index[4]
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[4]->sizeX, image[4]->sizeY, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image[4]->data);  
}

//Draw Ceiling light bulb 
void drawLights() {


    float
        bulbAmbAndDif[] = { bulbR / 255.0, bulbG / 255.0, bulbB / 255.0, 1.0 },
        bulbSpecular[] = { 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0 },
        bulbShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, bulbAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bulbSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, bulbShine);

    glDisable(GL_LIGHTING);
    
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

    if (ceilingIsLit) {
        glEnable(GL_LIGHT0); glEnable(GL_LIGHT5);
    }
    else { glEnable(GL_LIGHT0); glDisable(GL_LIGHT5); }

    glEnable(GL_LIGHTING);

    //Draw flashlight
}
void fractalPat() {
   
    float coordsVal1[4] = { -30.0, -15.0, 0.0, -15.0 + ROOT3 * 0.5 * 60.0 };
    float coordsVal2[4] = { 0.0, -15.0 + ROOT3 * 0.5 * 60.0, 30.0, -15.0 };
    float coordsVal3[4] = { 30.0, -15.0, -30.0, -15.0 };
    float coordsVal4[4] = { 0.0, -30.0, 0.0, -15.0 };



    Source src1 = *new Source(coordsVal1); // Edge of an equilateral triangle.
    Source src2 = *new Source(coordsVal2); // Edge of an equilateral triangle.
    Source src3 = *new Source(coordsVal3); // Edge of an equilateral triangle.
    Source src4 = *new Source(coordsVal4); // Vertical line segment.

    


    if (shape == KOCH)
        // Produce on all three edges of an equilateral triangle.
    {
        src1.produceKoch(0);
        src2.produceKoch(0);
        src3.produceKoch(0);
    }

    if (shape == KOCHVARIANT)
        // Produce on all three edges of an equilateral triangle.
    {
        src1.produceKochVariant(0);
        src2.produceKochVariant(0);
        src3.produceKochVariant(0);
    }

    if (Tshape == TREE)
    {
        // Produce on a vertical line segment.	  
        glPushMatrix();
        glTranslatef(60.0, 20.0, -60.0);
        glRotatef(90.0, 0, 1, 0);
        src4.produceTree(0);
        glPopMatrix();
    }

    glFlush();
}

void drawSky() {
    glEnable(GL_TEXTURE_2D);
    

    glBindTexture(GL_TEXTURE_2D, texture[1]);

    //Front
    glNormal3f(0.0, 0.0, -1.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(-xSky, ySky+1, ZSky);
    glTexCoord2f(rowsAndCol, 0.0); glVertex3f(xSky, ySky+1, ZSky);
    glTexCoord2f(rowsAndCol, rowsAndCol); glVertex3f(xSky, ysky-1, ZSky);
    glTexCoord2f(0.0, rowsAndCol); glVertex3f(-xSky, ysky-1, ZSky);
    glEnd();

    //Back 

    glNormal3f(0.0, 0.0, -1.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(-xSky, ySky+1, zSky);
    glTexCoord2f(rowsAndCol, 0.0); glVertex3f(xSky, ySky+1, zSky);
    glTexCoord2f(rowsAndCol, rowsAndCol); glVertex3f(xSky, ysky-1, zSky);
    glTexCoord2f(0.0, rowsAndCol); glVertex3f(-xSky, ysky-1, zSky);
    glEnd();

    //Left Side 
    glNormal3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(-xSky+100, ySky+1, zSky);
    glTexCoord2f(rowsAndCol, 0.0); glVertex3f(-xSky+100, ySky+1, ZSky);
    glTexCoord2f(rowsAndCol, rowsAndCol); glVertex3f(-xSky+100, ysky-1, ZSky);
    glTexCoord2f(0.0, rowsAndCol); glVertex3f(-xSky+100, ysky-1, zSky);
    glEnd();

    //Right Side
    glNormal3f(-1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(xSky-100, ySky+1, zSky);
    glTexCoord2f(rowsAndCol, 0.0); glVertex3f(xSky-100, ySky+1, ZSky);
    glTexCoord2f(rowsAndCol, rowsAndCol); glVertex3f(xSky-100, ysky-1, ZSky);
    glTexCoord2f(0.0, rowsAndCol); glVertex3f(xSky-100, ysky-1, zSky);
    glEnd();

    //Top Side 
    glNormal3f(0.0, -1.0, 0.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(-xSky, ySky+1, zSky);
    glTexCoord2f(rowsAndCol, 0.0); glVertex3f(xSky, ySky+1, zSky);
    glTexCoord2f(rowsAndCol, rowsAndCol); glVertex3f(xSky, ySky+1, ZSky);
    glTexCoord2f(0.0, rowsAndCol); glVertex3f(-xSky, ySky+1, ZSky);
    glEnd();


    glDisable(GL_TEXTURE_2D);
}
void drawGround() {
    //Top Side 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(-xSky, ysky-2, zSky);
    glTexCoord2f(rowsAndCol+7, 0.0); glVertex3f(xSky, ysky-1, zSky);
    glTexCoord2f(rowsAndCol+7, rowsAndCol+7); glVertex3f(xSky, ysky-1, ZSky);
    glTexCoord2f(0.0, rowsAndCol+7); glVertex3f(-xSky, ysky-1, ZSky);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

//Draw Studio
void drawStudio() {

    glEnable(GL_TEXTURE_2D);


    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // Exterior Walls

    // Front Wall 
    glNormal3f(0.0, 1.0, 0.0);

    //Front-Left Section 
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(xWall, yWall, zWall);
    glTexCoord2f(rowsAndCol, 0.0); glVertex3f(xWall, mWall, zWall);
    glTexCoord2f(rowsAndCol, rowsAndCol); glVertex3f(yWall, mWall, zWall);
    glTexCoord2f(0.0, rowsAndCol); glVertex3f(yWall, yWall, zWall);
    glEnd();

    //Front-Right Section 
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(-yWall, yWall, zWall);
    glTexCoord2f(rowsAndCol, 0.0); glVertex3f(-yWall, mWall, zWall);
    glTexCoord2f(rowsAndCol, rowsAndCol); glVertex3f(-xWall, mWall, zWall);
    glTexCoord2f(0.0, rowsAndCol); glVertex3f(-xWall, yWall, zWall);
    glEnd();

    //Front-Top Section 
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(xWall, mWall, zWall);
    glTexCoord2f(rowsAndCol + 2, 0.0); glVertex3f(xWall, Ywall, zWall);
    glTexCoord2f(rowsAndCol + 2, rowsAndCol + 2); glVertex3f(-xWall, Ywall, zWall);
    glTexCoord2f(0.0, rowsAndCol + 2); glVertex3f(-xWall, mWall, zWall);
    glEnd();
    glDisable(GL_CULL_FACE);


    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // Back Wall
    glNormal3f(0.0, 0.0, -1.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(xWall, yWall, Zwall);
    glTexCoord2f(rowsAndCol + 2, 0.0); glVertex3f(xWall, Ywall, Zwall);
    glTexCoord2f(rowsAndCol + 2, rowsAndCol + 2); glVertex3f(-xWall, Ywall, Zwall);
    glTexCoord2f(0.0, rowsAndCol + 2); glVertex3f(-xWall, yWall, Zwall);
    glEnd();
    

    //Right Wall

    glNormal3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(xWall, yWall, zWall);
    glTexCoord2f(rowsAndCol + 2, 0.0); glVertex3f(xWall, Ywall, zWall);
    glTexCoord2f(rowsAndCol + 2, rowsAndCol + 2); glVertex3f(xWall, Ywall, Zwall);
    glTexCoord2f(0.0, rowsAndCol + 2); glVertex3f(xWall, yWall, Zwall);
    glEnd();
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    float ceilingR = 245.0, ceilingG = 245.0, ceilingB = 220.0;
    glColor3ub(ceilingR, ceilingG, ceilingB);
    float
         ceilingAmbAndDif[] = { ceilingR / 255.0, ceilingG / 255.0, ceilingB / 255.0, 1.0 },
         ceilingSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
         ceilingShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ceilingAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ceilingSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, ceilingShine);   
    //Ceiling
    glNormal3f(0.0, -1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(xWall, Ywall, zWall);
    glVertex3f(-xWall, Ywall, zWall);
    glVertex3f(-xWall, Ywall, Zwall);
    glVertex3f(xWall, Ywall, Zwall);
    glEnd();
    

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(-xSky, ysky - 2, zSky);
    glTexCoord2f(rowsAndCol + 7, 0.0); glVertex3f(xSky, ysky - 1, zSky);
    glTexCoord2f(rowsAndCol + 7, rowsAndCol + 7); glVertex3f(xSky, ysky - 1, ZSky);
    glTexCoord2f(0.0, rowsAndCol + 7); glVertex3f(-xSky, ysky - 1, ZSky);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    


    
    glEnable(GL_TEXTURE_2D);
    //Floor
    glBindTexture(GL_TEXTURE_2D, texture[3]);

    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(xWall, yWall, zWall);
    glTexCoord2f(rowsAndCol+7, 0.0); glVertex3f(-xWall, yWall, zWall);
    glTexCoord2f(rowsAndCol+7, rowsAndCol+7); glVertex3f(-xWall, yWall, Zwall);
    glTexCoord2f(0.0, rowsAndCol+7); glVertex3f(xWall, yWall, Zwall);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
   
    float doorR = 211.0, doorG = 211.0, doorB = 211.0;
    float
         doorAmbAndDif[] = { doorR / 255.0, doorG / 255.0, doorB / 255.0, 1.0 },
         doorSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
         doorShine[] = { 10.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, doorAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, doorSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, doorShine);

     //Door
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();

    glBegin(GL_POLYGON);
    glVertex3f(xdoor, ydoor, zdoor);
    glVertex3f(xdoor, mdoor, zdoor);
    glVertex3f(-xdoor, mdoor, zdoor);
    glVertex3f(-xdoor, ydoor, zdoor);
    glEnd();
     
    glPopMatrix();



    // Interior Walls 

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);


    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glNormal3f(0.0, 1.0, 0.0);
    //Front-Left Section 
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(xWall, yWall, zWall);
    glTexCoord2f(rowsAndCol, 0.0); glVertex3f(xWall, mWall, zWall);
    glTexCoord2f(rowsAndCol, rowsAndCol); glVertex3f(yWall, mWall, zWall);
    glTexCoord2f(0.0, rowsAndCol); glVertex3f(yWall, yWall, zWall);
    glEnd();

    //Front-Right Section 
    glBegin(GL_POLYGON);

    glTexCoord2f(0.0, 0.0); glVertex3f(-yWall, yWall, zWall);
    glTexCoord2f(rowsAndCol, 0.0); glVertex3f(-yWall, mWall, zWall);
    glTexCoord2f(rowsAndCol, rowsAndCol); glVertex3f(-xWall, mWall, zWall);
    glTexCoord2f(0.0, rowsAndCol); glVertex3f(-xWall, yWall, zWall);
    glEnd();
    //Front-Top Section 
    glBegin(GL_POLYGON);

    glTexCoord2f(0.0, 0.0); glVertex3f(xWall, mWall, zWall);
    glTexCoord2f(rowsAndCol + 2, 0.0); glVertex3f(xWall, Ywall, zWall);
    glTexCoord2f(rowsAndCol + 2, rowsAndCol + 2); glVertex3f(-xWall, Ywall, zWall);
    glTexCoord2f(0.0, rowsAndCol + 2); glVertex3f(-xWall, mWall, zWall);
    glEnd();
    glDisable(GL_CULL_FACE);


    glCullFace(GL_FRONT);
    
    glBindTexture(GL_TEXTURE_2D, texture[4]);

    //Back Wall 
    glNormal3f(0.0, 0.0, -1.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(xWall, yWall, Zwall);
    glTexCoord2f(rowsAndCol + 2, 0.0); glVertex3f(xWall, Ywall, Zwall);
    glTexCoord2f(rowsAndCol + 2, rowsAndCol + 2); glVertex3f(-xWall, Ywall, Zwall);
    glTexCoord2f(0.0, rowsAndCol + 2); glVertex3f(-xWall, yWall, Zwall);
    glEnd();


    //Right Wall
    glNormal3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(xWall, yWall, zWall);
    glTexCoord2f(rowsAndCol + 2, 0.0); glVertex3f(xWall, Ywall, zWall);
    glTexCoord2f(rowsAndCol + 2, rowsAndCol + 2); glVertex3f(xWall, Ywall, Zwall);
    glTexCoord2f(0.0, rowsAndCol + 2); glVertex3f(xWall, yWall, Zwall);
    glEnd();


    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);

}
void drawGlass() {
    //Left Wall: GLass


    glEnable(GL_DEPTH_TEST); // Enable depth testing.

    // Light property vectors.
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
    float lightPos[] = { 0.0, 1.5, 3.0, 1.0 };


    // Light properties.

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

    float glassR = 168.0, glassG = 204.0, glassB = 215.0;
    // Material properties of the Glass Wall.
    float GlassAmbAndDif1[] = { glassR / 255.0, glassG / 255.0, glassB / 255.0, 0.5 }; // Alpha value of box sides = 0.5.
    float GlassSpec[] = { 1.0, 1.0, 1.0, 1.0 };
    float GlassShine[] = { 50.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, GlassAmbAndDif1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, GlassSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, GlassShine);


    glEnable(GL_BLEND); // Enable blending.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.

    glDepthMask(GL_FALSE); // Make depth buffer read-only.


    glNormal3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-xWall, yWall, zWall);
    glVertex3f(-xWall, Ywall, zWall);
    glVertex3f(-xWall, Ywall, Zwall);
    glVertex3f(-xWall, yWall, Zwall);
    glEnd();
 

    glDepthMask(GL_TRUE); // Make depth buffer writable.

}


void drawMan() {
    float xMan = 15.0, yMan = 0.0, zMan = -50.0, manRad = 2.5, chestRad = 2.5, radBase = 1.5;
    float baseRad = 1.0, topRad = baseRad, height = 7.0;
    float
        headAmbAndDif[] = { skinR / 255.0, skinG / 255.0, skinB / 255.0, 1.0 },
        headSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
        headShine[] = { 10.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, headAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, headSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, headShine);

    qobj = gluNewQuadric();

    //Toros 
    glPushMatrix();
    glTranslatef(xMan, yMan, zMan);
    glRotatef(180, 1, 0, 0);
    glScalef(2.0, 2.0, 1.0);
    glutSolidSphere(manRad, 5.0, 5.0);
    glPopMatrix();

    //Chest
    glPushMatrix();
    glTranslatef(xMan, yMan+7.0, zMan);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(chestRad, 20.0, 20.0);
    glPopMatrix();

    float
        baseAmbAndDif[] = { 160 / 255.0, 160 / 255.0, 160 / 255.0, 1.0 },
        baseSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
        baseShine[] = { 10.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, baseAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, baseSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, baseShine);

    glPushMatrix();
    glTranslatef(xMan, yMan-4.0, zMan);
    glRotatef(90, 1, 0, 0);
    gluCylinder(qobj, baseRad, topRad, height, 20.0, 20.0);
    glPopMatrix();

}

void drawMan2() {

    float xMan2 = -15.0, yMan2 = 0.0, zMan2 = -50.0, manRad2 = 2.5, chestRad2 = 2.5, radBase2 = 1.5;
    float baseRad2 = 1.0, topRad2 = baseRad2, height2 = 7.0;
    float
        skinAmbAndDif[] = { skinR / 255.0, skinG / 255.0, skinB / 255.0, 1.0 },
        skinSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
        skinShine[] = { 10.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, skinAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, skinSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, skinShine);

    qobj = gluNewQuadric();

    //Toros 
    glPushMatrix();
    glTranslatef(xMan2, yMan2, zMan2);
    glRotatef(180, 1, 0, 0);
    glScalef(2.0, 2.0, 1.0);
    glutSolidSphere(manRad2, 5.0, 5.0);
    glPopMatrix();

    //Chest
    glPushMatrix();
    glTranslatef(xMan2, yMan2 + 7.0, zMan2);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(chestRad2, 20.0, 20.0);
    glPopMatrix();

    float
        base2AmbAndDif[] = { 160 / 255.0, 160 / 255.0, 160 / 255.0, 1.0 },
        base2Specular[] = { 0.0, 0.0, 0.0, 1.0 },
        base2Shine[] = { 10.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, base2AmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, base2Specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, base2Shine);

    glPushMatrix();
    glTranslatef(xMan2, yMan2 - 4.0, zMan2);
    glRotatef(90, 1, 0, 0);
    gluCylinder(qobj, baseRad2, topRad2, height2, 20.0, 20.0);
    glPopMatrix();
}

void drawAvatar() {

    float
        SkinsAmbAndDif[] = { skinR / 255.0, skinG / 255.0, skinB / 255.0, 1.0 },
        SkinsSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
        SkinsShine[] = { 10.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, SkinsAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SkinsSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, SkinsShine);
    qobj = gluNewQuadric();

    //Arms
    glPushMatrix();
    glTranslatef(xAvatarArm, yAvatarArm, zAvatarArm);
    glRotatef(90, 1, 0, 0);
    gluCylinder(qobj, armBaseRad, armBaseRad, armHei, 20.0, 20.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-xAvatarArm, yAvatarArm, zAvatarArm);
    glRotatef(90, 1, 0, 0);
    gluCylinder(qobj, armBaseRad, armBaseRad, armHei, 20.0, 20.0);
    glPopMatrix();


    //Legs 
    glPushMatrix();
    glTranslatef(xlegAvatar, ylegAvatar, zlegAvatar);
    glRotatef(90, 1, 0, 0);
    gluCylinder(qobj, legBaseRad, legBaseRad, legHei, 20.0, 20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-xlegAvatar, ylegAvatar, zlegAvatar);
    glRotatef(90, 1, 0, 0);
    gluCylinder(qobj, legBaseRad, legBaseRad, legHei, 20.0, 20.0);
    glPopMatrix();

    //head
    glPushMatrix();
    glTranslatef(xHeadA, yHeadA, zHeadA);
    glutSolidSphere(headRadA, 20.0, 20.0);
    glPopMatrix();

    //Toros 
    glPushMatrix();
    glTranslatef(xAvatar, yAvatar, zAvatar);
    glRotatef(180, 1, 0, 0);
    glScalef(2.0, 2.0, 1.0);
    glutSolidSphere(AvatarRad, 5.0, 5.0);
    glPopMatrix();

    //Chest
    glPushMatrix();
    glTranslatef(xAvatar, yAvatar + 7.0, zAvatar);
    glScalef(1.5, 1.5, 1.0);
    glutSolidSphere(chestRad3, 20.0, 20.0);
    glPopMatrix();



}

void drawClothing() {
    qobj = gluNewQuadric();


    float dressR = 0.0, dressG = 179.0, dressB = 119.0;
    float
        dressAmbAndDif[] = {dressR / 255.0, dressG / 255.0, dressB / 255.0, 1.0 },
        dressSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
        dressShine[] = { 10.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, dressAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dressSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, dressShine);

    // Dress
    
    glPushMatrix();
    glTranslatef(xDress, yDress, zDress);
    glRotatef(90, -1, 0, 0);
    gluCylinder(qobj, bottomRad, waistRad, longc, 20.0, 20.0);
    glPopMatrix();
    
    //Crop Shirt 
    
    float shirtR = 255.0, shirtG = 230.0, shirtB = 139.0;
    float
        shirtAmbAndDif[] = { shirtR / 255.0, shirtG / 255.0, shirtB / 255.0, 1.0 },
        shirtSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
        shirtShine[] = { 10.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, shirtAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, shirtSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shirtShine);

    glPushMatrix();
    glTranslatef(xShirt, yShirt, zShirt);
    glRotatef(90, 1, 0, 0);
    gluCylinder(qobj, topRad, shirtEndrad, longS, 20.0, 20.0);
    glPopMatrix();
    // Sleeves
    glPushMatrix();
    glTranslatef(xSleeves, ySleeves, zSleeves);
    glRotatef(90, 1, 0, 0);
    gluCylinder(qobj, sleevesRtop, sleevesREnd, longeves, 20.0, 20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xSleeves+8, ySleeves, zSleeves);
    glRotatef(90, 1, 0, 0);
    gluCylinder(qobj, sleevesRtop, sleevesREnd, longeves, 20.0, 20.0);
    glPopMatrix();
    
    //Pants 
 
    float pantR = 52.0, pantG = 97.0, pantB = 133.0;
    float
        pantAmbAndDif[] = {pantR / 255.0, pantG / 255.0, pantB / 255.0, 1.0 },
        pantSpecular[] = { 0.0, 0.0, 0.0, 1.0 },
        pantShine[] = { 10.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pantAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pantSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pantShine);

    glPushMatrix();
    glTranslatef(xPant, yPant, zPant);
    glRotatef(180, 1, 0, 0);
    glScalef(2.0, 2.0, 1.0);
    glutSolidSphere(pantRad, 5.0, 5.0);
    glPopMatrix();

}



// Drawing routine.
void drawScene(void)
{
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(135.0, 1.0, 10.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //gluLookAt goes here
    gluLookAt(meX, meY, meZ, meX + sin(angle * PI / 180), meY, meZ + cos(angle * PI / 180), 0, 1, 0);

    fractalPat();
   
    drawSky();
    drawClothing();
    drawMan();
    drawMan2();
    drawAvatar();
    drawStudio();
    drawGround();
    drawGlass();
    drawLights();




    glutSwapBuffers();
}

void skin_menu(int id)
{
    switch (id) {
    case SHADE1: skinR = headR; skinG = headG; skinB = headB; break;
    case SHADE2: skinR = 224.0; skinG = 172.0; skinB = 105.0; break;
    case SHADE3: skinR = 255.0; skinG = 219.0; skinB = 172.0; break;
    default: break;
    }
    glutPostRedisplay();
}


void handleQuit(int id)
{
    if (id == 9) exit(0);
}

void MakeMenu(void) {
    SkinToneMenu = glutCreateMenu(skin_menu);
    glutAddMenuEntry("Shade 1", SHADE1);
    glutAddMenuEntry("Shade 2", SHADE2);
    glutAddMenuEntry("Shade 3", SHADE3);
    glutCreateMenu(handleQuit);
    glutAddSubMenu("Change Skin Tone", SkinToneMenu);
    glutAddMenuEntry("Quit", 9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initialization routine.
void setup(void)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glClearColor(0.10, 0.169, 0.222, 0.0); // night sky
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glEnable(GL_NORMALIZE);
   // glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // Create texture index array.
    glGenTextures(numOftextures, texture);

    // Load external textures.
    loadExternalTextures();

    // Turn on OpenGL texturing.
    glEnable(GL_TEXTURE_2D);

    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}


// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(135.0, 1.0, 10.0, 100.0);
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
    case 'Q':
        yDress += 1.0;
        glutPostRedisplay();
        break;
    case 'q':
        yDress -= 1.0;
        glutPostRedisplay();
        break;

    case 'W':
        yShirt += 1.0;
        ySleeves += 1.0;
        glutPostRedisplay();
        break;
    case 'w':
        yShirt -= 1.0;
        ySleeves -= 1.0;
        glutPostRedisplay();
        break;

    case 'E':
        yPant += 1.0;
        glutPostRedisplay();
        break;
    case 'e':
        yPant -= 1.0;
        glutPostRedisplay();
        break;

    case 'D':
        xDress = xAvatar;
        glutPostRedisplay();
        break;
    case 'd':
        xDress = -15.0;
        glutPostRedisplay();
        break;
    case 'S':
        xSleeves = xAvatar-4;
        xShirt = xAvatar;
        glutPostRedisplay();
        break;
    case 's':
        xSleeves = 11.0;
        xShirt = 15.0;
        glutPostRedisplay();
        break;
    case 'F':
        xPant = xAvatar;
        glutPostRedisplay();
        break;
    case 'f':
        xPant = 15.0;
        glutPostRedisplay();
        break;
    case 'K':
        maxLevel++;
        glutPostRedisplay();
        break;
    case 'k':
        if (maxLevel > 0) maxLevel--;
        glutPostRedisplay();
        break;
    case 'T':
        maxLevelT++;
        glutPostRedisplay();
        break;
    case 't':
        if (maxLevelT > 0) maxLevelT--;
        glutPostRedisplay();
        break;

    case 'O':
         xdoor-= -5;
        glutPostRedisplay();
        break;
    case 'o':
        if (xdoor > yWall) xdoor += -5;
        glutPostRedisplay();
        break;
    case '0':
        if (!ceilingIsLit) ceilingIsLit = true;
        glutPostRedisplay();
        break;
    case '9':
        ceilingIsLit = false;
        glutPostRedisplay();
        break;
    case '1':
        shape = KOCH;
        glutPostRedisplay();
        break;
    case '2':
        shape = KOCHVARIANT;
        glutPostRedisplay();
        break;

    case '+':
        if (meY < 85) meY = meY + UpAndDown;
        glutPostRedisplay();
        break;
    case '-':
        if (meY > 5) meY = meY - UpAndDown;
        glutPostRedisplay();
        break;
    case 'r':
        xDress = -15.0;
        xSleeves = 11.0;
        xShirt = 15.0;
        xPant = 15.0;
        yDress = -5.5;
        yShirt = 8.0;
        ySleeves = 7.5;
        yPant = 0.0;
        maxLevel = 0;
        maxLevelT = 0;
        xdoor = -10.0;
        ceilingIsLit = false;
        shape = KOCH; 
        angle = 0.0;
        meY = 10.0;
        meX = 0.0;
        meZ = -100.0;
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
        if (meZ + stepsize * cos(angle * PI / 180) <= -35) {
            meZ = meZ + stepsize * cos(angle * PI / 180); //z
            meX = meX + stepsize * sin(angle * PI / 180); //x
        }
        break;
    case GLUT_KEY_DOWN: //back
        if (meZ + stepsize * cos(angle * PI / 180) >= -95) {
        meZ -= stepsize * cos(angle * PI / 180); //z
        meX -= stepsize * sin(angle * PI / 180); //x
        }
        break;
    case GLUT_KEY_RIGHT: //turn right
        angle += turnsize;

        break;
    case GLUT_KEY_LEFT: //turn left
        angle -= turnsize;

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
    cout << "\t\t +/-: Increase/Decrease on the Y-axis" << endl;
    cout << "\t\t 0: Turn on Ceiling Light" << endl;
    cout << "\t\t 9: Turn off Ceiling Light" << endl;
    cout << "\t\t 1: Pattern on the Skirt is Koch, watch how the leaves turn on the tree if its grown for you " << endl;
    cout << "\t\t 2: Pattern on the Skirt is KachiVariant, watch how the leaves turn on the tree if its grown for you" << endl;
    cout << "\t\t D/d: Put the Skirt on Avatar/Mannequin" << endl;
    cout << "\t\t F/f: Put Pants on the Avatar/Mannequin" << endl;
    cout << "\t\t S/s: Put Shirt on the Avatar/Mannequin" << endl;
    cout << "\t\t W/w, E/e Q/q: Adjust clothing (W: Shirt, E: Pants, Q: Dress) by moving up and down on Avatar/ Mannequin" << endl;
    cout << "\t\t K/k: Increase/decrease the Fractals of the Koch or KachiVariant, watch how the leaves turn on the tree if its grown for you" << endl;
    cout << "\t\t O/o: Open or Close Door" << endl;
    cout << "\t\t T/t: Grow/De-Grow the Tree" << endl;
    cout << "\t\t r: Resets everything to the original position but the door " << endl;
    cout << "\t\t up_arrow: Moves Forward" << endl;
    cout << "\t\t down_arrow: Moves Backwards" << endl;
    cout << "\t\t right_arrow: Turns Right" << endl;
    cout << "\t\t left_arrow: Turns Left" << endl;
    cout << "\t\t Right-Click on Track Pad: A menu show up and you can change the Skin Tone or Quit" << endl;

}

// Main routine.
int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(750, 750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Minimalistic Boutique");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    MakeMenu();

    glutMainLoop();

    return 0;
}