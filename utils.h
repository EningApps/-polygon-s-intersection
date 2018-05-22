#define M 5
#define N 3
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGTH = 720;
const float Xcentr = SCREEN_HEIGTH / 2;
const float Ycentr = SCREEN_WIDTH / 2;

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


void drawDashedLine(int x1, int y1, int x2, int y2, SDL_Renderer *renderer)//Interpritation of resenham's line algo for dashed line
{
    float dx,dy,len,x,y;
    int i;
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    if(dx>=dy)
        len=dx;
    else
        len=dy;
    dx = (x2-x1)/len;
    dy = (y2-y1)/len;
    x = x1 + 0.5;
    y = y1 + 0.5;
    SDL_RenderDrawPoint(renderer, x1, y1);
    SDL_RenderDrawPoint(renderer, x2, y2);
    for(i=0;i<=len;i++){
        if(i%9>4)
            SDL_RenderDrawPoint(renderer, x, y);
        x += dx;
        y += dy;
        
    }
}








