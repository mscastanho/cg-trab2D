#ifndef MISC_H_
#define MISC_H_

typedef struct tColor {
	float r;
	float g;
	float b;
} Color;

const Color BLACK = {0,0,0};
const Color BLUE = {0,0,1};
const Color GREEN = {0,1,0};
const Color CYAN = {0,1,1};
const Color RED = {1,0,0};
const Color MAGENTA = {1,0,1};
const Color YELLOW = {1,1,0};
const Color WHITE = {1,1,1};
const Color GRAY = {0.1,0.1,0.1};

typedef struct tPoint {
	float x;
	float y;
} Point;

#endif /* MISC_H_ */
