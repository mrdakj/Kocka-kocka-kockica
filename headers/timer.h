#ifndef _TIMER_H_
#define _TIMER_H_

class Timer {
private:
	bool active;
	int id;
	int interval;
	void (*on_timer)(int value);

	/* number of timers used for ids */
	static int number;

public:
	Timer(void (*on_timer)(int value));
	Timer(int interval, void (*on_timer)(int value));

	void activate();
	bool is_active() const;
	void stop();
	bool check(int value) const;

	/* continue */
	bool cont() const;

};

#endif
