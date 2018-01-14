#include <GL/glut.h>
#include "../headers/timer.h"

int Timer::number = 0;

Timer::Timer(void (*on_timer)(int value)) : Timer(10 ,on_timer) {}
Timer::Timer(int interval, void (*on_timer)(int value)) : active(false), id(++number), interval(interval), on_timer(on_timer) {}

void Timer::activate()
{
	if (!active) {
		active = true;
		glutTimerFunc(interval, on_timer, id);
	}
}

bool Timer::is_active() const
{
	return active;
}

void Timer::stop()
{
	active = false;
}

bool Timer::check(int value) const
{
	return id == value;
}

bool Timer::cont() const
{
	if (!active) return false;
	glutTimerFunc(interval, on_timer, id);
	return true;
}
