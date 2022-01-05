#include "cs225/PNG.h"
#include <iostream>
#include <stdlib.h>
#include <random>

cs225::PNG mandelbrot() {
  int Width = 640, Height = 480;
  cs225::PNG png(Width, Height);
 
  bool Targeting = 0;

  double x, y, X, Y, I, temp;
  double MI = 128; // Find optimal scaling with Zoom if desired
  double Zoom = 2; // Expressed as a scale of magnitude from the original default Zoom
  double ScaleWidth = 4.5; // The length/width of the Real-Imaginary plane that the set is calculated in
  double CenterxShift = -3/(4 * ScaleWidth);
  double xShift = CenterxShift, yShift = 0.0; // Expressed as a percentage of the width and height of the image
  // For a scale width of 4, the centering xShift is -3/(4 * 4) = -0.1875. For a scale width of 5, it is -3/(5 * 4) = -0.15
  // 4.5 is used as the scale width since it allows more to be on the screen while still being close to 4
  // The centering xShift is the xShift that results in the horizontal center of the screen
  // being along thevertical axis where the main cardioid meets the biggest circle

  int SmallerDim = Width < Height ? Width : Height; // Use for scaling to smaller dimension and maintaining circularity
  int XShift = SmallerDim == Width ? 0 : (Height - Width)/2;
  int YShift = SmallerDim == Height ? 0 : (Width - Height)/2;
  for(int i = 0; i < Height; i++)
    for(int j = 0; j < Width; j++) {
      // Implemented Zoom squaring, not sure if it fixed the Zoom magnitudal scaling problem
      X = (ScaleWidth * ((j + XShift) + xShift * Zoom * SmallerDim)/SmallerDim - ScaleWidth/2)/Zoom; // If Zoom commented it is non-constant with Zoom so
      Y = (ScaleWidth * ((i + YShift) + yShift * Zoom * SmallerDim)/SmallerDim - ScaleWidth/2)/Zoom; // that the user can pan more easily without flying off
      x = 0, y = 0;
      I = 0;
      while(x * x + y * y <= 4 && I < MI) {
        temp = x * x - y * y + X;
        y = 2 * x * y + Y;
        x = temp;
        I++;
      }
      cs225::HSLAPixel & pixel = png.getPixel(j, i);
      pixel.h = 300;
      pixel.s = 1;
      pixel.l = 1 - I/MI;
      pixel.a = 1;
    }

// Starfish: xShift = -0.2713397521875522, yShift = 0.20449150682899375
// Minibrot: xShift = -0.7, yShift = 0.0

//Targeting:
  if(Targeting) {
    for(int i = Width/2 - 1; i < Width/2 + 1; ++i)
      for(int j = 0; j < Height; ++j) {
        cs225::HSLAPixel & pixel = png.getPixel(i, j);
        pixel.h = 360 - pixel.h;
        pixel.s = 1;
        pixel.l = 1 - pixel.l;
        pixel.a = 1;
      }
    for(int j = Height/2 - 1; j < Height/2 + 1; ++j)
      for(int i = 0; i < Width; ++i) {
        if(i >= Width/2 - 1 && i <= Width/2)
          continue;
        cs225::HSLAPixel & pixel = png.getPixel(i, j);
        pixel.h = 360 - pixel.h;
        pixel.s = 1;
        pixel.l = 1 - pixel.l;
        pixel.a = 1;
      }
  }

// /* Verifying Circular Scaling */
// int r = 405;
// for(int i = Width/2 - r; i <= Width/2 + r; ++i)
//   for(int j = Height/2 - r; j <= Height/2 + r; ++j)
//     if((Width/2 - i) * (Width/2 - i) + (Height/2 - j) * (Height/2 - j) <= r * r) {
//       cs225::HSLAPixel & pixel = png.getPixel(i, j);
//       pixel.h = 360;
//     }

  return png;
}

int main() {
    mandelbrot().writeToFile("Mandelbrot.png");

    return 0;
}

// For shifting, move old values and only calculate new values?
// Or recalculate all

/*
      X = 2.47 * j/Width - 1.235;
      Y = 2.24 * i/Height - 1.12;
 */

/* Can you have MI not change anything other than the depth of generation? I.e. have all the 
   colors outside of the Brot stay the same? Perhaps rethink the way that I/MI is used for
   data?
 */

/* The outermost circle of the Brot isn't circular because one axis is scaled to (-2, 0.47)
   while the other is scaled to (-1.12, 1.12). Scaling both to either range would fix this
   issue, but this might be incorrect. However, scaling both to the same range does seem to
   make the subbrots look circular, whereas they are ovular with different scalings.
   Additionally, the outer circle should be a circle to be correct
 */
