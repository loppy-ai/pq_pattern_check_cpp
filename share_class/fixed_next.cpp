#include "fixed_next.h"

Fixed_Next::Fixed_Next(const int next_color)
{
	setFixedNext(next_color);
}

Fixed_Next::~Fixed_Next()
{

}

void Fixed_Next::setFixedNext(const int next_color)
{
	switch (next_color) {
	case 1:	setNext(next_color_1);	break;
	case 2:	setNext(next_color_2);	break;
	case 3: setNext(next_color_3);	break;
	case 4:	setNext(next_color_4);	break;
	case 5:	setNext(next_color_5);	break;
	case 9:	setNext(next_color_9);	break;
	default:
		std::cout << "Error : next_color‚ÌÝ’è‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·" << std::endl;
		std::cout << std::endl;
		exit(1);
		break;
	}
}