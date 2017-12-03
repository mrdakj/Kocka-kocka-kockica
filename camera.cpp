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
	cameraPosition.x += moveFront * to.x * 0.2;
	cameraPosition.z += moveFront * to.z * 0.2;
}

void getLeftRightPosition() {
	cameraPosition.x += moveLeftRight * to.z * 0.2;
	cameraPosition.z += moveLeftRight * -to.x * 0.2;
}

void getUpDownPosition() {

	cameraPosition.y += moveUpDown;
}

void calculateDirection() {
	theta += thetaStep;
	phi += phiStep;

	if (phi >= 0.9) {
		phi = 0.9;
	} else if (phi <= -0.9) {
		phi = -0.9;
	}

	to.x = std::sin(theta);
	to.y = std::sin(phi);
	to.z = -std::cos(theta);

	getVectors();
}
