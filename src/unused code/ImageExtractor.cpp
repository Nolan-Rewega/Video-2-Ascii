#include"ImageExtractor.h"

using namespace std;
/*
 just get frame data and info   
*/

Extractor::Extractor(const char* file, bool live_stream){
    filename = file;
    is_live = live_stream;
}


/* return frame data.*/
unsigned char** Extractor::getFrameData(){
    if (!load_frame(filename, &frame_width, &frame_height, &frame_data)){
        cout << "failed to load frame..." << "\n";
    }
    return &frame_data;
}

int Extractor::getFrameHeight(){ return frame_height;}
int Extractor::getFrameWidth() { return frame_width; }

    


