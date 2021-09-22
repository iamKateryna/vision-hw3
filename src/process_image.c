#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

//повертаємо піксель, проходимось по межах зображення
float get_pixel(image im, int x, int y, int c) {
    // TODO Fill this in
    if (x < 0) x = 0;
    if (x > im.w) x = im.w - 1;

    if (y < 0) y = 0;
    if (y > im.h) y = im.h - 1;

    if (c < 0) c = 0;
    if (c > im.c) c = im.c - 1;

    int pixel = (im.w * im.h * c) + (im.w * y) + x; //28:40 - 03
    return im.data[pixel];
}

//задаємо піксель, при некоректних даних нічого не відбувається
void set_pixel(image im, int x, int y, int c, float v) {
    // TODO Fill this in
    if (x < 0 || x >= im.w) {
        return;
    } else if (y < 0 || y >= im.h) {
        return;
    } else if (c < 0 || c >= im.c) {
        return;
    }
    int index = (im.h * im.w * c) + (im.w * y) + x;
    im.data[index] = v;
}

//створюємо нову картинку з параметрами вихідної, проходимось по усім пікселям, копіюємо їх
image copy_image(image im) {
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    //for(int i = 0; i < im.w*im.h*im.c; i++){
    //    copy.data[i] = im.data[i];
    //}
    int size = im.w*im.h*im.c;
    memcpy(copy.data, im.data, sizeof(float)*size);
    return copy;
}


//проходимось по усім пікселям, використовуючи luma calculation отримуємо відтінок сірого
image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(int j=0; j < im.h; j++){
        for(int i=0; i < im.w; i++){
            float gray_pixel = 0.0;
            float weighted_sum[3] = {0.299, 0.587, 0.114};
            for (int c=0; c < 3; c++) gray_pixel += get_pixel(im, i, j, c) * weighted_sum[c];
            set_pixel(gray, i, j, 0, gray_pixel);
        }
    }
    return gray;
}

//додаємо значення v - value до кожного пікселя на зображенні, щоб "зсунути" колір
void shift_image(image im, int c, float v) {
    // TODO Fill this in
    for (int j = 0; j < im.h; j++){
        for (int i = 0; i < im.w; i++) {
            float pixel = get_pixel(im, i,j,c);
            set_pixel(im, i, j, c, pixel + v);
        }
    }
}

//фіксимо занадто сильний зсув обмеженнями
void clamp_image(image im) {
    // TODO Fill this in
    for(int i=0; i < im.w*im.h*im.c; i++){
        if (im.data[i]>1) im.data[i] =1;
        if (im.data[i]<0) im.data[i]=0;
    }
}


// These might be handy
float three_way_max(float a, float b, float c) {
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c) {
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

//переходимо з rgb до hsv перетвореннями, отримуємо значення пікселів, рахуємо value, saturation
//потім визначаємо hue
void rgb_to_hsv(image im) {
    // TODO Fill this in
    float r, g, b, h, s, v, m, c, h_primary;

    for(int i = 0; i < im.w; i++){
        for(int j = 0; j < im.h; j++){
            r = get_pixel(im, i, j, 0);
            g = get_pixel(im, i, j, 1);
            b = get_pixel(im, i, j, 2);

            v = three_way_max(r, g, b);
            m = three_way_min(r, g, b);

            c = v - m;

            if (  c != 0.0 ) {
                s = c / v;
                if (v == r){
                    h_primary = (g -b)/c;
                } else if (v == g){
                    h_primary = (b - r)/c +2;
                }else{
                    h_primary = (r - g)/c +4;
                }
                if(h_primary < 0){
                    h = (h_primary/6) + 1;
                } else {
                    h = (h_primary/6);
                }
            } else {
                s = 0;
                h = 0;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

// і навпаки, зворотніми перетвореннями, спочатку отримуємо значення пікселів в hsv, потім
// за hue визначаемо кольори
void hsv_to_rgb(image im) {
    // TODO Fill this in
    float h, s, v, r, g, b;
    for(int i = 0; i < im.w; i++){
        for(int j = 0; j < im.h; j++){
            h = get_pixel(im, i, j, 0);
            s = get_pixel(im, i, j, 1);
            v = get_pixel(im, i, j, 2);

            float c = s * v;
            float m = v - c;

            float h_temp = h * 6;
            //h_temp = round(h_temp);

            if(c == 0){
                r = v;
                g = v;
                b = v;
            } else if (h_temp >= 5 && h_temp < 6) {
                r = v;
                g = m;
                b = ((((h_temp /  6) - 1) * 6 * c) - g) * -1;
            } else if (h_temp < 5 && h_temp >= 4) {
                g = m;
                r = (h_temp - 4) * c + g;
                b = v;
            } else if (h_temp < 4 && h_temp >= 3) {
                r = m;
                g = (((h_temp - 4) * c) - r) * -1;
                b = v;
            } else if (h_temp < 3 && h_temp >= 2) {
                r = m;
                g = v;
                b = ((h_temp - 2) * c) + r;
            } else if (h_temp < 2 && h_temp >= 1) {
                g = v;
                b = m;
                r = (((h_temp - 2) * c) - b) * -1;
            } else if (h_temp < 1 && h_temp > 0) {
                r = v;
                b = m;
                g = (h_temp * c) + b;
            } else {
                r = v;
                g = m;
                b = m;
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}
