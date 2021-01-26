#include "damage_info.h"

Damage_Info::Damage_Info(Param_Info *pi, Chain_Info *ci)
{
	red_elimi = ci->getElementCountPerColor(Red);
	blue_elimi = ci->getElementCountPerColor(Blue);
	green_elimi = ci->getElementCountPerColor(Green);
	yellow_elimi = ci->getElementCountPerColor(Yellow);
	purple_elimi = ci->getElementCountPerColor(Purple);
	red_magni = ci->getMagnificationPerColor(pi, Red);
	blue_magni = ci->getMagnificationPerColor(pi, Blue);
	green_magni = ci->getMagnificationPerColor(pi, Green);
	yellow_magni = ci->getMagnificationPerColor(pi, Yellow);
	purple_magni = ci->getMagnificationPerColor(pi, Purple);
	wild_magni = ci->getMagnificationPerColor(pi, None);
}

Damage_Info::~Damage_Info()
{

}
