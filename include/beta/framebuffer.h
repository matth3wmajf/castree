#pragma once

#include <stdio.h>
#include <stdint.h>

#include <beta/numeric.h>

/* The framebuffer's structure & handle. */
typedef struct
{
	/*
	 *	The framebuffer's settings.
	 *	The first bit represents the initialization state.
	 *	The second bit represents the allocation state.
	 */
	uint8_t t_settings;

	/* The framebuffer's resolution. */
	uintmax_vector2_t t_resolution;

	/* The framebuffer's data. */
	uint8_t *t_data;
} framebuffer_t;

/* The framebuffer's creation function. */
int framebuffer_create(framebuffer_t *pt_framebuffer);

/* The framebuffer's re-sizing function. */
int framebuffer_resize(framebuffer_t *pt_framebuffer, uintmax_vector2_t t_resolution);

/* The framebuffer's data writing function. */
int framebuffer_fwrite(framebuffer_t *pt_framebuffer, FILE *pt_file);

/* The framebuffer's deletion function. */
int framebuffer_delete(framebuffer_t *pt_framebuffer);

/*
 *	The framebuffer's rendering function.
 *	It a virtual function that is meant to be implemented by the user.
 */
extern int (*framebuffer_render)(framebuffer_t *pt_framebuffer);