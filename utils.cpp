#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include "Utils.h"


float** multiplyMatrix(float **coordinates, float B[3][3]){
    float ** newCoordinates = (float**) calloc(5,sizeof(float*));
    for (int i = 0; i< 5; i++) {
        newCoordinates[i] = (float*) calloc(3,sizeof(float));
    }
    
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
                newCoordinates[i][j] += coordinates[i][k] * B[k][j];
        }
    return newCoordinates;
}


void drawLine(int x1, int y1, int x2, int y2, SDL_Renderer *renderer)//Bresenham's line algo
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x2 >= x1 ? 1 : -1;
    int sy = y2 >= y1 ? 1 : -1;
    if (dy <= dx)
    {
        int d = (dy << 1) - dx;
        int d1 = dy << 1;
        int d2 = (dy - dx) << 1;
        
        SDL_RenderDrawPoint(renderer, x1, y1);
        for (int x = x1 + sx, y = y1, i = 1; i <= dx; i++, x += sx)
        {
            if (d > 0)
            {
                d += d2;
                y += sy;
            }
            else
                d += d1;
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    else
    {
        int d = (dx << 1) - dy;
        int d1 = dx << 1;
        int d2 = (dx - dy) << 1;
        SDL_RenderDrawPoint(renderer, x1, y1);
        for (int x = x1, y = y1 + sy, i = 1; i <= dy; i++, y += sy)
        {
            if (d > 0)
            {
                d += d2;
                x += sx;
            }
            else
                d += d1;
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}



