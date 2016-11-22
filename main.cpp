#include <cstdlib>
#include <iostream>
#include <ctime>
#include <time.h>
#include <iomanip>
#include <vector>
#include <fstream>
using namespace std;

#define COMPLEMENT_VALUE -1;


class Prediction  {
    public:
      std::size_t row, col;
      int ** matriz;
      vector<string> header;
      string file;

      Prediction (string file_name) 
        : file(file_name) {
          this->LoadMatriz();
          this->matriz = createMatriz();
          //this->printMatriz(matriz, header, row, col);
      }

      int** createMatriz() {
          int ** m;
          m = new int*[row];        
          for (size_t i=0; i<row; ++i) {
              m[i] = new int[col];
              for (size_t j=0; j<col; ++j){
                m[i][j] = 0;
            }
          }
          return m;
      }

      int getComplementValue(int x, int y) {
          char cpm;
          string cad_one = this->header.at(0);
          string cad_two = this->header.at(1);
          if (cad_one.at(x) == 'A')
              cpm = 'U';
          else if (cad_one.at(x) == 'U')
              cpm = 'A';
          else if (cad_one.at(x) == 'C')
              cpm = 'G';
          else if (cad_one.at(x) == 'G')
              cpm = 'C';
          char cpm_two = cad_two.at(y);
          if (cpm == cpm_two) {
              return COMPLEMENT_VALUE;    
          }
          return 0;
      }

      int getRangeValue(int x, int y) {
          int min_value = 0;
          for (int k=x+1; k<y; ++k){
              int sum = this->matriz[x][k] + this->matriz[k+1][y];
              if (sum < min_value)
                min_value = sum;
          }
          return min_value;
      }

      int getMinValue(int x, int y) {
          int min_value = this->matriz[x][y-1];
          if (this->matriz[x+1][y] < min_value)
              min_value = this->matriz[x][y-1];
          int complement = this->matriz[x+1][y-1] + getComplementValue(x, y);
          if (complement < min_value)
              min_value = complement;
          int range_value = getRangeValue(x, y);
          if (range_value < min_value)
              min_value = range_value;
          return min_value;
      }

      void fillTable() {
          for (int x = 1; x < this->col; ++x) {
              int temp = 0;
              for (int u = 0; u < this->row-x; ++u) {
                  this->matriz[u][x+temp] = getMinValue(u, x+temp);
                  temp ++;
              } 
          }
          cout << "\n\n\tFINAL TABLE (WATSON-CRICK)\n" << endl;
          this->printMatriz(matriz, header, row, col);
      }

      void LoadMatriz() {
          ifstream in(this->file);
          if (!in) {
              cout << "Cannot open file.\n";
              return;
          }
          string line;
          for( size_t x = 0; getline( in, line ); ) {
              int i = 0;
              stringstream ssin(line);
              string temporal_header;
              while (ssin.good() && i < line.size()){
                ssin >> temporal_header;
                ++i;
              }
              cout << "\t\tSecuence: " << temporal_header << endl;
              header.push_back(temporal_header);
              ++x;            
          }
          if (header.at(0) < header.at(1))
            swap(header.at(0), header.at(1));
          this->row = header.at(0).size();
          this->col = header.at(1).size();
          in.close();
          cout << "\t\tComplement Value: " << COMPLEMENT_VALUE;
      }

      void printMatriz(int ** matriz, vector<string> header, size_t row, size_t col) {
          cout << "\n" << setw(7) << "-";
          for (size_t x = 0; x < col; ++x)
              cout << setw(7) << header.at(1)[x];
          cout << endl;   
          for (size_t x = 0; x < row; ++x) {
              cout << setw(7) << header.at(0)[x];
              for (size_t y = 0; y < col; ++y) {
                cout << setw(7) << *(*(matriz + x) + y);
              }
              cout << endl;
          }
      }

      ~Prediction() {
          for (size_t i = 0; i < this->row; ++i) {
            delete[] this->matriz[i];
          }
          delete[] this->matriz;
      }
};

int main() {
    Prediction *app = new Prediction("data.txt");
    app->fillTable();
    return 0;
}
