#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x)
#define TODO(message) do { fprintf(stderr, "%s:%d TODO: %s\n", __FILE__, __LINE__, message);} while (0)
#define UNREACHABLE(message) do { fprintf(stderr, "%s:%d UNREACHABLE: %s\n", __FILE__, __LINE__, message); abort(); } while (0)
#define da_realloc(da)\
            (da)->items = \
                realloc((da)->items, (da)->capacity * sizeof(*(da)->items))

#define DA_INIT_CAPACITY 256
#define da_append(da, item)\
    do {\
        if ((da)->capacity == 0) {\
            (da)->capacity = DA_INIT_CAPACITY;\
        } else if ((da)->count + 1 > (da)->capacity) {\
            (da)->capacity *= 2;\
        }\
        da_realloc(da);\
        (da)->items[(da)->count++] = (item);\
    } while (0)

#define da_last(da) (da)->items[(da)->count-1]
#define da_pop(da)\
    do {\
        (da)->count--;\
        if ((da)->count * 2 <= (da)->capacity) {\
            (da)->capacity /= 2;\
            da_realloc(da);\
        }\
    } while (0)

#define da_remove(da, idx)\
    do {\
        for (size_t i = idx; i < (da)->count; i++) {\
            (da)->items[(idx)] = (da)->items[(idx) + 1];\
        }\
        (da)->count--;\
    } while (0)

#define da_foreach(da, Type, it)\
    for (Type *it = (da)->items; it < (da)->items + (da)->count; ++it)

#define C_N 20
Color COLORS[C_N] = {
//	LIGHTGRAY, used for DYING
	GRAY,
	DARKGRAY,
//	YELLOW, used for DYING
	GOLD,
	ORANGE,
	PINK,
	RED,
	MAROON,
	GREEN,
	LIME,
	DARKGREEN,
	SKYBLUE,
	BLUE,
	DARKBLUE,
	PURPLE,
	VIOLET,
	DARKPURPLE,
	BEIGE,
	BROWN,
	DARKBROWN,
	MAGENTA
};

