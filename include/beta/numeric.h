#pragma once

#include <stdint.h>

/* ... */
typedef float float_t;
typedef double double_t;

/* ... */
typedef union
{
	/* ... */
	struct
	{
		uintmax_t t_x;
		uintmax_t t_y;
	};

	/* ... */
	uintmax_t t_data[2];
} uintmax_vector2_t;

/* ... */
typedef union
{
	/* ... */
	struct
	{
		float_t t_x;
		float_t t_y;
	};

	/* ... */
	float_t t_data[2];
} float_vector2_t;

/* ... */
typedef union
{
	/* ... */
	struct
	{
		double_t t_x;
		double_t t_y;
	};

	/* ... */
	double_t t_data[2];
} double_vector2_t;

/* ... */
typedef union
{
	/* ... */
	struct
	{
		uintmax_t t_x;
		uintmax_t t_y;
		uintmax_t t_z;
	};

	/* ... */
	uintmax_t t_data[3];
} uintmax_vector3_t;

/* ... */
typedef union
{
	/* ... */
	struct
	{
		float_t t_x;
		float_t t_y;
		float_t t_z;
	};

	/* ... */
	float_t t_data[3];
} float_vector3_t;

/* ... */
typedef union
{
	/* ... */
	struct
	{
		double_t t_x;
		double_t t_y;
		double_t t_z;
	};

	/* ... */
	double_t t_data[3];
} double_vector3_t;