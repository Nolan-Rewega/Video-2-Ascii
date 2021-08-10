#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include<iostream>
#include<string>
#include"load_frame.h"
#include"AsciiConverter.h"
#include"Projector.h"

/* Extracts frame data from a mp4*/
class Extractor{
public:
    const char* filename;
    bool is_live;
  
    int frame_width, frame_height;
    unsigned char* frame_data;

    Extractor(const char* file, bool live_stream);
    unsigned char** getFrameData();
    int getFrameHeight();
    int getFrameWidth();
    };
#endif
