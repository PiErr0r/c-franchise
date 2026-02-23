#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define UNUSED(x) ((void)(x))
#define EPSILON 1e-6

#define GET_VALUE(data, y, x, X, n, N) (*((data) + (y)*(X)*(N) + (x)*(N) + (n)))
#define GET_INDEX(y, x, X, n, N) ((y)*(X)*(N) + (x)*(N) + (n))

#define INTENSITY " .'~^*?#"

#define P_BLACK()   printf("\033[30m")
#define P_RED()     printf("\033[31m")
#define P_GREEN()   printf("\033[32m")
#define P_YELLOW()  printf("\033[33m")
#define P_BLUE()    printf("\033[34m")
#define P_MAGENTA() printf("\033[35m")
#define P_CYAN()    printf("\033[36m")
#define P_WHITE()   printf("\033[37m")
#define P_RESET()   printf("\033[m")

/*
BG  FG  Color
30	40	Black
31	41	Red
32	42	Green
33	43	Yellow
34	44	Blue
35	45	Magenta
36	46	Cyan
37	47	White
*/

// Basic usage (see HDR discussion below for HDR usage):
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ...
//    // ... x = width, y = height, n = # 8-bit components per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    // ... but 'n' will always be the number that it would have been if you said 0
//    stbi_image_free(data);
//
// Standard parameters:
//    int *x                 -- outputs image width in pixels
//    int *y                 -- outputs image height in pixels
//    int *channels_in_file  -- outputs # of image components in image file
//    int desired_channels   -- if non-zero, # of image components requested in result


typedef struct {
    float hue, saturation, lightness;
} HSL;

float fmodf_pos(float v, float m) {
    v = fmodf(v, m) + m;
    return fmodf(v, m);
}

// _r, _g, _b = [0, 255]
HSL rgb2hsl(unsigned int _r, unsigned int _g, unsigned int _b) {
    float r = (float)_r / 255.f;
    float g = (float)_g / 255.f;
    float b = (float)_b / 255.f;
    float X_max = fmax(fmax(r, g), b);
    float X_min = fmin(fmin(r, g), b);
    float V = X_max;
    float C = X_max - X_min;
    float L = V - C * 0.5f;
    float Sv = fabsf(V) < EPSILON ? 0.f : (C/V);
    float Sl = (fabs(L) < EPSILON || fabsf(1 - L) < EPSILON) ? 0.f : 2*(V-L)/(1-fabsf(2*L-1));
    UNUSED(Sl);
    float Hp;
    if (fabsf(C) < EPSILON) {
        Hp = 0;
    } else if (fabsf(V - r) < EPSILON) {
        Hp = fmodf((g - b) / C, 6.f);
    } else if (fabsf(V - g) < EPSILON) {
        Hp = (b - r) / C + 2.f;
    } else if (fabsf(V - b) < EPSILON) {
        Hp = (r - g) / C + 4.f;
    } else {
        printf("Cannot be here! Aborting...\n");
        exit(2);
    }
    HSL hsl = { .hue = fmodf_pos(60 * Hp, 360.f), .saturation = Sv, .lightness = L };
    return hsl;
}

void usage(const char* name) {
    printf("[ERROR] Usage:\n");
    printf("\t%s [path-to-image]\n", name);
}

HSL get_hsl(int i, int j, unsigned char* data, int x, int y, int n, const int cell_w, const int cell_h) {
    if (n < 3) {
        // TODO();
        exit(3);
    }
    unsigned int r = 0, g = 0, b = 0;
    for (int ii = 0; ii < cell_h; ++ii) {
        for (int jj = 0; jj < cell_w; ++jj) {
            r += GET_VALUE(data, ii+i*cell_h, jj+j*cell_w, x, 0, n);
            g += GET_VALUE(data, ii+i*cell_h, jj+j*cell_w, x, 1, n);
            unsigned int bi = GET_INDEX(ii+i*cell_h, jj+j*cell_w, x, 2, n);
            b += (bi < x * y * n) ? GET_VALUE(data, ii+i*cell_h, jj+j*cell_w, x, 2, n) : 0;
        }
    }
    r /= (cell_w * cell_h);
    g /= (cell_w * cell_h);
    b /= (cell_w * cell_h);
    return rgb2hsl(r, g, b);
}

void resize_data(unsigned char* old, int x, int y, int n, HSL* new, int w, int h) {
    const int cell_w = x / w;
    const int cell_h = y / h;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            HSL hsl = get_hsl(i, j, old, x, y, n, cell_w, cell_h);
            *(new + i * w + j) = hsl;
        }
    }
}

char get_intensity_from_value(HSL hsl) {
    int I = 8.f * hsl.lightness;
    return INTENSITY[I];
}

unsigned int get_color_from_value(HSL hsl) {
    if (hsl.lightness < 0.1) return 0;
    else if (hsl.lightness > 0.9) return 7;
    else if (hsl.saturation < 0.3) {
        return hsl.lightness < 0.5 ? 0 : 7;
    } else {
        return 1 + (int)(hsl.hue / 60);
    }
    if (hsl.saturation < 0.45f && hsl.lightness > 0.55f) {
        return 7;
    } else if (hsl.saturation < 0.45f && hsl.lightness < 0.45f) {
        return 0;
    } else {
        return 1 + (int)(hsl.hue / 60);
    }
}

void print_color(unsigned int color) {
    switch (color) {
		case 0: P_BLACK();      break;
		case 1: P_RED();        break;
		case 2: P_YELLOW();     break;
		case 3: P_GREEN();      break;
		case 4: P_CYAN();       break;
		case 5: P_BLUE();       break;
		case 6: P_MAGENTA();    break;
		case 7: P_WHITE();      break;
        default: break;
    }
}

void print_ascii(HSL* data, unsigned char *edges, int w, int h) {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            unsigned int color = get_color_from_value(*(data + i * w + j));
            print_color(color);
            char c = get_intensity_from_value(*(data + i * w + j));
            if (*(edges + i * w + j) != ' ') {
                c = *(edges + i * w + j);
            }
            printf("%c", c);
            P_RESET();
        }
        printf("\n");
    }
}


void print_hsl(HSL hsl) {
    printf("{ .hue = %f, .saturation = %f, .lightness = %f }\n", hsl.hue, hsl.saturation, hsl.lightness);
}

unsigned char get_grayscale_value(int i, int j, unsigned char* data, int x, int y, int n, const int cell_w, const int cell_h) {
    UNUSED(y);
    unsigned int result = 0;
    for (int ii = 0; ii < cell_h; ++ii) {
        for (int jj = 0; jj < cell_w; ++jj) {
            unsigned char r = GET_VALUE(data, i*cell_h+ii, j*cell_w+jj, x, 0, n);
            unsigned char g = GET_VALUE(data, i*cell_h+ii, j*cell_w+jj, x, 1, n);
            unsigned char b = GET_VALUE(data, i*cell_h+ii, j*cell_w+jj, x, 2, n);
            result += (r + g + b) / 3;
        }
    }
    return result / (cell_w * cell_h);
}

void resize_grayscale(unsigned char* old, int x, int y, int n, unsigned char* new, int w, int h) {
    int cell_h = y / h;
    int cell_w = x / w;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            unsigned char value = get_grayscale_value(i, j, old, x, y, n, cell_w, cell_h);
            *(new + i * w + j) = value;
        }
    }
}

char get_gs_value(unsigned char c) {
    int DIV = 256 / 8;
    return INTENSITY[c / DIV];
}

void print_gs_ascii(unsigned char* data, int w, int h) {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            char c = get_gs_value(*(data + i * w + j));
            printf("%c", c);
        }
        printf("\n");
    }
}

void print_char_ascii(unsigned char* data, int w, int h) {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            printf("%c", *(data + i * w + j));
        }
        printf("\n");
    }
}

int conv_step3x3(int i, int j, unsigned char* A, int w, int* B) {
    int res = 0;
    for (int di = -1; di < 2; ++di) {
        for (int dj = -1; dj < 2; ++dj) {
            int v = *(A + (i + di) * w + (j + dj)) * *(B + (1 - di) * 3 + (1 + dj));
            res += v;
        }
    }
    return res;
}

// B = 3x3
void convolution(unsigned char* A, int w, int h, int* B, int* out) {
    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            int v = conv_step3x3(i, j, A, w, B);
            *(out + i * w + j) = v;
        }
    }
}

void sobel(unsigned char* old, unsigned char* new, int w, int h) {
    int Gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int* X = malloc(w * h * sizeof(int));
    int* Y = malloc(w * h * sizeof(int));
    int T = 240;
    convolution(old, w, h, Gx, X);
    convolution(old, w, h, Gy, Y);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int idx = i * w + j;
            float v = X[idx] * X[idx] + Y[idx] * Y[idx];
            v = sqrtf(v);
            if (abs(X[idx]) > T && abs(Y[idx]) > T) {
                new[idx] = '/';
            } else if (abs(X[idx]) > T) {
                new[idx] = '|';
            } else if (abs(Y[idx]) > T) {
                new[idx] = '_';
            } else {
                new[idx] = ' ';
            }
        }
    }
}

int main(int argc, const char** argv)
{
    if (argc != 2) {
        usage(argv[0]);
        exit(1);
    }

    int x, y, n;
    unsigned char *data = stbi_load(argv[1], &x, &y, &n, 0);

    int W = 120;
    int H = y * (float)W / (float)x / 2.f;
    HSL* new_data = malloc(W * H * sizeof(HSL));

    unsigned char *gs = malloc(W * H * sizeof(char));
    resize_grayscale(data, x, y, n, gs, W, H);
    unsigned char *edges = malloc(W * H * sizeof(char));
    memset(edges, 0, W * H);
    sobel(gs, edges, W, H);
    resize_data(data, x, y, n, new_data, W, H);
    print_ascii(new_data, edges, W, H);
    // print_char_ascii(edges, W, H);

    return 0;
}
