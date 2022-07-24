#ifndef FRAMEPARSER_H
#define FRAMEPARSER_H
#include<iostream>
extern "C"{
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<inttypes.h>
}

using namespace std;

/* 
    Frame parser class to hold a instance of a video's
    FFMPEG variables.
*/
class Parser{
public:
    /* mp4 file basic info */
    const char* filename;

    /* codec data */
    AVFormatContext* av_format_ctx;
    AVCodecContext* av_codec_ctx;
    AVCodecParameters* av_codec_params;
    AVCodec* av_codec;
    
    /* audio/video channel indcies*/
    int video_stream_index;
    int audio_stream_index;
    
    /* frame and packet data*/
    AVFrame* av_frame;
    AVPacket* av_packet;

    /* time base data*/
    AVRational time_base;
    int64_t curr_pts;


    /* Public method decleration*/
    Parser(const char* file);
    bool init();
    bool get_frame_data(unsigned char** data_out);
    int get_frame_width();
    int get_frame_height();
    void free_all();

};
#endif
