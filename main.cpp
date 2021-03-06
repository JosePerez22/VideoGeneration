#include <iostream>
#include <stdio.h>
#include <cstring>
#include "global.h"
#include "Frame.h"
#include "Rectangle.h"
#include <vector>

using namespace std;

Frame frame;
int main(int argc, char * argv[]) {
    const char * cmd =
    "ffmpeg              "
    "-y                  "
    "-hide_banner        "
    "-f rawvideo         " // input to be raw video data
    "-pixel_format rgb24 "
    "-video_size 720x480 "
    "-r 60               " // frames per second
    "-i -                " // read data from the standard input stream
    "-pix_fmt yuv420p    " // to render with Quicktime
    "-vcodec mpeg4       "
    "-an                 " // no audio
    "-q:v 5              " // quality level; 1 <= q <= 32
    "output.mp4          ";
    
#ifdef _WIN32
    FILE * pipe = _popen(cmd, "wb");
#else
    FILE * pipe = popen(cmd, "w");
#endif
    if (pipe == 0) {
        cout << "error: " << strerror(errno) << std::endl;
        return 1;
    }
    
    vector<Rectangle>rect;
    
    Rectangle r1 (105, 105, 0, 0, 255, 255, 255);
    r1.setSpeed(5, 4);
    
    Rectangle r2 (45, 45, 5, 5, 0xff, 0x00, 0x00);
    r2.setSpeed(5, 4);
    
    Rectangle r3(45, 45, 55, 55, 0xff, 0xff, 0x00);
    r3.setSpeed(5, 4);
    Rectangle r4(45, 45, 5, 55, 0x00, 0x00, 0xff);
    r4.setSpeed(5, 4);
    Rectangle r5(45, 45, 55, 5, 0x00, 0xff, 0x00);
    r5.setSpeed(5, 4);
    
    rect.push_back(r1);
    rect.push_back(r2);
    rect.push_back(r3);
    rect.push_back(r4);
    rect.push_back(r5);
    
    int num_frames = duration_in_seconds * frames_per_second;
    for (int i = 0; i < num_frames; ++i) {
        double dt = i / frames_per_second;
        frame.clear();
        for (int j = 0; j < rect.size(); ++j) {
            rect[j].draw(frame);
            rect[j].update(dt);
        }
        frame.write(pipe);
    }
    
    fflush(pipe);
#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    std::cout << "num_frames: " << num_frames << std::endl;
    std::cout << "Done." << std::endl;
    return 0;
}
