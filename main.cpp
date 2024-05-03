#include <iostream>
#include <string>
#include "cs1300bmp.h"
using namespace std;
int main() {
    //using the cs1300bmp file from another class
    //turns a bmp file into a 2D array of rgb values
    struct cs1300bmp *input = new struct cs1300bmp;
    string filename = "C:\\Users\\peyto\\CLionProjects\\imageResize\\me.bmp";
    int ok = cs1300bmp_readfile((char *) filename.c_str(),input);

    const int scale = 8;
    int width = input->width;
    int height = input->height;

    if(ok){
        cout << "file loaded";

        struct cs1300bmp *linear = new struct cs1300bmp;
        linear->width = (input->width*scale)-(scale-1);
        linear->height = (input->height*scale)-(scale-1);

        for(int h = 0; h<3; h++) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    linear->color[h][i*scale][j*scale] = /*255;//*/input->color[h][i][j];
                }
            }
        }
        //convert the array into an image
        string outputName = "linear0.bmp";
        cs1300bmp_writefile((char *) outputName.c_str(), linear);

        for(int h = 0; h<3; h++) {
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height - 1; j++) {
                    int v1 = input->color[h][j][i];
                    int v2 = input->color[h][(j + 1)][i];
                    double delta = (double) (v2 - v1) / scale;
                    for (int k = 1; k < scale; k++) {
                        linear->color[h][j * scale + k][i * scale] = (unsigned char) (k * delta) + v1;
                    }
                }
            }
        }
        //convert the array into an image
        outputName = "linear1.bmp";
        cs1300bmp_writefile((char *) outputName.c_str(), linear);
        //interpolate horizonally
        for(int h = 0; h<3; h++) {
            for (int i = 0; i < linear->height; i++) {
                for (int j = 0; j < width-1; j++) {
                    int v1 = linear->color[h][i][j*scale];
                    int v2 = linear->color[h][i][(j+1)*scale];
                    double delta = (double) (v2 - v1) / scale;
                    for (int k = 1; k <= scale; k++) {
                        linear->color[h][i][j * scale + k] = (unsigned char) (k * delta) + v1;
                    }
                }
            }
        }
        outputName = "linear2.bmp";
        cs1300bmp_writefile((char *) outputName.c_str(), linear);
    }


    return 0;

}
