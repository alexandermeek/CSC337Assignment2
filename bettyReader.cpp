#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

std::string PATH = "Betty.raw";
unsigned int SIZE_X = 128, SIZE_Y = 32, SIZE_Z = 64;
unsigned int DIM = 3;
unsigned int X = 0, Y = 1, Z = 2;
unsigned int SQUARED = 2;

void read_betty(std::string path, int size, float *betty) {

	//Open binary file for reading
	std::ifstream betty_raw(path.c_str(), std::ios::in | std::ios::binary);
	if (betty_raw.is_open()) {
		unsigned char c = 0;

      for (int i = 0; i < size * DIM; i++) {
         betty_raw.read((char *)&c, 1);
         betty[i] = ((float)c)/255-0.5;
      }
	}
   betty_raw.close();
}

int main(int argc, char* argv[]) {
   float *betty = new float[SIZE_X * SIZE_Y * SIZE_Z * DIM];

   read_betty(PATH, SIZE_X * SIZE_Y * SIZE_Z, betty);

   //Save to csv
   std::ofstream file;
	file.open("betty.csv");
   file << "x-coord,y-coord,z-coord,mag" << std::endl; 
   for (int z = 0; z < SIZE_Z; z++) {
      for (int y = 0; y < SIZE_Y; y++) {
         for (int x = 0; x < SIZE_X; x++) {
            float magnitude = std::sqrt(std::pow(betty[(x + y * SIZE_X + z * SIZE_X * SIZE_Y) * DIM + X], SQUARED) 
                                      + std::pow(betty[(x + y * SIZE_X + z * SIZE_X * SIZE_Y) * DIM + Y], SQUARED)
                                      + std::pow(betty[(x + y * SIZE_X + z * SIZE_X * SIZE_Y) * DIM + Z], SQUARED));
            file << x << ","
                 << y << ","
                 << z << ","
                 << magnitude << std::endl;
         }
      }
   }

   //Finish and cleanup
   delete [] betty;
   std::cout << "Done!";
   file.close();
}