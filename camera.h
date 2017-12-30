#ifndef _CAMERA_H_
#define _CAMERA_H_


void get_vectors();
void recover_angles();
void calculate_camera_forward_position();
void calculate_camera_left_position();
void calculate_camera_up_position();
void calculate_camera_look(float theta_step, float phi_step);
void camera_on_timer(int value);

#endif
