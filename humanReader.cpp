#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <sstream>

unsigned int SIZE_X = 2048, SIZE_Y = 1216, SIZE_Z = 2028;
unsigned int START_Z = 1732;
unsigned int DIM = 3;

void read_human(std::string path, unsigned int size, float *human) {

	//Open binary file for reading
	std::ifstream human_raw(path.c_str(), std::ios::in | std::ios::binary);
	if (human_raw.is_open()) {
		unsigned char c = 0;

      for (unsigned int i = 0; i < size * DIM; i++) {
         human_raw.read((char *)&c, 1);
         human[i] = ((float)c);
      }
	}
   human_raw.close();
}

int main(int argc, char* argv[]) {
   unsigned int max_x, max_y, max_z;
   unsigned int compr;

   if (argv[1] != nullptr) {
        compr = std::stoi(argv[1]);
    } 
    else {
        compr = 10;
    }

   //Save human as csv
   std::ofstream file;
   file.open("human.csv");
   file << "x-coord,y-coord,z-coord,i" << std::endl; 

   //Read each different raw file
   for (unsigned int z = START_Z; z <= SIZE_Z; z+=compr){
      float *human = new float[SIZE_X * SIZE_Y * DIM];
      std::stringstream path;
      path << "pelvis/a_vm" << z << ".raw";

      read_human(path.str(), SIZE_X * SIZE_Y, human);

      //Save all scalars for each point in raw file
      for (int y = 0; y < SIZE_Y; y+=compr) {
         for (int x = 0; x < SIZE_X; x+=compr) {
            file << x / compr << ","
               << y / compr << ","
               << (z - START_Z) / compr << ","
               << human[x + (y * SIZE_X) + 0] << std::endl;
            max_x = x;
         }
         max_y = y;
      }
      max_z = z;
      delete [] human;
      std::cout << "Read file: " << path.str() << std::endl;
   }
   file.close();
   //Output the max values since paraview requires the range to convert to
   //structured grid.
   std::cout << "Max (x,y,z): " << max_x / compr << "," 
                                << max_y / compr << "," 
                                << (max_z - START_Z) /compr << std::endl;
}