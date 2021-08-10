#include<iostream>
#include"AsciiConverter.h"
#include"FrameParser.h"
#include"RenderText.h"

using namespace std;

int main(int argc, char *argv[]){
    /* Three steps per frame cycle
        1. extract current frame data from video
        2. convert the frame to a Ascii frame
        3. display the Ascii to a display medium (terminal or OpenGL window)
    */

    /* Raw video data */
    Parser video_parser = Parser("/home/shadow/code/Video-2-Ascii/src/videos/kakashi.mp4");
    int width = video_parser.get_frame_width();
    int height = video_parser.get_frame_height();
    
    /* Modifiable parameters */
    int display_w = 1280; int display_h = 720;
    int target_w = 240; int target_h = 135;
    int text_size = 6;
    float rgb_array[3] = {1.0f, 1.0f, 1.0f};
   
    /* Create OpenGL Object*/
    DisplayGL display = DisplayGL(display_w, display_h, text_size, rgb_array );
    
    unsigned char* data_ptr;

    /* False when frame read is EOF */
    bool is_success = true;
    
    /* The Ascii charcters representing 1 frame. */
    string line;

    while(is_success && !display.check_termination()){
        /*  1. */
        // -- catch a frame.
        is_success = video_parser.get_frame_data(&data_ptr);
        if(!is_success){ cout << "terminating...\n"; break;}

        /*  2. */
        // -- Convert to Ascii via AsciiConverter.h
        string ascii = toAscii(target_w, target_h, width, height, &data_ptr);
        
        /*  3. */
        // -- displaying with openGL
        display.background();
        for(int i = 0; i < target_h; i++){
            line = ascii.substr(i * target_w, target_w);
            display.render_text(line, 0.0f, (float)display_h - (float)(text_size * i), 1.0f);
        }
        display.post_render();
    }
    
    /* Clean-up functions */
    display.exit();
    video_parser.free_all();
    return 0;
};




