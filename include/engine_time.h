#pragma once

#include <limits.h>
#include <time.h>

struct Time
{
	float last_timestamp;
	float current_timestamp;
	float delta_time;
};

extern struct Time Time;

static void engine_time_init(void)
{
	Time.current_timestamp = Time.last_timestamp = ((float) clock()) / CLOCKS_PER_SEC;
	Time.delta_time = 0;
}

static void engine_time_update(void)
{
	Time.last_timestamp = Time.current_timestamp;
	Time.current_timestamp  = ((float) clock()) / CLOCKS_PER_SEC;

	if (Time.current_timestamp < Time.last_timestamp) {
		// clock() returns type clock_t which is a typedef to long
		Time.delta_time = (((float) LONG_MAX / CLOCKS_PER_SEC) - Time.last_timestamp) + (Time.current_timestamp - ((float) LONG_MIN / CLOCKS_PER_SEC));
	} else {
		Time.delta_time = Time.current_timestamp - Time.last_timestamp;
	}
}
