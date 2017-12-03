#include "mouse.h"
#include "utility.h"
#include "globalVariables.h"
#include "camera.h"

// get a direction of mouse picking ray
Vector3f getDirection(float fx, float fy) {
	fx -= windowWidth / 2.0;
	fy = windowHeight / 2.0 - fy;

	fy /= (windowHeight / 2.0);
	fx /= (windowWidth / 2.0);

	Vector3f pos = cameraPosition + view*nearClippingPlaneDistance + hvector*fx + v*fy;
	
	Vector3f dir = pos - cameraPosition;

	return dir;
}



void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON &&  state == GLUT_DOWN) { 
		x=windowWidth/2;
		y=windowHeight/2;

		if (space.selected!=-1) {
			space.putDown();
			glutPostRedisplay();	
		}

		Vector3f dir = getDirection(x,y);


		/* find intersection of planes and ray */
		int ids[3]={-1,-1,-1};
		float ds[3]={999999,999999,999999};

		for (int i = 0; i < 3; i++) {
		  float cPos = cameraPosition.get(i);
		  float direc = dir.get(i);
		  for (int currentPlane = 0; currentPlane < space.size; currentPlane++) {

				float k = ((i!=2) ? (currentPlane-cPos)/direc : (-currentPlane-cPos)/direc);
				float xf = cameraPosition.x + k*dir.x;
				float yf = cameraPosition.y + k*dir.y;
				float zf = -(cameraPosition.z + k*dir.z);
				if (i==2 && (xf<0 || xf>=space.size || yf<0 || yf>=space.size))
					continue;
				if (i==1 && (xf<0 || xf>=space.size || zf<0 || zf>=space.size))
					continue;
				if (i==0 && (yf<0 || yf>=space.size || zf<0 || zf>=space.size))
					continue;

				int x = xf;
				int y = yf;
				int z = zf;
				if (i==0) x=currentPlane;
				if (i==1) y=currentPlane;
				if (i==2) z=currentPlane;

				if (i==2 && (x<0 || x>=space.size || y<0 || y>=space.size))
					continue;
				if (i==1 && (x<0 || x>=space.size || z<0 || z>=space.size))
					continue;
				if (i==0 && (y<0 || y>=space.size || z<0 || z>=space.size))
					continue;

				float d;
				int id;

				if (i==0) {
					if (x-1>=0) {
						id = space.matrix[x-1][z][y]; 
						if (id!=0) {
							d = (cameraPosition-Vector3f(x, yf, -zf)).normSquared();
							if (d<ds[i]) {
								ids[i]=id;
								ds[i] = d;
							}
						}
					}

					id = space.matrix[x][z][y]; 
					if (id!=0) {
						d = (cameraPosition-Vector3f(x, yf, -zf)).normSquared();
						if (d<ds[i]) {
							ids[i]=id;
							ds[i] = d;
						}
					}
				}

				if (i==1) {
					if (y-1>=0) {
						id = space.matrix[x][z][y-1]; 
						if (id!=0) {
							d = (cameraPosition-Vector3f(xf, y, -zf)).normSquared();
							if (d<ds[i]) {
								ids[i]=id;
								ds[i] = d;
							}
						}
					}

					id = space.matrix[x][z][y]; 
					if (id!=0) {
						d = (cameraPosition-Vector3f(xf, y, -zf)).normSquared();
						if (d<ds[i]) {
							ids[i]=id;
							ds[i] = d;
						}
					}
				}
					
				if (i==2) {

					if (z-1>=0) {
						id = space.matrix[x][z-1][y]; 
						if (id!=0) {
							d = (cameraPosition-Vector3f(xf, yf, -z)).normSquared();
							if (d<ds[i]) {
								ids[i]=id;
								ds[i] = d;
							}
						}
					}

					id = space.matrix[x][z][y]; 
					if (id!=0) {
						d = (cameraPosition-Vector3f(xf, yf, -z)).normSquared();
						if (d<ds[i]) {
							ids[i]=id;
							ds[i] = d;
						}
					}
				}

		  }//end of for
		}//end of for
		

		int id = ids[getIndexOfMinimum(ds[0], ds[1], ds[2])];

		space.pick(id);

		glutPostRedisplay();
	}	

	if (button == GLUT_RIGHT_BUTTON &&  state == GLUT_DOWN) { 
		space.putDown();
		glutPostRedisplay();

	}
}


void mouseMotion(int x, int y) {}



void passiveMouse(int x, int y) {
	if (x>=windowWidth-10 || x<=10) {
		glutWarpPointer(windowWidth/2,y);
		prev_x=windowWidth/2;
	}
	if (y>=windowHeight-10 || y<=10) {
		glutWarpPointer(x,windowHeight/2);
		prev_y=windowHeight/2;
	}

	thetaStep = (x-prev_x)*0.002;
	if (abs(x-prev_x)>50) {
		glutWarpPointer(windowWidth/2,y);
		prev_x=windowWidth/2;
		thetaStep=0;
	}

	phiStep = -(y - prev_y) * 0.002;
	if (abs(y-prev_y)>50)
	{
		glutWarpPointer(x,windowHeight/2);
		prev_y=windowHeight/2;
		phiStep=0;
	}

	if (thetaStep) {
		calculateDirection();
	}

	if (phiStep) {
		calculateDirection();
	}

	prev_x = x;
	prev_y = y;

	thetaStep = phiStep = 0;

}
/* void passiveMouse(int x, int y) { */
/* 	y = windowHeight - y; */

/* 	thetaStep = (x - delta_x) * 0.005; */
/* 	phiStep = (y - delta_y) * 0.005; */

/* 	if (thetaStep) { */
/* 		calculateDirection2(); */
/* 	} */

/* 	if (phiStep) { */
/* 		calculateDirection2(); */
/* 	} */

/* 	delta_x = x; */
/* 	delta_y = y; */

/* 	theta = phi = 0; */
/* } */
