#include "image.h"

#include <jpeglib.h>
#include <cstdlib>

using namespace std;

Image::Image(int width, int height)
{
	this->width = width;
	this->height = height;
	data = new unsigned char[3 * width * height];
	for(int i=0; i < 3*width*height; i ++)
		data[i] = 0;
}

Image::Image(string filename)
{
	load(filename);
}

Image::~Image()
{
	delete data;
}

Image::Image(Image &img)
{
	this->width = img.width;
	this->height = img.height;
	this->data = new unsigned char[3*width*height];
	for(int i=0; i < 3*width*height; i ++)
		this->data[i] = img.data[i];
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}


void Image::load(string filename)
{
	FILE *infile;
	struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;

        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);

        if ((infile = fopen(filename.c_str(), "rb")) == NULL) 
	{
            cout << "Can't open " << filename << endl;
            exit(EXIT_FAILURE);
        }

	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);

	width = cinfo.image_width;
	height = cinfo.image_height;

	data = new unsigned char[3*width*height];

	if(data == NULL)
	{
		cout << "Error in Memory allocation. " << endl;
		exit(EXIT_FAILURE);
	}

	jpeg_start_decompress(&cinfo);

	int i = 0;

	buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, width*3, 1);

	while (cinfo.output_scanline < cinfo.output_height) 
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);

		for(int j=0; j < 3*width; j++)
			data[(cinfo.output_scanline-1)*3*width + j] = (*buffer)[j];
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);
}

void Image::save(string filename)
{
	FILE *outfile;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer;          

	if((outfile = fopen(filename.c_str(), "wb")) == NULL)
	{
		cout << "Can't open " << filename << " for saving image." << endl;
		exit(EXIT_FAILURE);
        }

 
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);
  
	cinfo.image_width      = width;
	cinfo.image_height     = height;
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality (&cinfo, 96, true);	  /*set the quality [0..100]  */
	jpeg_start_compress(&cinfo, true);

  
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		row_pointer = (JSAMPROW) &data[cinfo.next_scanline*width*3];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);
}

unsigned char & Image::operator()(int x, int y, int i)
{
	return data[y*3*width + x*3 + i];
}

void Image::flipHorizontally()
{
	for(int y=0; y < height / 2; y ++)
		for(int x = 0; x < width; x ++)
			for(int c =0; c < 3; c++)
			{
				int d = (*this)(x,y, c);
				(*this)(x,y, c) = (*this)(x,height-y-1, c);
				(*this)(x,height-y-1, c) = d;
			}
}
