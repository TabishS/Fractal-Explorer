#include "cs225/PNG.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

cs225::PNG mandelbrot() {
  int Width = 640, Height = 480;
  cs225::PNG png(Width, Height);
 
  bool Targeting = 0;

/*   double delta = 1.0/(8192 * 4096);
  double TargetY = 1.0/65536;
  int PosI, NegI; */

  int MI = 32; // Find optimal scaling with Zoom if desired
  double Zoom = 2; // Expressed as a scale of magnitude from the original default Zoom
  double n = 2;
  double ScaleWidth = 4.5; // The length/width of the Real-Imaginary plane that the set is calculated in
  // double CenterxShift = -3/(4 * ScaleWidth);
  double xShift = -0.0, yShift = -0.0; // Expressed as a percentage of the width and height of the image
  // For a scale width of 4, the centering xShift is -3/(4 * 4) = -0.1875. For a scale width of 5, it is -3/(5 * 4) = -0.15
  // 4.5 is used as the scale width since it allows more to be on the screen while still being close to 4
  // The centering xShift is the xShift that results in the horizontal center of the screen
  // being along the vertical axis where the main cardioid meets the biggest circle

  int SmallerDim = Width < Height ? Width : Height; // Use for scaling to smaller dimension and maintaining circularity
  int XShift = SmallerDim == Width ? 0 : (Height - Width)/2;
  int YShift = SmallerDim == Height ? 0 : (Width - Height)/2;
  for(int i = 0; i < Height; ++i)
    for(int j = 0; j < Width; ++j) {
      // Implemented Zoom squaring, not sure if it fixed the Zoom magnitudal scaling problem
      double X = (ScaleWidth * ((j + XShift) + xShift * Zoom * SmallerDim)/SmallerDim - ScaleWidth/2)/Zoom; // If Zoom commented it is non-constant with Zoom so
      double Y = (ScaleWidth * ((i + YShift) + yShift * Zoom * SmallerDim)/SmallerDim - ScaleWidth/2)/Zoom; // that the user can pan more easily without flying off
      // double x = 0, y = 0, x2plusy2 = 0, x2y2pown, narctan2yx;
      double x = -0.374004139, y = 0.659792175, X2plusY2 = X * X + Y * Y, X2Y2pown, narctan2YX;
      int I = 0;

      // /* Multibrot */
      // while(x2plusy2 <= 4 && I < MI) {
      //   narctan2yx = n * atan2(y, x);
      //   x2y2pown = pow((x2plusy2), (n/2));
      //   x = x2y2pown * cos(narctan2yx) + X;
      //   y = x2y2pown * sin(narctan2yx) + Y;
      //   x2plusy2 = x * x + y * y;
      //   ++I;
      // }
      
      // /* Multibrot Julia Set */ //
      while (X2plusY2 <= 4  &&  I < MI) {
        narctan2YX = n * atan2(Y, X);
        X2Y2pown = pow(X2plusY2, n/2);
        X = X2Y2pown * cos(narctan2YX) + x;
        Y = X2Y2pown * sin(narctan2YX) + y;
        X2plusY2 = X * X + Y * Y;
        ++I;
      }

      // /* Mandelbar */
      // while(x2plusy2 <= 4 && I < MI) {
      //   narctan2yx = n * atan2(y, x);
      //   x2y2pown = pow((x2plusy2), (n/2));
      //   x = x2y2pown * cos(narctan2yx) + X;
      //   y = -x2y2pown * sin(narctan2yx) + Y;
      //   x2plusy2 = x * x + y * y;
      //   ++I;
      // }

      // /* Burning Ship */
      // while(x2plusy2 <= 4 && I < MI) {
      //   narctan2yx = n * atan2(y, x);
      //   x2y2pown = pow((x2plusy2), (n/2));
      //   x = x2y2pown * cos(narctan2yx) + X;
      //   y = abs(x2y2pown * sin(narctan2yx)) + Y;
      //   x2plusy2 = x * x + y * y;
      //   ++I;
      // }

      /* Other Fractals */
      // ---

      cs225::HSLAPixel & pixel = png.getPixel(j, i);
      // /* Relative Colorization */
      // pixel.h = 300;
      // pixel.s = 1;
      // pixel.l = 1 - double(I)/MI;

      /* Modular Colorization */
      if(I == MI)
        pixel.l = 0;
      else {
        /* CGA 1 */
        switch(I % 4) {
          case 0 :
            pixel.h = 180;
            pixel.s = 1;
            pixel.l = 0.66;
            break;
          case 1 : 
            pixel.h = 300;
            pixel.s = 1;
            pixel.l = 0.67;
            break;
          case 2 : 
            pixel.l = 1;
            break;
          case 3 : 
            pixel.l = 0;
            break;
        }
      }

/*       if(Y <= (TargetY + delta) && Y >= (TargetY - delta)) {
        pixel.h = 360;
        pixel.l -= 0.1;
        PosI = i;
      }
      if(Y >= -(TargetY + delta) && Y <= -(TargetY - delta)) {
        pixel.h = 360;
        pixel.l -= 0.1;
        NegI = i;
      } */
    }

  // std::cout << PosI - NegI << '\n';


// Starfish: xShift = CenterxShift + 0.04173275077265841, yShift = 0.1428482553495056
// Seahorse Spiral: xShift = CenterxShift + 0.0028, yShift = 0.0463

//Targeting:
  if(Targeting) {
    for(int i = Width/2 - 1; i <= Width/2 + 1; ++i)
      for(int j = 0; j < Height; ++j) {
        cs225::HSLAPixel & pixel = png.getPixel(i, j);
        pixel.h = 360 - pixel.h;
        pixel.s = 1;
        pixel.l = 1 - pixel.l;
        pixel.a = 1;
      }
    for(int j = Height/2 - 1; j <= Height/2 + 1; ++j)
      for(int i = 0; i < Width; ++i) {
        if(i >= Width/2 - 1 && i <= Width/2 + 1)
          continue;
        cs225::HSLAPixel & pixel = png.getPixel(i, j);
        pixel.h = 360 - pixel.h;
        pixel.s = 1;
        pixel.l = 1 - pixel.l;
        pixel.a = 1;
      }
  }

  return png;
}

int main() {
    mandelbrot().writeToFile("Mandelbrot.png");

    return 0;
}

// Notes:

// For shifting, move old values and only calculate new values?
// Or recalculate all

/* Can you have MI not change anything other than the depth of generation? I.e. have all the 
   colors outside of the Brot stay the same? Perhaps rethink the way that I/MI is used for
   data?
 */