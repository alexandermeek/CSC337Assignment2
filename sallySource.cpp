#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

unsigned int DIM = 3;
unsigned int X = 0, Y = 1, Z = 2;
unsigned int SQUARED = 2;
unsigned int DEFAULT_SIZE = 128, DEFAULT_TIME = 1;

void gen_sally(int xs, int ys, int zs, int time, float *sally )
/*
 *  Gen_Sally creates a vector field of dimension [xs,ys,zs,3] from
 *  a proceedural function. By passing in different time arguements,
 *  a slightly different and rotating field is created.
 *
 *  The magnitude of the vector field is highest at some funnel shape
 *  and values range from 0.0 to around 0.4 (I think).
 *
 *  I just wrote these comments, 8 years after I wrote the function.
 *  
 *  Developed by Sally of Sally University
 *
 */
{
  float x, y, z;
  int ix, iy, iz;
  float r, xc, yc, scale, temp, z0;
  float r2 = 8;
  float SMALL = 0.00000000001;
  float xdelta = 1.0 / (xs-1.0);
  float ydelta = 1.0 / (ys-1.0);
  float zdelta = 1.0 / (zs-1.0);

  for( iz = 0; iz < zs; iz++ )
  {
     z = iz * zdelta;                        // map z to 0->1
     xc = 0.5 + 0.1*sin(0.04*time+10.0*z);   // For each z-slice, determine the spiral circle.
     yc = 0.5 + 0.1*cos(0.03*time+3.0*z);    //    (xc,yc) determine the center of the circle.
     r = 0.1 + 0.4 * z*z + 0.1 * z * sin(8.0*z); //  The radius also changes at each z-slice.
     r2 = 0.2 + 0.1*z;                           //    r is the center radius, r2 is for damping
     for( iy = 0; iy < ys; iy++ )
     {
		y = iy * ydelta;
		for( ix = 0; ix < xs; ix++ )
		{
			x = ix * xdelta;
			temp = sqrt( (y-yc)*(y-yc) + (x-xc)*(x-xc) );
			scale = fabs( r - temp );
/*
 *  I do not like this next line. It produces a discontinuity 
 *  in the magnitude. Fix it later.
 *
 */
           if ( scale > r2 )
              scale = 0.8 - scale;
           else
              scale = 1.0;
			z0 = 0.1 * (0.1 - temp*z );
		   if ( z0 < 0.0 )  z0 = 0.0;
		   temp = sqrt( temp*temp + z0*z0 );
			scale = (r + r2 - temp) * scale / (temp + SMALL);
			scale = scale / (1+z);
           *sally++ = scale * (y-yc) + 0.1*(x-xc);
           *sally++ = scale * -(x-xc) + 0.1*(y-yc);
           *sally++ = scale * z0;
		}
     }
  }
}

int main(int argc, char* argv[]) {
   unsigned int size, time;

   if (argv[1] != nullptr) {
        size = std::stoi(argv[1]);
        time = std::stoi(argv[2]);
    } 
    else {
        size = DEFAULT_SIZE;
        time = DEFAULT_TIME;
    }

   float *sally = new float[size * size * size * DIM];

   gen_sally(size, size, size, time, sally);

   //Save sally as csv
   std::ofstream file;
	file.open("sally.csv");
   file << "x-coord,y-coord,z-coord,x-dir,y-dir,z-dir,mag" << std::endl; 
   for (int z = 0; z < size; z++) {
      for (int y = 0; y < size; y++) {
         for (int x = 0; x < size; x++) {
            float magnitude = std::sqrt(std::pow(sally[(x + size * (y + size * z)) * DIM + X], SQUARED) 
                                      + std::pow(sally[(x + size * (y + size * z)) * DIM + Y], SQUARED)
                                      + std::pow(sally[(x + size * (y + size * z)) * DIM + Z], SQUARED));
            //Add new entry
            file << x << ","
                 << y << ","
                 << z << ","
                 << sally[(x + size * (y + size * z)) * DIM + X] << ","
                 << sally[(x + size * (y + size * z)) * DIM + Y] << ","
                 << sally[(x + size * (y + size * z)) * DIM + Z] << ","
                 << magnitude << std::endl;
         }
      }
   }

   //Finsh and cleanup
   delete [] sally;
   std::cout << "Done!";
   file.close();
}