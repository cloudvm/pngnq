
#include <math.h>
#include <unistd.h>
#include <stdio.h>

#include "colorspace.h"


void rgb2XYZ(const color_rgb *rgb, color_XYZ *xyz, const color_XYZ *wp)
{
  
  /* This is for sRGB D65 whitepoint */
  float m[3][3] ={{0.412424,    0.212656,    0.0193324},  
		  {0.357579,    0.715158,    0.119193},   
		  {0.180464,    0.0721856,   0.950444}};

  xyz->X = (m[0][0]*rgb->r + m[1][0]*rgb->g + m[2][0]*rgb->b)/256.0;
  xyz->Y = (m[0][1]*rgb->r + m[1][1]*rgb->g + m[2][1]*rgb->b)/256.0;
  xyz->Z = (m[0][2]*rgb->r + m[1][2]*rgb->g + m[2][2]*rgb->b)/256.0;

}


void XYZ2LUV(color_XYZ *xyz, color_LUV *luv, const color_XYZ *wp)
{
  float u,v;
  float yref,uref,vref;

  const float e = 216.0/24389.0;
  const float k = 24389.0/27.0;
  
  if(!wp){
    wp = &d65;
  }
  /* Reference white point */
  uref = 4.0 * wp->X/(wp->X + 15.0 * wp->Y +3.0 * wp->Z); 
  vref = 9.0 * wp->Y/(wp->X + 15.0 * wp->Y +3.0 * wp->Z); 
  yref = xyz->Y/wp->Y;

  /* Calculate LUV */
  u = 4.0 * xyz->X/(xyz->X + 15.0 * xyz->Y + 3.0 * xyz->Z);
  v = 9.0 * xyz->Y/(xyz->X + 15.0 * xyz->Y + 3.0 * xyz->Z);
  
  if(yref>e){
    luv->L = 116.0*powf(yref,1.0/3.0)-16.0; 
  }else{
    luv->L = k*yref;
  }

  luv->U = 13.0*luv->L*(u-uref);
  luv->V = 13.0*luv->L*(v-vref);

}

void rgb2LUV(const color_rgb *rgb, color_LUV *luv, const color_XYZ *wp)
{
  color_XYZ xyz;
  rgb2XYZ(rgb,&xyz,0);
  XYZ2LUV(&xyz,luv,0);
}


/* int  main(int argv, char **argc){
  int r,g,b;
  color_rgb RGB;
  color_XYZ XYZ;
  color_LUV LUV;
  color_LUV LUV2;

  for(r=0;r<255;r+=16){
    for(g=0;g<255;g+=16){
      for(b=0;b<255;b+=16){
	RGB.r=r;
	RGB.g=g;
	RGB.b=b;

	rgb2XYZ(&RGB, &XYZ, 0);
	XYZ2LUV(&XYZ, &LUV, 0);

	printf("(%d, %d, %d)\t(%f,%f,%f)\t(%f,%f,%f)\n",
	       RGB.r,RGB.g,RGB.b,XYZ.X,XYZ.Y,XYZ.Z,LUV.L,LUV.U,LUV.V);  

	rgb2LUV(&RGB, &LUV2, 0);

      }
    }
  }
  return(0);
  } */

