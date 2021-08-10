#include"FrameParser.h"

Parser::Parser(const char* file){
    filename = file;
    init();
}


bool Parser::init(){
    /* initialize context*/
    av_format_ctx = avformat_alloc_context();
    avformat_open_input(&av_format_ctx, filename, NULL,NULL);


    /* search all the streams for the codec and video*/
    for (int i = 0; i < av_format_ctx -> nb_streams; i++){
        auto streams = av_format_ctx -> streams[i];

        av_codec_params = av_format_ctx->streams[i]->codecpar;
        av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        
        /* if av_codec is invalid skip to next stream*/
        if(!av_codec){continue;}
        if(av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_index = i;
            time_base = av_format_ctx->streams[i]->time_base;
            break;
        }
    }
        // to do add error checking
    av_codec_ctx = avcodec_alloc_context3(av_codec);
    avcodec_parameters_to_context(av_codec_ctx, av_codec_params);
    avcodec_open2(av_codec_ctx, av_codec, NULL);

    av_frame = av_frame_alloc();
    av_packet = av_packet_alloc();
    
    return true;
}


bool Parser::get_frame_data(unsigned char** data_out){
    int response;
    int eof_flag = 1;

    /* find one video packet at a time */
    while(eof_flag >= 0){
        /* on error or EOF, eof_flag is < 0 */
        eof_flag = av_read_frame(av_format_ctx, av_packet);
        if(eof_flag < 0){return false;}

        /* keep skiping until a video frame is found. */
        if(av_packet->stream_index != video_stream_index) { 
            av_packet_unref(av_packet);
            continue; 
        }
        
        response = avcodec_send_packet(av_codec_ctx, av_packet);
        if(response < 0){ cout << "Error in send_packet...\n"; return false; }
        
        /* Know we check to see if the video frame is EOF or a duplicate */
        response = avcodec_receive_frame(av_codec_ctx, av_frame);
        if(response == AVERROR(EAGAIN)){
            av_packet_unref(av_packet);
            continue; 
        }
        
        else if(response < 0){ cout << "Error in receive_frame..\n"; return false; }
        
        /* de-reference the packet in preperation for the next frame. */
        av_packet_unref(av_packet);
        break;
    }
    
    /* get the time the current frame should be displayed at*/
    curr_pts = av_frame->pts;
    
    /* create a frame data buffer*/
    unsigned char* data = new unsigned char[av_frame->width * av_frame->height];
    
    /* loop through each pixel, copying along the way*/
    for(int w = 0; w < av_frame->width; w++){
        for(int h = 0; h < av_frame->height; h++){
            data[h * av_frame->width + w] = av_frame->data[0][h * av_frame->linesize[0] + w];
        }
    }
    
    *data_out = data;
    return true;
}


int Parser::get_frame_width(){ return av_codec_params->width; }
int Parser::get_frame_height(){ return av_codec_params->height; }


void Parser::free_all(){
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    avcodec_free_context(&av_codec_ctx);
    av_frame_free(&av_frame);
    av_packet_free(&av_packet);
}



