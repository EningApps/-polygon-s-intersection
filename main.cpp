#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include "utils.h"
#include "cohen_sutherland_algorithm.h"

#define n 60 // number of lines to form circle



struct Circle{
    point * coordinates;
    point center;
    float radius;
};

bool isInRect(ClippingRectangle rect, point p){
    if( (rect.x1 <= p.x && p.x <= rect.x2) && (rect.y1 <= p.y && p.y <= rect.y2))
        return true;
    else
        return false;
}


bool isInCircle(Circle circle, point p){
    float value = (p.x - circle.center.x) * (p.x - circle.center.x) + (p.y - circle.center.y)*(p.y - circle.center.y) ;
    if(value <= (circle.radius *  circle.radius))
        return true;
    else
        return false;
}

bool isRectCorner(ClippingRectangle rect, float x,float y){
    printf(" X:%1.0lf Y:%1.0lf\n",x,y);
    printf(" 1: x:%1.0lf y: %1.0lf\n",rect.x1,rect.y1);
    printf(" 2: x:%1.0lf y: %1.0lf\n",rect.x1,rect.y2);
    printf(" 3: x:%1.0lf y: %1.0lf\n",rect.x2,rect.y2);
    printf(" 4: x:%1.0lf y: %1.0lf\n",rect.x2,rect.y1);
    for (int i=-5; i <= 5; i++) {
        for (int j=-5; j <= 5; j++) {
            
            if((x == rect.x1+j && y == rect.y1+i)||
               (x == rect.x1+j && y == rect.y2+i)||
               (x == rect.x2+j && y == rect.y2+i)||
               (x == rect.x2+j && y == rect.y1+i)){
                
                printf("YES\n");
                    return true;
            }
        }
    }
    return false;
}

std::vector<point> findBorderPoints(Circle circle, ClippingRectangle rect , ClippingRectangle rectWindow){
    
    std::vector<point> points;
    for (int i=0;i<n;i++){
        point bufPoint1, bufPoint2;
        bufPoint1 = circle.coordinates[i];
        bufPoint2 = circle.coordinates[i+1];
        cohen_sutherland(&rect , &bufPoint1, &bufPoint2);
        if((bufPoint1.x!=circle.coordinates[i].x ||  bufPoint2.x!=circle.coordinates[i+1].x || bufPoint1.y!=circle.coordinates[i].y ||  bufPoint2.y!=circle.coordinates[i+1].y) &&
          (!isRectCorner(rect, circle.coordinates[i].x, circle.coordinates[i].y) || !isRectCorner(rect, circle.coordinates[i+1].x, circle.coordinates[i+1].y))){
            //if(!isRectCorner(rect,circle.coordinates[i].x,circle.coordinates[i].y))
                if(isInRect(rectWindow, bufPoint2)){
                    //printf(" 2 - X:%1.0lf Y: %1.0lf\n",bufPoint2.x,bufPoint2.y);
                    points.push_back(bufPoint2);
                }
           // if(!isRectCorner(rect, circle.coordinates[i].x,circle.coordinates[i].y))
                if(isInRect(rectWindow, bufPoint1)){
                    // printf("1 - X:%1.0lf Y: %1.0lf\n",bufPoint1.x,bufPoint1.y);
                    points.push_back(bufPoint1);
                }
        }
        bufPoint1 = circle.coordinates[i];
        bufPoint2 = circle.coordinates[i+1];
        cohen_sutherland(&rectWindow , &bufPoint1, &bufPoint2);
        if( bufPoint1.x!=circle.coordinates[i].x ||  bufPoint2.x!=circle.coordinates[i+1].x || bufPoint1.y!=circle.coordinates[i].y ||  bufPoint2.y!=circle.coordinates[i+1].y){
  
            //if(!isRectCorner(rect,circle.coordinates[i].x,circle.coordinates[i].y))
                if(!isInRect(rect, bufPoint2))
                    points.push_back(bufPoint2);
            //if(!isRectCorner(rect,circle.coordinates[i].x,circle.coordinates[i].y))
                if(!isInRect(rect, bufPoint1))
                    points.push_back(bufPoint1);

        }
        
    }
    
  
    return points;
}



Circle buildCircle(float radius, float xCenter, float yCenter){
    Circle circle;
    circle.coordinates = new point [n+1];
    circle.radius = radius;
    circle.center.x = xCenter;
    circle.center.y = yCenter;
    float R=radius;
    float angle=0;
    for (int i=0;i<=n;i++)
    {
        
        circle.coordinates[i].x=circle.center.x+R*cos(angle*M_PI/180);
        circle.coordinates[i].y=circle.center.y+R*sin(angle*M_PI/180);
        angle+= 360/n;
    }
    
    circle.coordinates[n].x = circle.coordinates[0].x;
    circle.coordinates[n].y = circle.coordinates[0].y;
    return circle;
    
}

void drawCircle(SDL_Renderer *renderer, Circle circle, ClippingRectangle rect , ClippingRectangle rectWindow){
    
    bool isDashed = isInRect(rect, circle.coordinates[0]) || !isInRect(rectWindow, circle.coordinates[0]) ;
   
    std::vector<point> borderPoints = findBorderPoints(circle, rect, rectWindow);
    for (int i=0;i<n;i++){
                for (int j =0; j<borderPoints.size(); j++) {
        
                    if(borderPoints[j].x == circle.coordinates[i].x && borderPoints[j].y == circle.coordinates[i].y){
                        if(isDashed == false){
                            isDashed = true;
                        }
                        else{
                            isDashed = false;
                        }
                    }
                }
                if(isDashed && i%2==0)
                    continue;
                else
                    drawLine(circle.coordinates[i].x, circle.coordinates[i].y, circle.coordinates[i+1].x, circle.coordinates[i+1].y, renderer);
    }
}


void drawClippedLine(SDL_Renderer *renderer,ClippingRectangle rect,ClippingRectangle rectWindow, point bufPoint1, point bufPoint2,int x1,int x2,int y1,int y2){
    if(cohen_sutherland(&rectWindow , &bufPoint1, &bufPoint2)==0){
        if(x1!=bufPoint1.x || y1!=bufPoint1.y){
            drawDashedLine(x1, y1, bufPoint1.x, bufPoint1.y, renderer);
            drawLine(bufPoint1.x, bufPoint1.y, x2, y2, renderer);
        }else if(x2!=bufPoint2.x || y2!=bufPoint2.y){
            drawLine(bufPoint1.x, bufPoint1.y, bufPoint2.x, bufPoint2.y, renderer);
            drawDashedLine(bufPoint2.x, bufPoint2.y, x2, y2, renderer);
        }else{
            drawLine(bufPoint1.x, bufPoint1.y, bufPoint2.x, bufPoint2.y, renderer);
        }
    }else{
        if(isInRect(rectWindow, bufPoint1)){
            drawLine(bufPoint1.x, bufPoint1.y, bufPoint2.x, bufPoint2.y, renderer);
        }else{
            drawDashedLine(bufPoint1.x, bufPoint1.y, bufPoint2.x, bufPoint2.y, renderer);
        }
    }
}

void drawRectWithClipping(SDL_Renderer *renderer,ClippingRectangle rect,ClippingRectangle rectWindow){
    
    point bufPoint1, bufPoint2;
    float x1 , x2 , y1 , y2;
    
    x1 = rect.x1;
    y1 = rect.y1;
    x2 = rect.x1;
    y2 = rect.y2;
    bufPoint1.x = x1;
    bufPoint1.y = y1;
    bufPoint2.x = x2;
    bufPoint2.y = y2;
    drawClippedLine(renderer, rect , rectWindow, bufPoint1, bufPoint2, x1, x2, y1, y2);

    x1 = rect.x1;
    y1 = rect.y2;
    x2 = rect.x2;
    y2 = rect.y2;
    bufPoint1.x = x1;
    bufPoint1.y = y1;
    bufPoint2.x = x2;
    bufPoint2.y = y2;
    drawClippedLine(renderer, rect , rectWindow, bufPoint1, bufPoint2, x1, x2, y1, y2);
    
    x1 = rect.x2;
    y1 = rect.y2;
    x2 = rect.x2;
    y2 = rect.y1;
    bufPoint1.x = x1;
    bufPoint1.y = y1;
    bufPoint2.x = x2;
    bufPoint2.y = y2;
    drawClippedLine(renderer, rect , rectWindow, bufPoint1, bufPoint2, x1, x2, y1, y2);

    x1 = rect.x2;
    y1 = rect.y1;
    x2 = rect.x1;
    y2 = rect.y1;
    bufPoint1.x = x1;
    bufPoint1.y = y1;
    bufPoint2.x = x2;
    bufPoint2.y = y2;
    drawClippedLine(renderer, rect , rectWindow, bufPoint1, bufPoint2, x1, x2, y1, y2);
}



void drawRect(SDL_Renderer *renderer,ClippingRectangle rect){
    
    drawLine(rect.x1, rect.y1, rect.x1, rect.y2, renderer);
    drawLine(rect.x1, rect.y2, rect.x2, rect.y2, renderer);
    drawLine(rect.x2, rect.y2, rect.x2, rect.y1, renderer);
    drawLine(rect.x2, rect.y1, rect.x1, rect.y1, renderer);
}

int main(int argc, char *args[])
{
    
    bool rect_to_be_dragged = false;
    bool circle_to_be_dragged = false;
    
    
    point bufPoint;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGTH, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    ClippingRectangle rectangle;
    rectangle.x1 = 150;
    rectangle.x2 = 700;
    rectangle.y1 = 200;
    rectangle.y2 = 350;
    
    ClippingRectangle rectangleClippingWindow;
    rectangleClippingWindow.x1 = 100;
    rectangleClippingWindow.x2 = 950;
    rectangleClippingWindow.y1 = 100;
    rectangleClippingWindow.y2 = 650;
    
    drawRect(renderer, rectangleClippingWindow);
    drawRectWithClipping(renderer, rectangle, rectangleClippingWindow);
    
    Circle circle = buildCircle(250, SCREEN_WIDTH/2, SCREEN_HEIGTH/2);
    drawCircle(renderer, circle,rectangle , rectangleClippingWindow);

    SDL_RenderPresent(renderer);

    bool done = false;
    while (!done)
    {
        while(SDL_PollEvent(&event)) {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    
                    bufPoint.x = event.motion.x;
                    bufPoint.y = event.motion.y;
                    if(isInRect(rectangle, bufPoint))
                        rect_to_be_dragged = true;
                    if(isInCircle(circle, bufPoint))
                        circle_to_be_dragged = true;
                    if(circle_to_be_dragged && rect_to_be_dragged)
                        circle_to_be_dragged = false;
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                    
                    rect_to_be_dragged = false;
                    circle_to_be_dragged = false;
                    break;
                case SDL_MOUSEMOTION:
                    
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
                    std::stringstream ss;
                    ss << "X: " << mouseX << " Y: " << mouseY;
                    SDL_SetWindowTitle(window, ss.str().c_str());
                    
                    if(rect_to_be_dragged){

                        rectangle.x1 += event.motion.xrel;
                        rectangle.x2 += event.motion.xrel;
                        rectangle.y1 += event.motion.yrel;
                        rectangle.y2 += event.motion.yrel;
                        
                    }
                    if(circle_to_be_dragged){
                        circle.center.x += event.motion.xrel;
                        circle.center.y += event.motion.yrel;
                        circle = buildCircle(circle.radius, circle.center.x, circle.center.y);

                    }
                    
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                    SDL_RenderClear(renderer);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    
                    drawRect(renderer, rectangleClippingWindow);
                    drawRectWithClipping(renderer, rectangle, rectangleClippingWindow);
                    drawCircle(renderer, circle,rectangle, rectangleClippingWindow);
                    SDL_RenderPresent(renderer);
                    break;
            }
        }
    }
    return 0;
}
