#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

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

 */


int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    VoxObj obj1;

    double angleZ = 0;
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

    bool run=true;
    while (run)
    {
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

        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;

        angleZ += 0.02 * ellapsed_time;
        //angleY += 0.023 * ellapsed_time;
        angleX += 0.027 * ellapsed_time;

        //obj1.draw_slow(angleX,angleY,angleZ);
        obj1.draw_slow_octree(angleX,angleY,angleZ);

        ellapsed_time = SDL_GetTicks() - start_time;
        if (ellapsed_time < 10)
        {
            SDL_Delay(10 - ellapsed_time);
        }

    }

    return 0;
}
