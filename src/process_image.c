#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"



float get_pixel(image im, int x, int y, int c)
{  //padding
      if(x>255) x=255;
else if(x<0) x=0;
if(y<0) y=0;
else if(y>255) y=255;

return im.data[x + (im.w)*y + (im.w)*(im.h)*c];
    
   
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if((x<255)||(x<0)||(y<0)||(y>255)){
        return;
    }
    else{
        im.data[x + (im.w)*y + (im.w)*(im.h)*c] = v;

    }
   
    
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
  //  memcpy(copy,im);
    
    for(int i=0; i<im.c; i++){
        for(int j=0; j<im.h; j++){
            for(int k=0; k<im.w; k++){
                copy.data[k + (copy.w)*j + (copy.w)*(copy.h)*i] = im.data[k + (im.w)*j + (im.w)*(im.h)*i];
            }
        }
    }

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    image copy= copy_image(im);
    
        for(int j=0; j<im.h; j++){
            for(int k=0; k<im.w; k++){
                gray.data[k + (copy.w)*j] = 0.299*im.data[k + (im.w)*j] + 0.587*im.data[k + (im.w)*j +(im.w)*(im.h)] + 0.114*im.data[k + (im.w)*j + (im.w)*(im.h)*2];
            }
        }
    
    return gray;
}

void shift_image(image im, int c, float v)
{
    
        for(int j=0; j<im.h; j++){
            for(int k=0; k<im.w; k++){
                im.data[k + (im.w)*j + (im.w)*(im.h)*c] += v;
            }
        }
    
}

void clamp_image(image im)
{
   for(int i=0; i<im.c; i++){
        for(int j=0; j<im.h; j++){
            for(int k=0; k<im.w; k++){
                if(im.data[k + (im.w)*j + (im.w)*(im.h)*i] <0) im.data[k + (im.w)*j + (im.w)*(im.h)*i]=0;
                else if(im.data[k + (im.w)*j + (im.w)*(im.h)*i]>255) im.data[k + (im.w)*j + (im.w)*(im.h)*i]=255;
                else continue;
            }
        }
    }

}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{ //calculating value
for(int i=0; i<im.h; i++){
    for(int j=0; j<im.w; j++){

    float V=three_way_max(im.data[j+ i*im.w], im.data[j+i*im.w+im.h*im.w], im.data[j+i*im.w+2*im.h*im.w]);
    float m=three_way_min(im.data[j+ i*im.w], im.data[j+i*im.w+im.h*im.w], im.data[j+i*im.w+2*im.h*im.w]);

//calculating saturation
    float S;
    if(im.data[j+ i*im.w] || im.data[j+i*im.w+im.h*im.w] || im.data[j+i*im.w+2*im.h*im.w]){  S = (V-m)/V;}
    else{  S = 0;}

//calculating hue
float H;
    if(V==m){ H = 0;}
    else if(V-m!=0){
        if(V==im.data[j+ i*im.w]){
            float H1 = (im.data[j+i*im.w+im.h*im.w]-im.data[j+i*im.w+2*im.h*im.w])/(V-m);
            if(H1<0){  H = H1/6 + 1;}
            else{  H = H1/6;}

        }
        else if(V == im.data[j+i*im.w+im.h*im.w]){
            float H1 = (im.data[j+i*im.w+2*im.h*im.w]-im.data[j+ i*im.w])/(V-m) + 2;
            if(H1<0){ H = H1/6 + 1;}
            else{  H = H1/6;}
        }
        else if(V== im.data[j+i*im.w+2*im.h*im.w]){
            float H1 = (im.data[j+ i*im.w]-im.data[j+i*im.w+im.h*im.w])/(V-m) + 4;
            if(H1<0){  H = H1/6 + 1;}
            else{  H = H1/6;}
        }
    }
    im.data[j+ i*im.w]=H;
    im.data[j+ i*im.w+ im.h*im.w]=S;
    im.data[j+ i*im.w+ 2*im.h*im.w]=V;


    }
}
    

}

void hsv_to_rgb(image im)
{
    for(int i=0; i<im.h; i++){
      for(int j=0; j<im.w; j++){
        float R,G,B;

        im.data[j+ i*im.w]=6*im.data[j+ i*im.w];
        int p=floor(im.data[j+ i*im.w]);
        float F=im.data[j+ i*im.w]-p;
        float m=im.data[j+ i*im.w+ 2*im.h*im.w]*(1-im.data[j+ i*im.w+ im.h*im.w]);
        float n=im.data[j+ i*im.w+ 2*im.h*im.w]*(1-im.data[j+ i*im.w+ im.h*im.w]*F);
        float k=im.data[j+ i*im.w+ 2*im.h*im.w]*(1-im.data[j+ i*im.w+ im.h*im.w]*(1-F));
        switch(p){
            case 0: R=im.data[j+ i*im.w+ 2*im.h*im.w];
                    G=k;
                    B=m;
            case 1: R=n;
                    G=im.data[j+ i*im.w+ 2*im.h*im.w];
                    B=m;
            case 2: R=m;
                    G=im.data[j+ i*im.w+ 2*im.h*im.w];
                    B=k;
            case 3: R=m;
                    G=n;
                    B=im.data[j+ i*im.w+ 2*im.h*im.w];
            case 4: R=k;
                    G=m;
                    B=im.data[j+ i*im.w+ 2*im.h*im.w];
            case 5: R=im.data[j+ i*im.w+ 2*im.h*im.w];
                    G=m;
                    B=n;



        }
    im.data[j+ i*im.w]=R;
    im.data[j+ i*im.w+ im.h*im.w]=G;
    im.data[j+ i*im.w+ 2*im.h*im.w]=B;


      }
    }
    
}
