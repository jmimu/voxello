#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>

#include "voxobj.h"

#define RES_X 640
#define RES_Y 480

/*
 data/pawn.vox

 without ocrtee optimization:
Total: 3062 voxels
Octree: 5353 cells
Octree: 3062 rendered cells

Total: 3062 voxels
Octree: 4489 cells
Octree: 2306 rendered cells


data/duke.vox: got 36*46*76voxels
Total :   125856 voxels
Simple:    17453 rendered voxels
Octree:    18609 cells
Octree:    16283 terminal cells
Octree:     9242 rendered cells
Memory:   125856 bytes for voxels
Memory:  1414284 bytes for octree
May save: 521056 bytes by changing terminal cells
 */


int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    VoxObj obj1;

    double angleZ = 3.14;
    double angleY = 0;
    double angleX = 0;


    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(RES_X, RES_Y, 32, SDL_OPENGL);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)RES_X/RES_Y,1,1000);

    glEnable(GL_DEPTH_TEST);

    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;

    Uint32 previous_fps_time=SDL_GetTicks()/1000;
    int fps=0;

    bool run=true;
    while (run)
    {
        fps++;

        start_time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {

            switch(event.type)
            {
                case SDL_QUIT:
                exit(0);
                break;

                case SDL_KEYDOWN:
                run = !run;
                break;

            }
        }

        if (previous_fps_time!=current_time/1000)
        {
            std::cout<<"FPS: "<<fps<<std::endl;
            previous_fps_time=current_time/1000;
            fps=0;
        }

        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;

        angleZ += 0.02 * ellapsed_time;
        //angleY += 0.023 * ellapsed_time;
        //angleX += 0.027 * ellapsed_time;

        //obj1.draw_slow(angleX,angleY,angleZ);
        obj1.draw_slow_octree(angleX,angleY,angleZ);

        ellapsed_time = SDL_GetTicks() - start_time;
        if (ellapsed_time < 20)
        {
            //SDL_Delay(20 - ellapsed_time);
        }

    }

    return 0;
}
