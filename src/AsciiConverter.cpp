#include"AsciiConverter.h"
#include <fstream>

/*
    Goal of this file is to convset frame data into ASCII chars.
*/

string toAscii(int target_width, int target_height, int frame_width, int frame_height, unsigned char** data){
    
    ofstream file;
    file.open("log.txt");

    /* defining buffers */
    string ascii_array;
    unsigned char frame_luma;
    
    /* darkest to lightest; 0 -> 17 */

    map <int, const char*> usable_chars = {
        {20, "#"}, {19, "&"}, {18, "@"},
        {17, "%"}, {16, "K"}, {15, "g"},
        {14, "$"}, {13, "8"}, {12, "E"},
        {11, "q"}, {10, "u"}, {9,  "?"},
        {8,  "*"}, {7,  "+"}, {6,  "+"},
        {5,  "t"}, {4,  "1"}, {3,  "!"},
        {2,  "-"}, {1,  "."}, {0,  " "}
    };

    /*
    map <int, const char*> usable_chars = {
        {8, "#"}, {7, "&"}, {6, "%"},
        {5, "$"},  {4, "?"},  {3, "/"},
        {2, "*"},  {1, "-"},  {0, "."}
    };
    */
    /* luma_threashold */
    double luma_threashold = 255 / (usable_chars.size() - 1);
    //file << luma_threashold << endl;
    int skip_w = frame_width / target_width;
    int skip_h = frame_height / target_height;
    int key;
    
    // loop through each pixel incrementing by skip.
    for(int h = 0; h < frame_height; h += skip_h){
        for(int w = 0; w < frame_width; w += skip_w){
            frame_luma = (*data)[frame_width * h + w];
            key = (int)frame_luma / luma_threashold;
            
            ascii_array.append(usable_chars[key]);
        }
    }
    return ascii_array;
};

