#include "mouse.h"
#include "utility.h"
#include "globalVariables.h"
#include "camera.h"
#include "animate.h"
#include "collision.h"

extern float theta;
extern float phi;
extern float theta_step;
extern float phi_step;

/* 0.005 for mouse */
float sensitivity = 0.01;
static bool right_down=false;
static bool left_down=false;


/* get a direction of mouse picking ray */
Vector3f getDirection(float fx, float fy) {
	fx -= window_width / 2.0;
	fy = window_height / 2.0 - fy;

	fy /= (window_height / 2.0);
	fx /= (window_width / 2.0);

	int nearClippingPlaneDistance = 1;
	Vector3f pos = camera_position + view*nearClippingPlaneDistance + horizontal_vector*fx + vertical_vector*fy;
	
	Vector3f dir = pos - camera_position;

	return dir;
}



void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON &&  state == GLUT_DOWN) { 
		left_down=true;
		x=window_width/2;
		y=window_height/2;

		if (space.selected_brick!=-1) {
			space.put_down();
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
		  float cPos = camera_position.get(i);
		  float direc = dir.get(i);
		  for (int currentPlane = 0; currentPlane < space.size; currentPlane++) {

			  /* FIX check dividing by zero */
				float k = ((i!=2) ? (currentPlane-cPos)/direc : (-currentPlane-cPos)/direc);
				float xf = camera_position.x + k*dir.x;
				float yf = camera_position.y + k*dir.y;
				float zf = -(camera_position.z + k*dir.z);
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
						if (id!=0 && id!=255) {
							d = (camera_position-Vector3f(x, yf, -zf)).normSquared();
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
					if (id!=0 && id!=255) {
						d = (camera_position-Vector3f(x, yf, -zf)).normSquared();
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
						if (id!=0 && id!=255) {
							d = (camera_position-Vector3f(xf, y, -zf)).normSquared();
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
					if (id!=0 && id!=255) {
						d = (camera_position-Vector3f(xf, y, -zf)).normSquared();
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
						if (id!=0 && id!=255) {
							d = (camera_position-Vector3f(xf, yf, -z)).normSquared();
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
					if (id!=0 && id!=255) {
						d = (camera_position-Vector3f(xf, yf, -z)).normSquared();
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
		
		int indeks = ut_index_of_minimum(ds[0], ds[1], ds[2]);
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

		if (space.selected_brick != -1) {
			to.x = objX-camera_position.x;
			to.z = objZ-camera_position.z;
			to.y = objY-camera_position.y;

			/* for sphere coordinates */
			float modultheta = sqrt(to.x*to.x+to.z*to.z+to.y*to.y);
			/* for polar coordinates */
			/* float modultheta = sqrt(to.x*to.x+to.z*to.z); */

			to.x /= modultheta;
			to.z /= modultheta;
			to.y /= modultheta;
		}

		glutPostRedisplay();
	}	


	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_down=false;
		space.put_down();

		/* for polar coordinates */
		phi = std::asin(to.y);
		theta = std::acos(-to.z);
		if (to.x<0)
			theta *= -1;



		/* for sphere coordinates */
		phi = std::acos(-to.y);
		theta = std::acos(to.x/(std::sin(phi)));
		if (to.z<0)
			theta *= -1;

		get_vectors();

		glutPostRedisplay();
	}

	if (button == GLUT_RIGHT_BUTTON &&  state == GLUT_DOWN) { 
		right_down=true;
	}

	if (button == GLUT_RIGHT_BUTTON &&  state == GLUT_UP) { 
		right_down=false;
	}

}

void mouseLook(int x, int y) {
	theta_step = (x - window_width/2) * 0.0015;
	phi_step = - (y - window_height/2) * 0.0015;

	if (theta_step) {
		calculate_camera_look();
	}

	if (phi_step) {
		calculate_camera_look();
	}

	theta_step = phi_step = 0;
	
	if (x!=window_width/2 || y!=window_height/2)
		glutWarpPointer(window_width / 2, window_height / 2);
}

void on_mouse_active_move(int x, int y) {

	if (space.selected_brick==-1) {
		mouseLook(x,y);
		glutPostRedisplay();
		return;
	}

	if (x == window_width/2 && y == window_height/2)
		return;

	Brick& current_brick = space.bricks[space.selected_brick];

	float delta_x = x-window_width/2;

	delta_x *= sensitivity;

	float d = sqrt(to.x*to.x+to.z*to.z);

	float ax=0,bx=0,ay=0,by=0;

	if (delta_x) {
		bx=-to.z*(delta_x/d);
		ax=-to.x*(delta_x/d);
	}


	float delta_y = -y+window_height/2;

	delta_y *= sensitivity;



	if (delta_y) {
		by=to.x*(delta_y/d);
		ay=-to.z*(delta_y/d);
	}

	if (left_down && !right_down) {
		float a = ax+ay;
		float b = bx+by;

		/* if this is > 1 collision will not work properly because we cannot skip a field */
		if (a>0.9)
			a = 0.9;
		if (a<-0.9)
			a = -0.9;
		if (b>0.9)
			b = 0.9;
		if (b<-0.9)
			b = -0.9;

		if (b>0)
			move_brick(Right,current_brick,b);
		if (b<0)
			move_brick(Left,current_brick,-b);
		if (a>0)
			move_brick(Backward,current_brick,a);
		if (a<0)
			move_brick(Forward,current_brick,-a);
	}

	if (left_down && right_down) {
		if (delta_y>0.9)
			delta_y = 0.9;
		if (delta_y<-0.9)
			delta_y = -0.9;
		if (delta_y>0)
			move_brick(Up, current_brick, delta_y);
		else if (delta_y<0)
			move_brick(Down, current_brick, -delta_y);
	}

	if (x!=window_width/2 || y!=window_height/2)
		glutWarpPointer(window_width / 2, window_height / 2);

	glutPostRedisplay();
}


void on_mouse_passive_move(int x, int y) {
	mouseLook(x, y);
	glutPostRedisplay();
}
