#include "mouse.h"
#include "utility.h"
#include "global_variables.h"
#include "camera.h"
#include "collision.h"
#include <stdio.h>


static float sensitivity = 0.01;
static bool mouse_right_button_clicked = false;
static bool mouse_left_button_clicked = false;

float objX, objY, objZ;


/* get a direction of mouse picking ray */
Vector3f get_ray_direction(float fx, float fy) {

	Vector3f horizontal_vector = camera.view * Vector3f(0,1,0);
	horizontal_vector.normalize();
	Vector3f vertical_vector=horizontal_vector*camera.view;
	vertical_vector.normalize();

	float rad = fovy * PI / 180;
	float vLength = std::tan( rad / 2 ) * near_clipping_distance;
	float hLength = vLength * ((float)(window_width) / (window_height));
	
	vertical_vector = vertical_vector*vLength;
	horizontal_vector = horizontal_vector*hLength;

	fx -= window_width/2.0;
	fy = window_height/2.0 - fy;

	fy /= window_height/2.0;
	fx /= window_width/2.0;


	Vector3f point_at_clipping_plane = 
		camera.position + near_clipping_distance*camera.view + fx*horizontal_vector + fy*vertical_vector;
	
	Vector3f direction = point_at_clipping_plane - camera.position;

	return direction;
}



void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON &&  state == GLUT_DOWN) { 
		mouse_left_button_clicked=true;
		x=window_width/2;
		y=window_height/2;

		if (space.selected_brick!=-1) {
			space.put_down();
			glutPostRedisplay();	
		}

		Vector3f dir = get_ray_direction(x,y);


		/* find intersection of planes and ray */
		int ids[3]={-1,-1,-1};
		float ds[3]={999999,999999,999999};

		float clickX0, clickY0, clickZ0;
		float clickX1, clickY1, clickZ1;
		float clickX2, clickY2, clickZ2;

		for (int i = 0; i < 3; i++) {
		  float cPos = camera.position.get(i);
		  float direc = dir.get(i);
		  for (int currentPlane = 0; currentPlane < space.size; currentPlane++) {

			  /* FIX check dividing by zero */
				float k = ((i!=2) ? (currentPlane-cPos)/direc : (-currentPlane-cPos)/direc);
				float xf = camera.position.x + k*dir.x;
				float yf = camera.position.y + k*dir.y;
				float zf = -(camera.position.z + k*dir.z);
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
							Vector3f pick_vector = Vector3f(x, yf, -zf) - camera.position;
							d = (camera.position-Vector3f(x, yf, -zf)).norm_squared();
							if (d<ds[i] && camera.view.dot(pick_vector)>0) {
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
						Vector3f pick_vector = Vector3f(x, yf, -zf) - camera.position;
						d = (camera.position-Vector3f(x, yf, -zf)).norm_squared();
						if (d<ds[i] && camera.view.dot(pick_vector)>0) {
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
							Vector3f pick_vector = Vector3f(xf, y, -zf) - camera.position;
							d = (camera.position-Vector3f(xf, y, -zf)).norm_squared();
							if (d<ds[i] &&  camera.view.dot(pick_vector)>0) {
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
						Vector3f pick_vector = Vector3f(xf, y, -zf) - camera.position;
						d = (camera.position-Vector3f(xf, y, -zf)).norm_squared();
						if (d<ds[i] &&  camera.view.dot(pick_vector)>0) {
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
							Vector3f pick_vector = Vector3f(xf, yf, -z) - camera.position;
							d = (camera.position-Vector3f(xf, yf, -z)).norm_squared();
							if (d<ds[i] &&  camera.view.dot(pick_vector)>0) {
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
						Vector3f pick_vector = Vector3f(xf, yf, -z) - camera.position;
						d = (camera.position-Vector3f(xf, yf, -z)).norm_squared();
						if (d<ds[i] &&  camera.view.dot(pick_vector)>0) {
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
			camera.view.x = objX-camera.position.x;
			camera.view.z = objZ-camera.position.z;
			camera.view.y = objY-camera.position.y;

			float modultheta = std::sqrt(camera.view.x*camera.view.x+camera.view.z*camera.view.z+camera.view.y*camera.view.y);

			camera.view.x /= modultheta;
			camera.view.z /= modultheta;
			camera.view.y /= modultheta;

		}

		glutPostRedisplay();
	}	


	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mouse_left_button_clicked=false;
		space.put_down();

		camera.recover_angles();

		glutPostRedisplay();
	}

	if (button == GLUT_RIGHT_BUTTON &&  state == GLUT_DOWN) { 
		mouse_right_button_clicked=true;
	}

	if (button == GLUT_RIGHT_BUTTON &&  state == GLUT_UP) { 
		mouse_right_button_clicked=false;
	}

}

static bool mouse_at_center(int x, int y) {
	return x == window_width/2 && y == window_height/2;
}

void mouse_look(int x, int y) {
	float theta_step = (x - window_width/2) * 0.0015;
	float phi_step = - (y - window_height/2) * 0.0015;

	if (theta_step)
		camera.rotate(theta_step, phi_step);

	if (phi_step)
		camera.rotate(theta_step, phi_step);

	if (!mouse_at_center(x, y))
		glutWarpPointer(window_width/2, window_height/2);
}


void on_mouse_active_move(int x, int y) {

	if (space.selected_brick == NONE) {
		mouse_look(x,y);
		glutPostRedisplay();
		return;
	}
	
	if (mouse_at_center(x, y)) return;

	Brick& current_brick = space.bricks[space.selected_brick];

	float delta_x = x-window_width/2;
	delta_x *= sensitivity;

	float delta_y = -y+window_height/2;
	delta_y *= sensitivity;


	if (mouse_left_button_clicked && !mouse_right_button_clicked) {
		move_delta(delta_x, delta_y, current_brick);
	}

	if (mouse_left_button_clicked && mouse_right_button_clicked) {
		if (delta_y > 0.9) delta_y = 0.9;
		if (delta_y < -0.9) delta_y = -0.9;
		if (delta_y > 0)
			move_brick(Up, current_brick, delta_y);
		else if (delta_y < 0)
			move_brick(Down, current_brick, -delta_y);
	}

	if (!mouse_at_center(x, y)) glutWarpPointer(window_width/2, window_height/2);

	glutPostRedisplay();
}


void on_mouse_passive_move(int x, int y) {
	mouse_look(x, y);
	glutPostRedisplay();
}
