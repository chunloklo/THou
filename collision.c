#include <stdlib.h>

/*
Library detects various geometric collisions based on the dimensions and sizes passed in

Rectangle -> Rectangle collision
Circle -> Circle collision
Rectangle -> Circle collision
*/
int detectRectCollision(int r1, int c1, int width1, int height1, int r2, int c2, int width2, int height2)
{
	if (r1 < r2 + height2 && r1+ height1> r2 && c1 < c2 + width2 && width1 + c1 > c2)
	{
    	return 1;
	}
	return 0;

}

int detectCircleCollision(int r1, int c1, int radius1, int r2, int c2, int radius2)
{
	int dr = r1 - r2;
	int dc = c1 - c2;
	int distance = dr*dr + dc*dc;
	int radii = radius1 + radius2;
	if (distance < radii * radii)
	{
		return 1;
	} 
	return 0;
}

int detectRectCirCollision(int r1, int c1, int width1, int height1, int r2, int c2, int radius2)
{
	int distR = abs(r2 - r1 - r1/2);
    int distC = abs(c2 - c1 - c1/2);

    if (distR > (height1/2 + radius2)) { return 0; }
    if (distC > (width1/2 + radius2)) { return 0; }

    if (distR <= (height1/2)) { return 1; } 
    if (distC <= (width1/2)) { return 1; }

    int dr=distR-width1/2;
    int dc=distC-height1/2;
    return (dr*dr+dc*dc<=(radius2*radius2));
}