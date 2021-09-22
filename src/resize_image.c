#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, roundf(x), roundf(y), c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image new_image = make_image(w, h, im.c);
    float ax = ((float) im.w)/ w;
    double bx = 0.5 * ax - 0.5;
    float ay = ((float) im.h)/ h;
    double by = 0.5 * ay - 0.5;

    for (int i = 0; i < new_image.w; i++){
        for (int j = 0; j < new_image.h; j++){
            for (int k = 0; k < new_image.c; k ++){

                float scaled_x = i * ax + bx;
                float scaled_y = j * ay + by;
                float value = nn_interpolate(im, scaled_x, scaled_y, k);
                set_pixel(new_image, i, j, k, value);
            }
        }
    }
    return new_image;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    float d1 = x - ceilf(x);
    float d2 = floorf(x) - x;
    float d3 = y - ceil(y);
    float d4 = floorf(y) - y;

    float v1 = get_pixel(im, ceilf(x), ceilf(y), c);
    float v2 = get_pixel(im , floorf(x), ceilf(y), c);
    float v3 = get_pixel(im, ceilf(x), floorf(y), c);
    float v4 = get_pixel(im , floorf(x), floorf(y), c);

    float q1 = v1*d2 + v2*d1;
    float q2 = v3*d2 + v4*d1;
    return q1*d4 + q2*d3;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image new_image = make_image(w, h, im.c);
    float ax = ((float) im.w)/ w;
    float bx = 0.5 * ax - 0.5;
    float ay = ((float) im.h)/ h;
    float by = 0.5 * ay - 0.5;

    for (int i = 0; i < new_image.w; i++){
        for (int j = 0; j < new_image.h; j++){
            for (int k = 0; k < new_image.c; k ++){

                float scaled_x = i * ax + bx;
                float scaled_y = j * ay + by;
                float value = bilinear_interpolate(im, scaled_x, scaled_y, k);
                set_pixel(new_image, i, j, k, value);
            }
        }
    }
    return new_image;
}

