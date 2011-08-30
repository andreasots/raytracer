extern "C"
{
	#include <png.h>
}

#include <jpeglib.h>

#include <cstdlib>
#include <csetjmp>

struct jpeg_error_mgr_jump: public jpeg_error_mgr
{
	jmp_buf jmp;
};

void jpeg_error_handler(j_common_ptr cinfo)
{
	longjmp(reinterpret_cast<jpeg_error_mgr_jump*>(cinfo->err)->jmp, 1);
}

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "PixelToaster/PixelToaster.h"

class Image
{
public:
	virtual ~Image()
	{
	}
	PixelToaster::Pixel pixel(int x, int y)
	{
		return mData[x+y*mW];
	}
	
	std::vector<PixelToaster::Pixel> data()
	{
		return mData;
	}
	
	int width()
	{
		return mW;
	}
	
	int height()
	{
		return mH;
	}
protected:
	std::vector<PixelToaster::Pixel> mData;
	int mW, mH;
};

class PNGImage: public Image
{
public:
	PNGImage(std::string file)
	{
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr)
        		throw std::ios::failure("PNG init failed");
        	png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			png_destroy_read_struct(&png_ptr, NULL, NULL);
	        	throw std::ios::failure("PNG init failed");
		}

		png_infop end_info = png_create_info_struct(png_ptr);
		if (!end_info)
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			throw std::ios::failure("PNG init failed");
		}
		
		FILE *f = fopen(file.c_str(), "rb");
		
		if(setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			fclose(f);
			throw std::ios::failure("PNG init failed");
		}
		
		png_init_io(png_ptr, f);
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_GRAY_TO_RGB, NULL);
		
		mW = png_get_image_width(png_ptr, info_ptr);
		mH = png_get_image_height(png_ptr, info_ptr);		
		png_bytep *rows = png_get_rows(png_ptr, info_ptr);
		
		for(size_t y = 0; y < mH; y++)
		{
			for(size_t x = 0; x < mW*3; x+=3)
				mData.push_back(PixelToaster::Pixel(rows[y][x]/255.0, rows[y][x+1]/255.0, rows[y][x+2]/255.0));
		}
		
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	}
	
	static bool detect(std::string file)
	{
		unsigned char buf[8];
		std::ifstream in(file.c_str(), std::ios::in | std::ios::binary);
		in.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
		in.read(reinterpret_cast<char*>(buf), 8);
		bool is_png = !png_sig_cmp(buf, 0, 8);
		return is_png;
	}
	
	virtual ~PNGImage()
	{
	}
	
};

class JPEGImage: public Image
{
public:
	JPEGImage(std::string file)
	{
		FILE *fp = fopen(file.c_str(), "rb");
		if(!fp)
			throw std::ios::failure("read failed");
		struct jpeg_decompress_struct cinfo;
	        struct jpeg_error_mgr jerr;
	        cinfo.err = jpeg_std_error(&jerr);
	        jpeg_create_decompress(&cinfo);
	        jpeg_stdio_src(&cinfo, fp);
		jpeg_read_header(&cinfo, TRUE);
		jpeg_start_decompress(&cinfo);
		mW = cinfo.output_width;
		mH = cinfo.output_height;
		JSAMPLE *row = new JSAMPLE[mW*cinfo.output_components];
		
		if(cinfo.output_components == 1) // Grayscale
		{
			for(size_t y = 0; y < mH; y++)
			{
				jpeg_read_scanlines(&cinfo, &row, 1);
				for(size_t x = 0; x < mW; x++)
					mData.push_back(PixelToaster::Pixel(row[x]/255.0, row[x]/255.0, row[x]/255.0));
			}
		}
		else if(cinfo.output_components == 3) // Truecolor
		{
			for(size_t y = 0; y < mH; y++)
			{
				jpeg_read_scanlines(&cinfo, &row, 1);
				for(size_t x = 0; x < mW*3; x+=3)
					mData.push_back(PixelToaster::Pixel(row[x]/255.0, row[x+1]/255.0, row[x+2]/255.0));
			}
		}
		else
			throw std::ios::failure("Unsupported format");
		
		delete[] row;
	}
	
	virtual ~JPEGImage()
	{
	}
	
	static bool detect(std::string file)
	{
		FILE *fp = fopen(file.c_str(), "rb");
		if(!fp)
			throw std::ios::failure("read failed");
		struct jpeg_decompress_struct cinfo;
	        struct jpeg_error_mgr_jump jerr;
	        cinfo.err = jpeg_std_error(&jerr);
	       	jerr.error_exit = jpeg_error_handler;
    		if(setjmp(jerr.jmp))
	        {
	        	jpeg_destroy_decompress(&cinfo);
	        	return false;
	        }
	        jpeg_create_decompress(&cinfo);
	        jpeg_stdio_src(&cinfo, fp);
		jpeg_read_header(&cinfo, TRUE);
		return true;
	}
};

Image *detect(std::string file)
{
	if(PNGImage::detect(file))
		return new PNGImage(file);
	if(JPEGImage::detect(file))
		return new JPEGImage(file);
	throw std::ios::failure("Unknown format");
}

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			Image *img;
			try
			{
				img = detect(argv[i]);
			}
			catch(std::exception &e)
			{
				std::cerr << "Error reading file '" << argv[i] << "': " << e.what() << std::endl;
				continue;
			}
							
			PixelToaster::Display screen(argv[i], img->width(), img->height());
			screen.update(img->data());
			while(screen.open())
			        screen.update(img->data());
			delete img;
		}
	}
	return 0;
}
