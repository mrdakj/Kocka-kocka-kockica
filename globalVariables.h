#ifndef _GLOBALVARIABLES_H_
#define _GLOBALVARIABLES_H_

#include "space.h"
#include "keyboard.h"
#include "vector3f.h"

/* #define windowWidth 1200 */
/* #define windowHeight 700 */
extern int windowWidth;
extern int windowHeight;

extern Space space;
extern float speed;

extern int animation_ongoing;
extern float yplane;
extern Button bUp, bDown, bLeft, bRight, bForward, bBackward, bSelectDeselect;
extern Vector3f cameraPosition;
extern Vector3f to;
extern Vector3f view, hvector, v;
extern float fovy;
extern int nearClippingPlaneDistance;
extern float moveFront;
extern float moveLeftRight;
extern float moveUpDown;
extern float theta;
extern float phi;
extern float thetaStep;
extern float phiStep;
// coordinates of selected_brick object
extern GLdouble objX;
extern GLdouble objY;
extern GLdouble objZ;
extern bool go;

#endif
