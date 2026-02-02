#include <stdio.h>
#include <stdlib.h>

#define da_realloc(da)\
            (da)->items = \
                realloc((da)->items, sizeof((da)->items[0]) * (da)->capacity)

#define DA_INIT_CAPACITY 256
#define da_append(da, item)\
    do {\
        if ((da)->capacity == 0) {\
            (da)->items = malloc(DA_INIT_CAPACITY * sizeof((da)->items));\
            (da)->capacity = DA_INIT_CAPACITY;\
        } else if ((da)->count + 1 > (da)->capacity) {\
            (da)->capacity *= 2;\
            da_realloc(da);\
        }\
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

typedef struct {
    int *items;
    int capacity;
    int count;
} Nums;

int main(void) {
    Nums nums = {0};
    printf("Hello, world\n");
    for (int i = 0; i < 300; ++i) {
        da_append(&nums, 10 - i);
    }
    da_foreach(&nums, int, tmp) {
        printf("%d \n", *tmp);
    }

    return 0;
}

