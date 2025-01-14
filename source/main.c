#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <beta/endl.h>
#include <beta/timer.h>
#include <beta/numeric.h>
#include <beta/framebuffer.h>

int __framebuffer_render(framebuffer_t *pt_framebuffer)
{
    for(uintmax_t l_y = 0; l_y < pt_framebuffer->t_resolution.t_y; l_y++)
	{
        for(uintmax_t l_x = 0; l_x < pt_framebuffer->t_resolution.t_x; l_x++)
		{            
            float_vector3_t l_color = {0.5f, 0.5f, 0.5f};
            
            pt_framebuffer->t_data[(l_y * pt_framebuffer->t_resolution.t_x + l_x) * 3 + 0] = l_color.t_x * 255.9999f;
            pt_framebuffer->t_data[(l_y * pt_framebuffer->t_resolution.t_x + l_x) * 3 + 1] = l_color.t_y * 255.9999f;
            pt_framebuffer->t_data[(l_y * pt_framebuffer->t_resolution.t_x + l_x) * 3 + 2] = l_color.t_z * 255.9999f;
        }
    }
    
    return 0;
}

int main(int argc, const char *argv[])
{
	/* Define the framebuffer object. */
	framebuffer_t l_framebuffer = { };

	/* Define the timer object. */
	timer_t l_timer = { };

	/* Define the time variable. */
	uintmax_t l_time;

	/* Create the timer object. */
	timer_create(&l_timer);

	/* Create the framebuffer object. */
	framebuffer_create(&l_framebuffer);

	/*
	 *	Start the timer, we'll be measuring the time it takes to re-size the
	 *	framebuffer.
	 */
	timer_toggle(&l_timer);

	/*
	 *	Resize the framebuffer object.
	 *	We use the general 1080p resolution, but we could easily use any
	 *	other resolution that we wish to have.
	 */
	framebuffer_resize(&l_framebuffer, (uintmax_vector2_t){1920, 1080});

	/* Stop the timer. */
	timer_toggle(&l_timer);

	/* Obtain the time that it took between the timer's start and stop. */
	timer_obtain(&l_timer, &l_time);

	/* Print the amount of time that it took to re-size the framebuffer object. */
	fprintf(stderr, "debug: Took %jums to create the framebuffer object." ENDL, l_time);

	/* Set the framebuffer's render function. */
	framebuffer_render = __framebuffer_render;

	/*
	 *	Start the timer, we'll be measuring the time it takes to render to the
	 *	framebuffer.
	 */
	timer_toggle(&l_timer);

	/* Render the framebuffer. */
	framebuffer_render(&l_framebuffer);

	/* Stop the timer. */
	timer_toggle(&l_timer);

	/* Obtain the time that it took between the timer's start and stop. */
	timer_obtain(&l_timer, &l_time);

	/* Print the amount of time that it took to render to the framebuffer. */
	fprintf(stderr, "debug: Took %jums to render to the framebuffer." ENDL, l_time);

	/* Define the output image file. */
	FILE *l_file = fopen("output.bmp", "wb");

	/* Ensure that the file was opened successfully. */
	if(l_file == NULL)
	{
		/* Print an error message. */
		fprintf(stderr, "Failed to open the output image file!" ENDL);

		/* Return with failure. */
		return -1;
	}

	/*
	 *	Start the timer, we'll be measuring the amount of time that it takes
	 *	to write the framebuffer's contents to the file.
	 */
	timer_toggle(&l_timer);

	/* Write the framebuffer's contents to the file. */
	framebuffer_fwrite(&l_framebuffer, l_file);

	/* Stop the timer. */
	timer_toggle(&l_timer);

	/* Obtain the time that it took between the timer's start and stop. */
	timer_obtain(&l_timer, &l_time);

	/* Print the amount of time that it took to write the framebuffer to the file. */
	fprintf(stderr, "debug: Took %jums to write the framebuffer to the file." ENDL, l_time);

	/* Destroy the framebuffer object. */
	framebuffer_delete(&l_framebuffer);

	/* Close the file. */
	fclose(l_file);

	/* Destroy the timer object. */
	timer_delete(&l_timer);

	return 0;
}