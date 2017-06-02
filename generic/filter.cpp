/*! 
    This code was adapted from the cross bilateral filter
    from http://people.csail.mit.edu/sparis/bf/    
    
    Copyright (c) 2017, Demetris Marnerides
    Copyright (c) 2006, Sylvain Paris and Frédo Durand

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy,
    modify, merge, publish, distribute, sublicense, and/or sell copies
    of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

*/


#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/filter.cpp"
#else

#define NO_XML
#include "fast_lbf.h"

static int libfilter_(Main_lbf)(lua_State *L)
{
    typedef Array_2D<real> image_type;
    // Dimensions
    int width = luaL_checkint(L, 3);
    int height = luaL_checkint(L, 4);

    // Image
    THTensor *imgtensor = (THTensor*) luaT_checkudata(L, 1, torch_Tensor);
    THTensor *imgtensorc = (THTensor*) THTensor_(newContiguous)(imgtensor);
    real *tdataimg = THTensor_(data)(imgtensorc);
    image_type image(width,height);
    int count = 0;
    for(unsigned y=0;y<height;y++){
        for(unsigned x=0;x<width;x++){
            image(x,y) = tdataimg[count++]; 
        }
    }

    // Edge
    THTensor *edgetensor = (THTensor*) luaT_checkudata(L, 2, torch_Tensor);
    THTensor *edgetensorc = (THTensor*) THTensor_(newContiguous)(edgetensor);
    real *tdataedge = THTensor_(data)(edgetensorc);
    image_type edge(width,height);
    count = 0;
    for(unsigned y=0;y<height;y++){
        for(unsigned x=0;x<width;x++){
            edge(x,y) = tdataedge[count++]; 
        }
    }

    // Sigma
    double sigma_s = (double)luaL_checknumber(L,5);
    double sigma_r = (double)luaL_checknumber(L,6);

    image_type filtered_image(width,height);
   
    Image_filter::fast_LBF(image,edge,
                sigma_s,sigma_r,
                false,
                &filtered_image,&filtered_image);

    //Result Tensor
    THTensor *restensor = NULL;
    restensor = THTensor_(newWithSize2d)( height, width);
    real *restdata = THTensor_(data)(restensor);

    count = 0;
    for(unsigned y=0;y<height;y++){
        for(unsigned x=0;x<width;x++){
            restdata[count++] = filtered_image(x,y);
        }
    }

    luaT_pushudata(L, restensor, torch_Tensor);
    return 1;
}

static const luaL_Reg libfilter_(Main__)[] =
{
  {"lbf", libfilter_(Main_lbf)},
  {NULL, NULL}
};

extern "C" { DLL_EXPORT int libfilter_(Main_init)(lua_State *L)
{
  luaT_pushmetatable(L, torch_Tensor);
  luaT_registeratname(L, libfilter_(Main__), "libfilter");
  return 1;
}
}

#endif

