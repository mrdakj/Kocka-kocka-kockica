#ifndef _CAMERA_H_
#define _CAMERA_H_

#define CAMERA_TIMER_ID 1

void get_vectors();
void calculate_camera_forward_position();
void calculate_camera_left_position();
void calculate_camera_up_position();
void calculate_camera_look();
void camera_on_timer(int value);

#endif
