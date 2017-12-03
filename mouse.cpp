#include "mouse.h"
#include "utility.h"
#include "globalVariables.h"
#include "camera.h"
#include "animate.h"

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

		float clickX0, clickY0, clickZ0;
		float clickX1, clickY1, clickZ1;
		float clickX2, clickY2, clickZ2;

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
								clickX0=x;
								clickY0=yf;
								clickZ0=-zf;
								ids[i]=id;
								ds[i] = d;
							}
						}
					}

					id = space.matrix[x][z][y]; 
					if (id!=0) {
						d = (cameraPosition-Vector3f(x, yf, -zf)).normSquared();
						if (d<ds[i]) {
							clickX0=x;
							clickY0=yf;
							clickZ0=-zf;
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
								clickX1=xf;
								clickY1=y;
								clickZ1=-zf;
								ids[i]=id;
								ds[i] = d;
							}
						}
					}

					id = space.matrix[x][z][y]; 
					if (id!=0) {
						d = (cameraPosition-Vector3f(xf, y, -zf)).normSquared();
						if (d<ds[i]) {
							clickX1=xf;
							clickY1=y;
							clickZ1=-zf;
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
								clickX2=xf;
								clickY2=yf;
								clickZ2=-z;
								ids[i]=id;
								ds[i] = d;
							}
						}
					}

					id = space.matrix[x][z][y]; 
					if (id!=0) {
						d = (cameraPosition-Vector3f(xf, yf, -z)).normSquared();
						if (d<ds[i]) {
							clickX2=xf;
							clickY2=yf;
							clickZ2=-z;
							ids[i]=id;
							ds[i] = d;
						}
					}
				}

		  }//end of for
		}//end of for
		
		int indeks = getIndexOfMinimum(ds[0], ds[1], ds[2]);
		if (indeks==0) {
			objX = clickX0;
			objY = clickY0+0.2;
			objZ = clickZ0;
		}
		if (indeks==1) {
			objX = clickX1;
			objY = clickY1+0.2;
			objZ = clickZ1;
		}
		if (indeks==2) {
			objX = clickX2;
			objY = clickY2+0.2;
			objZ = clickZ2;
		}

		int id = ids[indeks];

		space.pick(id);

		glutPostRedisplay();
	}	

	if (button == GLUT_RIGHT_BUTTON &&  state == GLUT_DOWN) { 
		space.putDown();
		glutPostRedisplay();

	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		space.putDown();
		glutPostRedisplay();
	}
}

void mouseLook(int x, int y) {
    static bool warped = false;
	
    /* Check if glutWarpPointer was just called */
    /* if so - exit to avoid program freeze */
	if (warped) {
		warped = false;
		return;
	}
	
	glutWarpPointer(windowWidth / 2, windowHeight / 2);

	thetaStep = (x - windowWidth/2) * 0.0015;
	phiStep = - (y - windowHeight/2) * 0.0015;

	if (thetaStep) {
		calculateDirection();
	}

	if (phiStep) {
		calculateDirection();
	}

	thetaStep = phiStep = 0;
	
    /* The problematic glutWarpPointer was called */
	warped = true;
}

void mouseMotion(int x, int y) {
	if (space.selected==-1) {
		mouseLook(x,y);
		return;
	}

    static bool warped = false;
	
    /* Check if glutWarpPointer was just called */
    /* if so - exit to avoid program freeze */
	if (warped) {
		warped = false;
		return;
	}
	
	glutWarpPointer(windowWidth / 2, windowHeight / 2);



	bool moveCameraTheta = false;
	bool moveCameraPhi = false;


	Cuboid& selectedCuboid = space.cuboids[space.selected];

	int delta_x = x-windowWidth/2;

	if (delta_x>0)
		delta_x=10;
	if (delta_x<0)
		delta_x=-10;


	if (delta_x) {
		float speed = abs(delta_x)*0.005;
		if (delta_x<0) {
			if (to.x-to.z>=0 && -to.x-to.z>=0) {
				move(Left,selectedCuboid,speed);
				moveCameraTheta=true;
			}
			if (-to.x-to.z>=0 && -to.x+to.z>=0) {
				move(Forward,selectedCuboid,speed);
				moveCameraTheta=true;
			}
			if (-to.x+to.z>=0 && to.x+to.z>=0) {
				move(Right,selectedCuboid,speed);
				moveCameraTheta=true;
			}
			if (to.x+to.z>=0 && to.x-to.z>=0) {
				move(Backward,selectedCuboid,speed);
				moveCameraTheta=true;
			}
		}

		if (delta_x>0) {
			if (to.x-to.z>=0 && -to.x-to.z>=0) {
				move(Right,selectedCuboid,speed);
				moveCameraTheta=true;
			}
			if (-to.x-to.z>=0 && -to.x+to.z>=0) {
				move(Backward,selectedCuboid,speed);
				moveCameraTheta=true;
			}
			if (-to.x+to.z>=0 && to.x+to.z>=0) {
				move(Left,selectedCuboid,speed);
				moveCameraTheta=true;
			}
			if (to.x+to.z>=0 && to.x-to.z>=0) {
				move(Forward,selectedCuboid,speed);
				moveCameraTheta=true;
			}
		}
	}


	int delta_y = -y+windowHeight/2;

	if (delta_y>0)
		delta_y=10;
	if (delta_y<0)
		delta_y=-10;


	if (delta_y) {
		float speed = abs(delta_y)*0.005;
		if (delta_y<0) {
			if (to.x-to.z>=0 && -to.x-to.z>=0) {
				move(Forward,selectedCuboid,speed);
				moveCameraPhi=true;
			}
			if (-to.x-to.z>=0 && -to.x+to.z>=0) {
				move(Right,selectedCuboid,speed);
				moveCameraPhi=true;
			}
			if (-to.x+to.z>=0 && to.x+to.z>=0) {
				move(Backward,selectedCuboid,speed);
				moveCameraPhi=true;
			}
			if (to.x+to.z>=0 && to.x-to.z>=0) {
				move(Left,selectedCuboid,speed);
				moveCameraPhi=true;
			}
		}

		if (delta_y>0) {
			if (to.x-to.z>=0 && -to.x-to.z>=0) {
				move(Backward,selectedCuboid,speed);
				moveCameraPhi=true;
			}
			if (-to.x-to.z>=0 && -to.x+to.z>=0) {
				move(Left,selectedCuboid,speed);
				moveCameraPhi=true;
			}
			if (-to.x+to.z>=0 && to.x+to.z>=0) {
				move(Forward,selectedCuboid,speed);
				moveCameraPhi=true;
			}
			if (to.x+to.z>=0 && to.x-to.z>=0) {
				move(Right,selectedCuboid,speed);
				moveCameraPhi=true;
			}
		}
	}

	/* if (moveCameraTheta) { */
	/* 	thetaStep = (x - windowWidth/2) * 0.0002; */
	/* 	phiStep=0; */
	/* 	calculateDirection(); */
	/* } */

	/* if (moveCameraPhi) { */
	/* 	phiStep = - (y - windowHeight/2) * 0.0002; */
	/* 	thetaStep=0; */
	/* 	calculateDirection(); */
	/* } */

	thetaStep = phiStep = 0;

    /* The problematic glutWarpPointer was called */
	warped = true;
}


void passiveMouse(int x, int y) {
	mouseLook(x, y);
}
