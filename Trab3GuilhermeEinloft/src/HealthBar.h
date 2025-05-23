#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "Vector2.h"

class HealthBar {
public:
	//vida atual
	int health;
	//vida maxima
	int max_health;

	HealthBar();
	//renderiza com cor diferente
	//hide = esconde se estiver com vida cheia
	void render(Vector2 center, bool hide = true, int scale = 8,
			float r = 0.3f, float g = 0.75f, float b = 0.3f);
};

#endif
