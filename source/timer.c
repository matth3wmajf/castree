#include <time.h>
#include <stdint.h>

#include <beta/timer.h>

int timer_create(timer_t *pt_timer)
{
	/* Ensure that the object is valid. */
	if(pt_timer == NULL) return -1;

	/* Initialize the data to zero. */
	pt_timer->t_start = 0;
	pt_timer->t_stop = 0;

	/*
	 *	Set the initialization state to true, and the running state to false.
	 */
	pt_timer->t_settings |= 0b00000001;
	pt_timer->t_settings &= 0b11111101;

	/* Return with success. */
	return 0;
}

int timer_toggle(timer_t *pt_timer)
{
	/* Ensure that the object is valid. */
	if(pt_timer == NULL) return -1;

	/* Ensure that the timer is initialized. */
	if(!(pt_timer->t_settings & 0b00000001)) return -2;

	/* Toggle the running state. */
	pt_timer->t_settings ^= 0b00000010;

	/* If the running state is enabled, then set the start time. */
	if(pt_timer->t_settings & 0b00000010) pt_timer->t_start = clock();

	/* If the running state is disabled, then set the stop time. */
	if(!(pt_timer->t_settings & 0b00000010)) pt_timer->t_stop = clock();

	/* Return with success. */
	return 0;
}

int timer_obtain(timer_t *pt_timer, uintmax_t *pt_time)
{
	/* Ensure that the object is valid. */
	if(pt_timer == NULL) return -1;

	/* Ensure that the timer is initialized. */
	if(!(pt_timer->t_settings & 0b00000001)) return -2;

	/* Ensure that the running state is disabled. */
	if(pt_timer->t_settings & 0b00000010) return -3;

	/*
	 *	Calculate the time difference, convert it to milliseconds, and output
	 *	the value.
	 */
	*pt_time = (clock() - pt_timer->t_start) / (CLOCKS_PER_SEC / 1000);

	/* Return with success. */
	return 0;
}

int timer_delete(timer_t *pt_timer)
{
	/* Ensure that the object is valid. */
	if(pt_timer == NULL) return -1;

	/* Ensure that the timer is initialized. */
	if(!(pt_timer->t_settings & 0b00000001)) return -2;

	/* Initialize the data to zero. */
	pt_timer->t_start = 0;
	pt_timer->t_stop = 0;

	/*
	 *	Initialize the settings to their default value(s), for possible
	 *	re-use.
	 */
	pt_timer->t_settings = 0b00000000;

	/* Return with success. */
	return 0;
}