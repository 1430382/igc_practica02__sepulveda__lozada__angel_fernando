/*
 * GL07BouncingBall.cpp: A ball bouncing inside the window
 */
//#include <windows.h>  // for MS Windows
#include <iostream>
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <math.h>     // Needed for sin, cos
#define PI 3.14159265f

// Global variables
char title[] = "Bouncing Ball (2D)";  // Windowed mode's title
int windowWidth  = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX   = 50;      // Windowed mode's top-left corner x
int windowPosY   = 50;      // Windowed mode's top-left corner y

int refreshMillis = 30;      // Refresh period in milliseconds

int tamano = 0;

GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

class bouncingBall{
public:
  GLfloat ballRadius;   // Radius of the bouncing ball
  GLfloat ballX;         // Ball's center (x, y) position
  GLfloat ballY;

  GLfloat xSpeed;      // Ball's speed in x and y directions
  GLfloat ySpeed;
  GLfloat ballXMax, ballXMin, ballYMax, ballYMin; // Ball's center (x, y) bounds

  bouncingBall(GLfloat ballX, GLfloat ballY){
    //std::cout << "En la clase\n" << "X: " << ballX/1000 << "  Y: "<< ballY/1000 << std::endl;
    this->ballRadius = 0.1f;
    this->ballX = ballX/1000;
    this->ballY = ballY/1000;
    this->xSpeed = 0.02f;
    this->ySpeed = 0.007f;
  }
  bouncingBall(){
    this->ballRadius = 0.1f;
    this->ballX = 0.0f;
    this->ballY = 0.0f;
    this->xSpeed = 0.02f;
    this->ySpeed = 0.007f;
  }

  void dibujar(){
    glLoadIdentity();
    glTranslatef(this->ballX, this->ballY, 0.0f);  // Translate to (xPos, yPos)
    // Use triangular segments to form a circle
    glBegin(GL_TRIANGLE_FAN);
       glColor3f(0.0f, 0.0f, 1.0f);  // Blue
       glVertex2f(0.0f, 0.0f);       // Center of circle
       int numSegments = 100;
       GLfloat angle;
       for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
          angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
          glVertex2f(cos(angle) * this->ballRadius, sin(angle) * this->ballRadius);
       }
    glEnd();
  }
};

bouncingBall *pelotas;

/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
   pelotas = new bouncingBall[++tamano];
}

/* Callback handler for window re-paint event */
void display() {
   glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
   glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix

   for(int i=0; i<tamano; i++){
     pelotas[i].dibujar();
   }

   glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)

   // Animation Control - compute the location for the next refresh
   for(int i=0; i<tamano; i++){
     pelotas[i].ballX += pelotas[i].xSpeed;
     pelotas[i].ballY += pelotas[i].ySpeed;
     // Check if the ball exceeds the edges
     if (pelotas[i].ballX > pelotas[i].ballXMax) {
        pelotas[i].ballX = pelotas[i].ballXMax;
        pelotas[i].xSpeed = -(pelotas[i].xSpeed);
     } else if (pelotas[i].ballX < pelotas[i].ballXMin) {
        pelotas[i].ballX = pelotas[i].ballXMin;
        pelotas[i].xSpeed = -(pelotas[i].xSpeed);
     }
     if (pelotas[i].ballY > pelotas[i].ballYMax) {
        pelotas[i].ballY = pelotas[i].ballYMax;
        pelotas[i].ySpeed = -(pelotas[i].ySpeed);
     } else if (pelotas[i].ballY < pelotas[i].ballYMin) {
        pelotas[i].ballY = pelotas[i].ballYMin;
        pelotas[i].ySpeed = -(pelotas[i].ySpeed);
     }

     for(int j=0; j<tamano; j++){
       GLfloat d = sqrt(pow((pelotas[j].ballY - (pelotas[i].ballY) ),2)+pow((pelotas[j].ballX - (pelotas[i].ballX) ),2));
       if( (d <= pelotas[i].ballRadius*2 || d <= pelotas[j].ballRadius*2) and i!=j ){
         if(pelotas[i].ballX > pelotas[j].ballX){
          pelotas[i].xSpeed = -(pelotas[i].xSpeed);
          pelotas[j].xSpeed = -(pelotas[j].xSpeed);
        }
        if(pelotas[i].ballY > pelotas[j].ballY){
         pelotas[i].ySpeed = -(pelotas[i].ySpeed);
         pelotas[j].ySpeed = -(pelotas[j].ySpeed);
       }
      }
     }
   }
}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset the projection matrix
   if (width >= height) {
      clipAreaXLeft   = -1.0 * aspect;
      clipAreaXRight  = 1.0 * aspect;
      clipAreaYBottom = -1.0;
      clipAreaYTop    = 1.0;
   } else {
      clipAreaXLeft   = -1.0;
      clipAreaXRight  = 1.0;
      clipAreaYBottom = -1.0 / aspect;
      clipAreaYTop    = 1.0 / aspect;
   }
   gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);

   for(int i=0; i<tamano; i++){
     pelotas[i].ballXMin = clipAreaXLeft + pelotas[i].ballRadius;
     pelotas[i].ballXMax = clipAreaXRight - pelotas[i].ballRadius;
     pelotas[i].ballYMin = clipAreaYBottom + pelotas[i].ballRadius;
     pelotas[i].ballYMax = clipAreaYTop - pelotas[i].ballRadius;
   }

}

/* Called back when the timer expired */
void Timer(int value) {
   glutPostRedisplay();    // Post a paint request to activate display()
   glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}

/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27:     // ESC key
         exit(0);
         break;
   }
}

/* Callback handler for mouse event */
void mouse(int button, int state, int x, int y) {
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // Pause/resume
     float rx =  ( (windowWidth - windowWidth / 2) - x);
     float rx1 = ( rx/ (windowWidth / 2) )*-1;
     float ry =  ( (windowHeight - windowHeight / 2) - y );
     float ry1 = ry / (windowHeight / 2);
     bouncingBall *aux = new bouncingBall[tamano];
     bouncingBall dato(rx, ry);
     std::cout << "X: " << x << "  Y: "<< y << std::endl;

     for(int i=0; i<tamano;i++){
       aux[i] = pelotas[i];
     }
     delete[] pelotas;
     pelotas = NULL;
     pelotas = new bouncingBall[tamano+1];
     for(int i=0; i<tamano;i++){
       pelotas[i] = aux[i];
     }
     pelotas[tamano] = dato;
     delete[] aux;
     tamano++;
     aux = NULL;

     for(int i=0; i<tamano; i++){
       pelotas[i].ballXMin = clipAreaXLeft + pelotas[i].ballRadius;
       pelotas[i].ballXMax = clipAreaXRight - pelotas[i].ballRadius;
       pelotas[i].ballYMin = clipAreaYBottom + pelotas[i].ballRadius;
       pelotas[i].ballYMax = clipAreaYTop - pelotas[i].ballRadius;
     }
   }
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   initGL();                     // Our own OpenGL initialization
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
   glutCreateWindow(title);      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutTimerFunc(0, Timer, 0);   // First timer call immediately
   glutMouseFunc(mouse);   // Register callback handler for mouse event
   glutKeyboardFunc(keyboard);
   glutMainLoop();               // Enter event-processing loop
   return 0;
}
