#pragma once

#include <time.h>
#include <stdint.h>

/* The timer's structure & handle. */
typedef struct
{
	/*
	 *	The timer's settings.
	 *	The first bit represents the initialization state.
	 *	The second bit represents the running state.
	 */
	uint8_t t_settings;

	/* The timer's temporary start & stop times. */
	clock_t t_start;
	clock_t t_stop;
} timer_t;

/* The timer's creation function. */
int timer_create(timer_t *pt_timer);

/* The timer's toggle function. */
int timer_toggle(timer_t *pt_timer);

/* The timer's function for obtaining the time difference. */
int timer_obtain(timer_t *pt_timer, uintmax_t *pt_time);

/* The timer's deletion function. */
int timer_delete(timer_t *pt_timer);