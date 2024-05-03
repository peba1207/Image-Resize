#include <iostream>
#include <string>
#include "cs1300bmp.h"
using namespace std;
int main() {
    //using the cs1300bmp file from another class
    //turns a bmp file into a 2D array of rgb values
    auto *input = new struct cs1300bmp;
    string filename = R"(C:\Users\peyto\CLionProjects\imageResize\me.bmp)";
    int ok = cs1300bmp_readfile((char *) filename.c_str(),input);

    const int scale = 2;
    int width = input->width;
    int height = input->height;

    if(ok){
        cout << "file loaded";

        auto *linear = new struct cs1300bmp;
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

        auto * cubic = new struct cs1300bmp;
        cubic->width = (input->width*scale)-(scale-1);
        cubic->height = (input->height*scale)-(scale-1);

        //Cubic spline
        //vertical interpolations
        auto* a= new double[height+1];
        auto* b= new double[height];
        auto* d= new double[height];
        auto* alpha= new double[height];
        auto* c= new double[height+1];
        auto* l= new double[height+1];
        auto* mu= new double[height+1];
        auto* z= new double[height+1];

        for(int h = 0; h<3; h++) {
            for(int g = 0; g<width;g++) {
                for (int i = 0; i < height; i++) {
                    a[i] = input->color[h][i][g];
                }

                for (int i = 1; i < height - 1; i++) {
                    alpha[i] = (3.0 / scale) * (a[i + 1] - a[i]) - (3.0 / scale) * (a[i] - a[i - 1]);
                }

                l[0] = 1;
                mu[0] = 0;
                z[0] = 0;
                for (int i = 1; i < height - 1; i++) {
                    l[i] = 4 * scale - scale * mu[i - 1];
                    mu[i] = scale / l[i];
                    z[i] = (alpha[i] - scale * z[i - 1]) / l[i];
                }
                l[height - 1] = 1;
                z[height - 1] = 0;
                c[height - 1] = 0;
                for (int j = height - 2; j >= 0; j--) {
                    c[j] = z[j] - mu[j] * c[j + 1];
                    b[j] = (a[j + 1] - a[j]) / scale - (scale * (c[j + 1] + 2 * c[j])) / 3;
                    d[j] = (c[j + 1] - c[j]) / (3 * scale);
                }

                //use spline to interpolate
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < scale; j++) {
                        cubic->color[h][scale * i + j][g*scale] = (unsigned char) (a[i] + b[i] * j + c[i] * (j * j) +
                                                                             d[i] * (j * j * j));
                    }
                }
            }
        }
        outputName = "cubic1.bmp";
        cs1300bmp_writefile((char *) outputName.c_str(), cubic);

        delete[] a;
        delete[] b;
        delete[] d;
        delete[] alpha;
        delete[] c ;
        delete[] l ;
        delete[] mu ;
        delete[] z;
        //horizontal splines
        a= new double[width+1];
        b= new double[width];
        d= new double[width];
        alpha= new double[width];
        c= new double[width+1];
        l= new double[width+1];
        mu= new double[width+1];
        z= new double[width+1];
        for(int h = 0; h<3; h++) {
            for(int g = 0; g<cubic->height;g++) {
                for (int i = 0; i < width; i++) {
                    a[i] = cubic->color[h][g][i*scale];
                }

                for (int i = 1; i < width - 1; i++) {
                    alpha[i] = (3.0 / scale) * (a[i + 1] - a[i]) - (3.0 / scale) * (a[i] - a[i - 1]);
                }

                l[0] = 1;
                mu[0] = 0;
                z[0] = 0;
                for (int i = 1; i < width - 1; i++) {
                    l[i] = 4 * scale - scale * mu[i - 1];
                    mu[i] = scale / l[i];
                    z[i] = (alpha[i] - scale * z[i - 1]) / l[i];
                }
                l[width - 1] = 1;
                z[width - 1] = 0;
                c[width - 1] = 0;
                for (int j = width - 2; j >= 0; j--) {
                    c[j] = z[j] - mu[j] * c[j + 1];
                    b[j] = (a[j + 1] - a[j]) / scale - (scale * (c[j + 1] + 2 * c[j])) / 3;
                    d[j] = (c[j + 1] - c[j]) / (3 * scale);
                }

                //use spline to interpolate
                for (int i = 0; i < width; i++) {
                    for (int j = 0; j < scale; j++) {
                        cubic->color[h][g][scale * i + j] = (unsigned char) (a[i] + b[i] * j + c[i] * (j * j) +
                                                                               d[i] * (j * j * j));
                    }
                }
            }
        }

        outputName = "cubic2.bmp";
        cs1300bmp_writefile((char *) outputName.c_str(), cubic);
    }


    return 0;

}
