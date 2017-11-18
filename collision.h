/*
Library detects various geometric collisions based on the dimensions and sizes passed in

Rectangle -> Rectangle collision
Circle -> Circle collision
Rectangle -> Circle collision
*/

int detectRectCollision(int r1, int c1, int height1, int width1, int r2, int c2, int height2, int width2);
int detectCircleCollision(int r1, int c1, int radius1, int r2, int c2, int radius2);
int detectRectCirCollision(int r1, int c1, int width1, int height1, int r2, int c2, int radius2);