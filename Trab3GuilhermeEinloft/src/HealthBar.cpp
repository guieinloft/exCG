#include "HealthBar.h"
#include "Vector2.h"
#include "gl_canvas2d.h"

HealthBar::HealthBar() {
}

void HealthBar::render(Vector2 center, bool hide, int scale,
	float r, float g, float b) {
	if (health == max_health && hide) return;
	if (health <= 0) return;
	float x1, x2, x3, y1, y2;
	x1 = center.x - max_health * 0.5 * scale;
	y1 = center.y - 0.5 * scale;
	x2 = center.x + max_health * 0.5 * scale;
	y2 = center.y + 0.5 * scale;
	x3 = center.x + (health - max_health * 0.5) * scale;
	CV::color(0.f, 0.f, 0.f);
	CV::rectFill(x1, y1, x2, y2);
	CV::color(r, g, b);
	CV::rectFill(x1, y1, x3, y2);
	CV::color(1.f, 1.f, 1.f);
	CV::rect(x1, y1, x2, y2);
}
