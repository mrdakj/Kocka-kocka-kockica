#include "globalVariables.h"

void getVectors() {
	view = to;
	view.normalize();
	hvector = view * Vector3f(0,1,0);
	hvector.normalize();
	v=hvector*view;
	v.normalize();
	float rad = fovy * 3.14159 / 180;
	float vLength = tan( rad / 2 ) * nearClippingPlaneDistance;
	float hLength = vLength * ((float)windowWidth / windowHeight);
	v = v*vLength;
	hvector = hvector*hLength;
}


void getFrontPosition() {
	float d = sqrt(to.x*to.x+to.z*to.z); // not necessary for polar coordinates
	cameraPosition.x += moveFront * to.x/d * 0.2;
	cameraPosition.z += moveFront * to.z/d * 0.2;
}

void getLeftRightPosition() {
	float d = sqrt(to.x*to.x+to.z*to.z); // not necessary for polar coordinates
	cameraPosition.x += moveLeftRight * to.z/d * 0.2;
	cameraPosition.z += moveLeftRight * -to.x/d * 0.2;
}

void getUpDownPosition() {

	cameraPosition.y += moveUpDown;
}

void calculateDirection() {
	/* theta = thetaStep; */
	/* phi = phiStep; */

	/* float toxtmp = std::cos(theta)*to.x-std::sin(theta)*to.z; */
	/* float toztmp = std::sin(theta)*to.x+std::cos(theta)*to.z; */
	/* to.x = toxtmp; */
	/* to.z = toztmp; */


	/* float toytmp = std::cos(phi)*to.y-std::sin(phi)*to.z; */
	/* toztmp = std::sin(phi)*to.y+std::cos(phi)*to.z; */
	/* to.y = toytmp; */
	/* to.z = toztmp; */

	
	theta += thetaStep;
	phi += phiStep;

	/* if (phi >= 1.8) { */
	/* 	phi = 1.8; */
	/* } else if (phi <= -1.8) { */
	/* 	phi = -1.8; */
	/* } */

	/* to.x = std::sin(theta); */
	/* to.y = std::sin(phi); */
	/* to.z = -std::cos(theta); */



	float PI=3.141592;
	if (phi >= PI)
		phi = PI;
	if (phi<0.001) {
		phi= 0.001;
	}

	/*FIX calculate sin and cos only once */
	to.x = std::sin(phi)*std::cos(theta);
	to.z = std::sin(phi)*std::sin(theta);
	to.y = -std::cos(phi);

	getVectors();
}
