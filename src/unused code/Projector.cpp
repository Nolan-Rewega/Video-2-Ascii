#include"Projector.h"

/*
    Outputing via terminal window
*/

void displayFrame(string frame, int width, int height){
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++ ){
            cout << frame[width * h + w];
        }
        cout << "\n";
    }

}





