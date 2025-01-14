#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <string.h>

#include <beta/numeric.h>
#include <beta/framebuffer.h>

int (*framebuffer_render)(framebuffer_t *pt_framebuffer) = NULL;

int framebuffer_create(framebuffer_t *pt_framebuffer)
{
	/* Ensure that the object is valid. */
	if(pt_framebuffer == NULL) return -1;

	/* Initialize the data to zero. */
	pt_framebuffer->t_data = NULL;
	pt_framebuffer->t_resolution = (uintmax_vector2_t){0, 0};

	/*
	 *	Set the initialization state to true, and the allocation state to
	 *	false.
	 */
	pt_framebuffer->t_settings |= 0b00000001;
	pt_framebuffer->t_settings &= 0b11111101;

	/* Return with success. */
	return 0;
}

int framebuffer_resize(framebuffer_t *pt_framebuffer, uintmax_vector2_t t_resolution)
{
	/* Ensure that the object is valid. */
	if(pt_framebuffer == NULL) return -1;

	/* Ensure that the framebuffer is initialized. */
	if(!(pt_framebuffer->t_settings & 0b00000001)) return -2;

	/*
	 *	Ensure that the resolution is valid. If it isn't valid, then check if
	 *	the allocation state is enabled. If it is enabled, then disable it,
	 *	and free the data.
	 */
	if(t_resolution.t_x == 0 || t_resolution.t_y == 0)
	{
		/* Set the resolution to zero. */
		pt_framebuffer->t_resolution = (uintmax_vector2_t){0, 0};

		/* Check if the allocation state is enabled. */
		if(pt_framebuffer->t_settings & 0b00000010)
		{
			/* Disable the allocation state. */
			pt_framebuffer->t_settings &= 0b11111101;

			/* Free the data. */
			free(pt_framebuffer->t_data);
			pt_framebuffer->t_data = NULL;

			/* Return with success. */
			return 0;
		}
	}

	/* Set the resolution. */
	pt_framebuffer->t_resolution = t_resolution;

	/* Re-allocate the data. */
	pt_framebuffer->t_data = realloc(pt_framebuffer->t_data, t_resolution.t_x * t_resolution.t_y * (sizeof(uint8_t) * 3));

	/* Ensure the allocation was successful. */
	if(pt_framebuffer->t_data == NULL) return -4;

	/* Enable the allocation state, if it isn't already enabled. */
	if(!(pt_framebuffer->t_settings & 0b00000010)) pt_framebuffer->t_settings |= 0b00000010;

	/* Return with success. */
	return 0;
}

int framebuffer_fwrite(framebuffer_t *pt_framebuffer, FILE *pt_file)
{
    /* Ensure that the object is valid. */
    if(pt_framebuffer == NULL || pt_file == NULL) return -1;

    /* Ensure that the framebuffer is initialized and allocated. */
    if(!(pt_framebuffer->t_settings & 0b00000001)) return -2;
    if(!(pt_framebuffer->t_settings & 0b00000010)) return -3;

	/* Calculate total image size. */
    uintmax_t l_padding = (4 - ((pt_framebuffer->t_resolution.t_x * 3) % 4)) % 4;
    uintmax_t l_row_size = pt_framebuffer->t_resolution.t_x * 3 + l_padding;
    uintmax_t l_image_size = l_row_size * pt_framebuffer->t_resolution.t_y;

	/* Define the BMP header. */
	uint8_t l_header[54] = {
		/* The BMP signature. */
		'B', 'M',

		/* The file size. */
		0, 0, 0, 0,

		/* Reserved data. */
		0, 0, 0, 0,

		/* The pixel data offset. */
		54, 0, 0, 0,
		
		/* The DIB header size. */
		40, 0, 0, 0,
		
		/* The width & height. */
		0, 0, 0, 0,
		0, 0, 0, 0,

		/* The color planes. */
		1, 0,

		/* The bits per pixel. */
		24, 0,

		/* The compression method. */
		0, 0, 0, 0,

		/* The image size. */
		0, 0, 0, 0,

		/* The amount of horizontal pixels per meter. */
		0, 0, 0, 0,

		/* The amount of vertical pixels per meter. */
		0, 0, 0, 0,

		/* The colors in the palette. */
		0, 0, 0, 0,

		/* The important colors. */
		0, 0, 0, 0
	};

	/* Set the size value(s). */
	uint32_t l_file_size = 54 + l_image_size;
    memcpy(l_header + 2, &l_file_size, 4);
    memcpy(l_header + 18, &pt_framebuffer->t_resolution.t_x, 4);
    memcpy(l_header + 22, &pt_framebuffer->t_resolution.t_y, 4);
    memcpy(l_header + 34, &l_image_size, 4);

	/* Write the header to the file. If it fails, then return with failure. */
    if(fwrite(l_header, 1, 54, pt_file) != 54) return -4;

	/* Write the pixel data row-by-row, from bottom to top. */
	uint8_t l_pad[3] = {0, 0, 0};
	for(intmax_t l_y = pt_framebuffer->t_resolution.t_y - 1; l_y >= 0; l_y--)
	{
		for(uint32_t l_x = 0; l_x < pt_framebuffer->t_resolution.t_x; l_x++)
		{
			/* Calculate the pixel offset. */
			uintmax_t l_pixel_offset = (l_y * pt_framebuffer->t_resolution.t_x + l_x) * 3;

			/* Form the pixel. */
			uint8_t l_pixel[3] = {
				pt_framebuffer->t_data[l_pixel_offset + 2],
				pt_framebuffer->t_data[l_pixel_offset + 1],
				pt_framebuffer->t_data[l_pixel_offset]
			};

			/* Write the pixel to the file. */
			if(fwrite(l_pixel, 1, 3, pt_file) != 3) return -5;
		}

		/* Write padding. */
		if(l_padding > 0) if(fwrite(l_pad, 1, l_padding, pt_file) != l_padding) return -6;
	}

	/* Return with success. */
	return 0;
}

int framebuffer_delete(framebuffer_t *pt_framebuffer)
{
	/* Ensure that the object is valid. */
	if(pt_framebuffer == NULL) return -1;

	/* Ensure that the framebuffer is initialized. */
	if(!(pt_framebuffer->t_settings & 0b00000001)) return -2;

	/* Initialize the resolution to zero. */
	pt_framebuffer->t_resolution = (uintmax_vector2_t){0, 0};

	/*
	 *	If the allocation state is enabled, then free the data & disable the
	 *	allocation state.
	 */
	if(pt_framebuffer->t_settings & 0b00000010)
	{
		/* Free that memory. */
		free(pt_framebuffer->t_data);

		/* Disable the allocation state. */
		pt_framebuffer->t_settings &= 0b11111101;
	}

	/*
	 *	Initialize the settings to their default value(s), for possible
	 *	re-use.
	 */
	pt_framebuffer->t_settings = 0b00000000;

	/* Return with success. */
	return 0;
}