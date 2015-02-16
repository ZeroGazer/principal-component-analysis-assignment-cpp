#include <stdio.h>
#include <algorithm>
#define cimg_display 0
#include "CImg.h"
#include <string>
#include <sstream>
#define size1 1000
#define size2 250

using namespace std;
using namespace cimg_library;


int main()
{
  FILE *fid=fopen("data.txt","w+");
  for(int j = 1; j <= size1*2; j++) {
    string name;
    stringstream out;
    out << j;
    string num=out.str();
    string format=".jpg";
    if(j <= size1)
      name="trainingset/tr4_";
    else
      name="trainingset/tr9_";
    CImg<float> img((name+num+format).c_str());
    int width=img.width();
    int height=img.height();
    int i;
    // get the storage buffer pointer
    float *img_ptr=img.data();
    // get the max/min element of image
    float max=*max_element(img_ptr,img_ptr+width*height);
    float min=*min_element(img_ptr,img_ptr+width*height);
    // normalization
    for(int i=0;i<width*height;i++)
      img_ptr[i]=(img_ptr[i]-min)/(max-min);
    //write to a file
    //File *fid=fopen("sample.txt","w+");
    for(i=0;i<height*width;i++)
      // write normalized pixel values
      fprintf(fid,"%f ", img_ptr[i]);
  }

  for(int j = 1; j <= size2*2; j++) {
    string name;
    stringstream out;
    out << j;
    string num=out.str();
    string format=".jpg";
    if(j <= size2)
      name="validationset/tr4_";
    else
      name="validationset/tr9_";
    CImg<float> img((name+num+format).c_str());
    int width=img.width();
    int height=img.height();
    int i;
    // get the storage buffer pointer
    float *img_ptr=img.data();
    // get the max/min element of image
    float max=*max_element(img_ptr,img_ptr+width*height);
    float min=*min_element(img_ptr,img_ptr+width*height);
    // normalization
    for(int i=0;i<width*height;i++)
      img_ptr[i]=(img_ptr[i]-min)/(max-min);
    //write to a file
    //File *fid=fopen("sample.txt","w+");
    for(i=0;i<height*width;i++)
      // write normalized pixel values
      fprintf(fid,"%f ", img_ptr[i]);
  }

  //fclose(fid);
  fclose(fid);

  return 0;
}
