
#pragma once

#include <iostream>
#include <fstream>
#include <Windows.h>

static void rotate_bitmap(const char *src, const char *dest);
static void copy_file(const char *src, const char *dest);
static fpos_t get_filesize(const char *filename);
static void load_bitmap(const char *filename, unsigned char *buf, long long buf_len);

static void rotate_bitmap(const char *src, const char *dest)
{
	FILE *fpSrc  = fopen(src, "rb");
	FILE *fpDest = fopen(dest, "wb");

	long long len = get_filesize(src);
	unsigned char *buf = (unsigned char*)malloc(len);
	unsigned char *buf2 = (unsigned char*)malloc(len);
	unsigned char *top = buf;
	unsigned char *top2 = buf2;
	load_bitmap(src, buf, len);

	BITMAPFILEHEADER *header = (BITMAPFILEHEADER*)buf;
	memcpy(buf2, buf, sizeof(BITMAPFILEHEADER));
	buf += sizeof(BITMAPFILEHEADER);
	buf2 += sizeof(BITMAPFILEHEADER);
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER*)buf;
	memcpy(buf2, buf, sizeof(BITMAPINFOHEADER));
	buf += sizeof(BITMAPINFOHEADER);
	buf2 += sizeof(BITMAPINFOHEADER);

	int x, y;
	const int w = info->biWidth;
	const int h = info->biHeight;

	for(y = 0; y < h; y++)
	{
		for(x = 0; x < w; x++)
		{
			int dest = w * h * 3 - x * 3;
			int src  = x * 3 + y;
			buf2[dest - 2] = buf[src + 0];
			buf2[dest - 1] = buf[src + 1];
			buf2[dest - 0] = buf[src + 2];
		}
	}
	buf = top;
	buf2 = top2;

	fwrite(buf2, sizeof(unsigned char), len, fpDest);

	free(buf);
	free(buf2);
	fclose(fpSrc);
	fclose(fpDest);
}

static void load_bitmap(const char *filename, unsigned char *buf, long long buf_len)
{
	static const int BLOCK_LEN = 256;
	FILE *fp  = fopen(filename, "rb");
	if(buf == NULL)
	{
		return;
	}
	unsigned char *top = buf;
	size_t len;
	while((len = fread(buf, sizeof(unsigned char), BLOCK_LEN, fp)) == BLOCK_LEN)
	{
		buf += len;
	}
	buf = top;
	fclose(fp);
}

static fpos_t get_filesize(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	fpos_t size = 0;
	fpos_t end = fseek(fp, 0, SEEK_END);
	fgetpos(fp, &size);
	fseek(fp, end, SEEK_SET);
	fclose(fp);
	return size;
}

static void copy_file(const char *src, const char *dest)
{
	static const int BUF_SIZE = 256;
	FILE *fpSrc  = fopen(src, "rb");
	FILE *fpDest = fopen(dest, "wb");
	unsigned char buf[BUF_SIZE] = {0};
	
	size_t len;
	while((len = fread(buf, sizeof(unsigned char), BUF_SIZE, fpSrc)) == BUF_SIZE)
	{
		fwrite(buf, sizeof(unsigned char), BUF_SIZE, fpDest);
	}
	fwrite(buf, sizeof(unsigned char), len, fpDest);

	fclose(fpSrc);
	fclose(fpDest);
}

static void create_bitmap(const char *filename)
{
	const int WIDTH = 1024;
	const int HEIGHT = 768;
	std::ofstream ofs(filename, std::ios::binary);

	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	memset(&fh, 0, sizeof(BITMAPFILEHEADER));
	memset(&ih, 0, sizeof(BITMAPINFOHEADER));

	fh.bfType = 'B' | 'M' << 8;
	ih.biBitCount = 24;
	ih.biHeight = HEIGHT;
	ih.biPlanes = 1;
	ih.biSize = sizeof(BITMAPINFOHEADER);
	ih.biWidth = WIDTH;

	ofs.write((char*)&fh, sizeof(BITMAPFILEHEADER));
	ofs.write((char*)&ih, sizeof(BITMAPINFOHEADER));

	const int IMG_SIZE = WIDTH * HEIGHT * 3;
	unsigned char *buf = new unsigned char[IMG_SIZE];
	unsigned char *first = buf;
	memset(buf, 0, IMG_SIZE);

	for(int y = 0; y < HEIGHT; y++)
	{
		for(int x = 0; x < WIDTH; x++)
		{
			*buf++ = x ^ y;
			*buf++ = x ^ x;
			*buf++ = x ^ y;
		}
	}
	buf = first;

	ofs.write((char*)buf, IMG_SIZE);
	ofs.close();

	delete[] buf;
}