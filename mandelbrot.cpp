#include "cs225/PNG.h"
#include <iostream>
#include <stdlib.h>
#include <random>

cs225::PNG mandelbrot() {
  int Width = 640, Height = 480;
  cs225::PNG png(Width, Height);
 
  bool Targeting = 0;

/*   double delta = 1.0/(8192 * 4096);
  double TargetY = 1.0/65536;
  int PosI, NegI; */

  int MI = 128; // Find optimal scaling with Zoom if desired
  double Zoom = 2; // Expressed as a scale of magnitude from the original default Zoom
  double ScaleWidth = 4.5; // The length/width of the Real-Imaginary plane that the set is calculated in
  double CenterxShift = -3/(4 * ScaleWidth);
  double xShift = CenterxShift, yShift = 0.0; // Expressed as a percentage of the width and height of the image
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
      double x = 0, y = 0, x2 = 0, y2 = 0;
      int I = 0;

      while(x2 + y2 <= 4 && I < MI) {
        y = 2 * x * y + Y;
        x = x2 - y2 + X;
        x2 = x * x;
        y2 = y * y;
        ++I;
      }

      cs225::HSLAPixel & pixel = png.getPixel(j, i);
      pixel.h = 300;
      pixel.s = 1;
      pixel.l = 1 - double(I)/MI;
      pixel.a = 1;
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