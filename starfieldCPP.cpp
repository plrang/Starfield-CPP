// Subject: Starfield CPP demo for Windows SDL
// Author: PLRANG
// URL: https://plrang.com
// Technology: C++, Converted from JAVA and modified.
// V: 1.0.1 prototype, working version: 24-02-2023
// "Forked" from Benny: https://github.com/BennyQBD
// DOCS: https://www.youtube.com/watch?v=v7nrzvd9A5c

//  MIT License
// Copyright (c) [2017-2023] [Plrang]
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Parts of the code are commented out for testing and further development .
// It's an unoptimized prototype. Yet it works pretty well.
// There are also some "thinking process" comments left and comments from Benny.

// TODO: fix the double window
// TODO: fix closing the app

#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <vector>
#include <math.h>

// #include <emscripten.h>
// some things commented for using in esp8266
// #include <ctime>
// #include <chrono>
//
// #include <iostream>
// #include <sys/time.h>
//
// #include <unistd.h>

// COMPILE IT
// g++ -o starfieldCPP.exe starfieldCPP.cpp -I C:\SDL2\SDL2-2.0.4\x86_64-w64-mingw32\include\SDL2 -L C:\SDL2\SDL2-2.0.4\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 -Wall -mwindows

// #include <cstdlib> // for rand() and srand()
// #include <ctime> // for time()

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

/*     */

class Stars3D
{

    /** How much the stars are spread out in 3D space, on average. */
    float m_spread;

    /** How quickly the stars move towards the camera */
    float m_speed;

    /** The star positions on the X axis */
    std::vector<float> m_starX;

    /** The star positions on the Y axis */
    std::vector<float> m_starY;

    /** The star positions on the Z axis */
    std::vector<float> m_starZ;

    float m_angle = 0.0f;
    float m_angle_delta = 0.0f;

    /**
     * Creates a new 3D star field in a usable state.
     *
     * @param numStars The number of stars in the star field
     * @param spread   How much the stars spread out, on average.
     * @param speed    How quickly the stars move towards the camera
     */

public:
    Stars3D(int numStars, float spread, float speed)
    {

        m_spread = spread;
        m_speed = speed;

        m_starX.resize(numStars);
        m_starY.resize(numStars);
        m_starZ.resize(numStars);

        std::cout << "INIT CLASS size:" << m_starX.size() << " vs " << numStars << std::endl;

        // for(int i = 0; i < m_starX.size(); i++)

        for (int i = 0; i < numStars; i++)
        {
            // std::cout <<  "INIT:" << i << std::endl;
            InitStar(i);
        }
    }

    /**
     * Initializes a star to a new pseudo-random location in 3D space.
     *
     * @param i The index of the star to initialize.
     */
    void InitStar(int i)
    {
        // The random values have 0.5 subtracted from them and are multiplied
        // by 2 to remap them from the range (0, 1) to (-1, 1).
        //
        //  rand() / (RAND_MAX)  RAND_MAX, a constant in cstdlib that is typically set to 32767
        //  http://www.learncpp.com/cpp-tutorial/59-random-number-generation/

        m_starX[i] = 2 * ((float)((double)rand() / (double)RAND_MAX) - 0.5f) * m_spread;
        m_starY[i] = 2 * ((float)((double)rand() / (double)RAND_MAX) - 0.5f) * m_spread;

        // For Z, the random value is only adjusted by a small amount to stop
        // a star from being generated at 0 on Z.
        m_starZ[i] = ((float)((double)rand() / (double)RAND_MAX) + 0.00001f) * m_spread;

        // std::cout <<  "INIT m_starY[i]" << m_starY[i]  << std::endl;
    }

    /**
     * Updates every star to a new position, and draws the starfield in a
     * bitmap.
     *
     * @param target The bitmap to render to.
     * @param delta  How much time has passed since the last update.
     */

    void UpdateAndRender(SDL_Renderer *target, float delta)
    {

        // std::cout << "UpdateAndRender " << "m_starX.size() " << m_starX.size() << "\n";

        // Stars are drawn on a black background
        // target.Clear((byte)0x00);

        // float halfWidth  = target.GetWidth()/2.0f;
        // float halfHeight = target.GetHeight()/2.0f;

        float halfWidth = SCREEN_WIDTH / 2.0f;
        float halfHeight = SCREEN_HEIGHT / 2.0f;

        SDL_Rect r;
        int green_val;

        m_angle_delta += 0.0001f;

        if (m_angle_delta > 360.0f)
            m_angle_delta = 0.0f;

        for (unsigned int i = 0; i < m_starX.size(); i++)
        {

            // Update the Star.

            // Move the star towards the camera which is at 0 on Z.
            m_starZ[i] -= delta * m_speed;

            // std::cout << "delta * m_speed " << delta * m_speed << "\n";

            // If star is at or behind the camera, generate a new position for
            // it.
            if (m_starZ[i] <= 0)
            {
                InitStar(i);
            }

            // Render the Star.

            //  Multiplying the position by (size/2) and then adding (size/2)
            //  remaps the positions from range (-1, 1) to (0, size)

            //          int x = (int)((m_starX[i]/m_starZ[i]) * halfWidth + halfWidth);
            //          int y = (int)((m_starY[i]/m_starZ[i]) * halfHeight + halfHeight);

            m_angle = atan2((float)m_starY[i] / m_starZ[i], (float)m_starX[i] / m_starZ[i]);

            m_angle = (m_angle + m_angle_delta) * (180 / 3.14159265);

            // std::cout << m_angle << " "  << m_starY[i] / m_starZ[i] << " " << m_starX[i] / m_starZ[i] << std::endl;

            //  the parametric equation of a circle
            //  x=r*cos(fi);
            //  y=r*sin(fi);

            float circle_r = sqrt(pow(m_starX[i] / m_starZ[i], 2) + pow(m_starY[i] / m_starZ[i], 2));

            int x = static_cast<int>(circle_r * cos(m_angle) * halfWidth + halfWidth);
            int y = static_cast<int>(circle_r * sin(m_angle) * halfHeight + halfHeight);

            //                std::cout << m_angle << std::endl;
            //                SDL_Delay( 100 );

            // If the star is not within range of the screen, then generate a
            // new position for it.
            // if(x < 0 || x >= target.GetWidth() || (y < 0 || y >= target.GetHeight()))

            if (x < 0 || x >= SCREEN_WIDTH || (y < 0 || y >= SCREEN_HEIGHT))
            {
                InitStar(i);
            }
            else
            {
                // Otherwise, it is safe to draw this star to the screen.
                // target.DrawPixel(x, y, (byte)0xFF, (byte)0xFF, (byte)0xFF, (byte)0xFF);

                // SDL_RenderDrawPoint(target, x, y);

                r.w = 5 - m_starZ[i] * 0.05;
                r.h = 5 - m_starZ[i] * 0.05;

                r.x = x;
                r.y = y;

                green_val = (r.w * 50);

                SDL_SetRenderDrawColor(target, r.w * 5, green_val, r.w * 5, 255);

                SDL_RenderFillRect(target, &r);
            }
        }

        // SDL_RenderPresent(target);
    }
};

/*     */

int main(int argc, char *argv[])
{
    using namespace std; // no more std:: - but the best is to use it in a function, not globally

    printf("START");

    srand(4391); // set initial seed value to 4391

    std::cout << "STARS";

    // srand(static_cast<unsigned int>(time(0))); // set initial seed value to system clock

    /*
    auto t1 = std::chrono::high_resolution_clock::now();
    //process to be timed

    long z = 0;
    for (int i = 1; i<=10000000;i++)
        {
            z=(z*5+i)/i;
        };

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "process took: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
        << " milliseconds\n";


    int y;
    std::cin >> y;
    return 0;

    */

    // The window we'll be rendering to
    SDL_Window *window;

    // The surface contained by the window
    // SDL_Surface* screenSurface = NULL;

    SDL_Renderer *renderer;

    SDL_Event event;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

        renderer = SDL_CreateRenderer(window, -1, 0);

        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {

            // Get window surface
            //  screenSurface = SDL_GetWindowSurface( window );

            // Fill the surface white
            //  SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );

            // Update the surface
            // SDL_UpdateWindowSurface( window );

            SDL_SetRenderDrawColor(renderer, 128, 255, 128, 255);
            SDL_RenderDrawPoint(renderer, 200, 200);

            std::cout << "STARS" << std::endl;

            Stars3D stars{512, 64.0f, 20.0f}; // 4096

            // time_t previousTime;
            //  time(&previousTime);

            /*
              char buffer[30];
              struct timeval tv;
              time_t curTime, prevTime;

              gettimeofday(&tv, NULL);
              curTime=tv.tv_sec;

              strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curTime));
              printf("%s%ld\n",buffer,tv.tv_usec);

            std::cout << "ms " <<tv.tv_usec<<endl;

            float currentTime, previousTime;
            previousTime = float(tv.tv_usec);
                */

            // auto t1 = std::chrono::high_resolution_clock::now();

            // auto t2 = std::chrono::high_resolution_clock::now();

            float delta = 0;

            int green_val, red_val;

            // bool quit = false;

            for (long i = 0; i < 10000000; i += 1)
            // while(1)
            //  while(!quit)
            {

                green_val = abs(255 - (i / 5 + 255) % 512) / 2;
                red_val = abs(255 - (i / 5 + 255) % 512) / 4;

                SDL_SetRenderDrawColor(renderer, red_val, 0, green_val, 255);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 128, 255, 200, 255);

                SDL_PumpEvents();

                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                    case SDL_QUIT:
                        // quit = true;
                        break;
                        // handle other events as needed
                    }
                }
                // time_t currentTime;
                // time(&currentTime);

                /*
                gettimeofday(&tv, NULL);
                curTime=tv.tv_sec;
                localtime(&curTime);
                currentTime = tv.tv_usec;
                */

                //                        t2 = std::chrono::high_resolution_clock::now();
                // float delta = ((currentTime - previousTime)/1000000000.0);

                // delta = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()/100000.0;
                delta = 0.01;

                // std::cout <<  "currentTime " << currentTime << std::endl;
                // std::cout << "ITER: " << i << " delta " << delta << " green_val " << green_val << " red_val " << red_val << std::endl;

                stars.UpdateAndRender(renderer, delta);

                SDL_RenderPresent(renderer);
                SDL_Delay(10);
            }

            // drawStarField();

            // Wait two seconds
            SDL_Delay(10000);
        }
    }

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
