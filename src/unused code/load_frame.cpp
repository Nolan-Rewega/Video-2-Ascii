#include"load_frame.h"
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <inttypes.h>
}


bool load_frame(const char* filename, int* width_out, int* height_out, unsigned char** data_out){
    
    // creating context
    AVFormatContext* av_format_ctx = avformat_alloc_context();
    
    // error checking the avformat context
    if (!av_format_ctx) {
        printf("error in creating avformat context...\n");
        return false;
    }
    
    // open the file into context.
    avformat_open_input(&av_format_ctx, filename, NULL, NULL);

    int video_stream_index = -1;
    AVCodecParameters* av_codec_params;
    AVCodec* av_codec;
    
    // finding the a codec and the video stream
    for (int i = 0; i < av_format_ctx -> nb_streams; i++ ){
        auto streams = av_format_ctx -> streams[i];
        
        av_codec_params = av_format_ctx->streams[i]->codecpar;
        av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        
        // checking if there is a valid decoder
        if(!av_codec){continue;}
        
        if(av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_index = i;
            break;
        }
    }

    // setting up codec context for the decoder.
    AVCodecContext* av_codec_ctx = avcodec_alloc_context3(av_codec);
    if(!av_codec_ctx){
        printf("Couldn't allocate AVcodecContext...\n");
        return false;
    }
    
    if(avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0){
        printf("Couldn't allocate AVCodecContext\n");
        return false;
    }

    if(avcodec_open2(av_codec_ctx, av_codec, NULL) < 0){
        printf("Couldn't open codec\n");
        return false;
    }

    AVFrame* av_frame = av_frame_alloc();
    if(!av_frame){
        printf("error in AV_frame...\n");
        return false;
    }
    
    AVPacket* av_packet = av_packet_alloc();
    if(!av_packet){
        printf("error in AV_packet...\n");
        return false;
    }
    int response;

    // read packets to find a video packet
    while (av_read_frame(av_format_ctx, av_packet) >= 0){
        if (av_packet->stream_index != video_stream_index){ continue;}

        response = avcodec_send_packet(av_codec_ctx, av_packet);
        if (response < 0){
            printf("error in send packet...\n");
            return false;
        }
        
        response = avcodec_receive_frame(av_codec_ctx, av_frame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF){
            continue;
        }else if (response < 0){
            printf("failed to decode packet...\n");
            return false;
        }
        
        av_packet_unref(av_packet);
        break;
    }
    

    // loop through each width and height pixel
    unsigned char* data = new unsigned char[av_frame->width * av_frame->height];
    cout << "width: " << av_frame->width << "| height: " << av_frame->height << "\n";
    
    cout << "av -> linesize[]: " << av_frame->linesize[0] << '\n';
    cout << "av -> data[]: " << (int)av_frame->data[0][1000*1920+960] << "\n";
    for(int w = 0; w < av_frame->width; w++){
        for(int h = 0; h < av_frame->height; h++){
            data[h * av_frame->width + w] = av_frame->data[0][h* av_frame->linesize[0]+w];
        }
    }

    *width_out = av_frame->width;
    *height_out = av_frame->height;
    *data_out = data;
    

    // closing file and freeing context memory.
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    avcodec_free_context(&av_codec_ctx);
    av_frame_free(&av_frame);
    av_packet_free(&av_packet);

    return true;
}
