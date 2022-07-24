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
    Parser video_parser = Parser("/home/shadow/code/Video-2-Ascii/src/videos/social.mp4");
    int width = video_parser.get_frame_width();
    int height = video_parser.get_frame_height();
    
    /* Modifiable parameters */
    int display_w = 1280; int display_h = 720;
    int target_w = 240; int target_h = 135;
    int text_size = 6;
   
    /* Create OpenGL Object*/
    DisplayGL display = DisplayGL( (float)display_w,
                                   (float)display_h,
                                   (float)target_w,
                                   (float)target_h,
                                   text_size
    );
   

    /* False when frame read is EOF or ERROR*/
    bool is_success = true;
    unsigned char* data_ptr;
    
    while(is_success && !display.check_termination()){
        /*  1. Fetch frame */
        is_success = video_parser.get_frame_data(&data_ptr);
        if(!is_success){ cout << "terminating...\n"; break;}

        /*  2. Convert to Ascii */
        string ascii = toAscii(target_w, target_h, width, height, &data_ptr);
        free(data_ptr);

        /*  3. Displaying with openGL */
        display.background();
        display.render_text(ascii, 1.0f);
        display.post_render();
    }
    
    /* Clean-up functions */
    display.exit();
    video_parser.free_all();
    return 0;
};

