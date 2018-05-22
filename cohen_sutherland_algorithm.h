#define LEFT  1  /* binary 0001 */
#define RIGHT 2  /* binary 0010 */
#define BOT   4  /* binary 0100 */
#define TOP   8  /* binary 1000 */

struct point {
    double x, y;
};


struct ClippingRectangle {
    double x1, y1, x2, y2;
};

/* computing point's code
 r : pointer to struct clipping_rectangle; p : pointer to struct point */
#define vcode(r, p) \
((((p)->x < (r)->x1) ? LEFT : 0)  + /* +1 if point to the left of rectangle */ \
(((p)->x > (r)->x2) ? RIGHT : 0) +  /* +2 if point to the right of rectangle  */\
(((p)->y < (r)->y1) ? BOT : 0)   +  /* +4 if point is below */  \
(((p)->y > (r)->y2) ? TOP : 0))     /* +8 if point is above */

/* if line ab doesn't cross rectangle r function returns -1;
 if line ab crosses rectangle r function returns -1 and clippes
 parts of line out of rectangle*/
int cohen_sutherland (const struct ClippingRectangle *r, struct point * pointA , struct point * pointB)
{
    int code_a, code_b, buf_code;
    struct point *c;
    
    code_a = vcode(r, pointA);
    code_b = vcode(r, pointB);
    
    while (code_a | code_b) {
        if (code_a & code_b)
            return -1;
        
        if (code_a) {
            buf_code = code_a;
            c = pointA;
        } else {
            buf_code = code_b;
            c = pointB;
        }
        if (buf_code & LEFT) {
            c->y += (pointA->y - pointB->y) * (r->x1 - c->x) / (pointA->x - pointB->x);
            c->x = r->x1;
        } else if (buf_code & RIGHT) {
            c->y += (pointA->y - pointB->y) * (r->x2 - c->x) / (pointA->x - pointB->x);
            c->x = r->x2;
        }
        else if (buf_code & BOT) {
            c->x += (pointA->x - pointB->x) * (r->y1 - c->y) / (pointA->y - pointB->y);
            c->y = r->y1;
        } else if (buf_code & TOP) {
            c->x += (pointA->x - pointB->x) * (r->y2 - c->y) / (pointA->y - pointB->y);
            c->y = r->y2;
        }
          if (buf_code == code_a) {
            pointA = c;
            code_a = vcode(r,pointA);
        } else {
            pointB = c;
            code_b = vcode(r,pointB);
        }
    }
    
    return 0;
}

