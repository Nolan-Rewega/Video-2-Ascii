#include"AsciiConverter.h"

/*
    Goal of this file is to convset frame data into ASCII chars.
*/

string toAscii(int target_width, int target_height, int frame_width, int frame_height, unsigned char** data){
    /* defining buffers */
    string ascii_array;
    unsigned char frame_luma;
    
    /* darkest to lightest; 0 -> 8 */
    map <int, const char*> usable_chars = {
        {0, "#"}, {1, "&"}, {2, "%"},
        {3, "$"}, {4, "?"}, {5, "/"},
        {6, "*"}, {7, "-"}, {8, "."}
    };
    /* luma_threashold == ceiling(255 / size(usable_chars)) */
    double luma_threashold = 29;
    int skip = frame_width / target_width;
    int key;
    
    // loop through each pixel incrementing by skip.
    for(int h = 0; h < frame_height; h += skip){
        for(int w = 0; w < frame_width; w += skip){
            frame_luma = (*data)[frame_width * h + w];
            key = (int)frame_luma / luma_threashold;
            ascii_array.append(usable_chars[key]);
        }
    }
    return ascii_array;
};

