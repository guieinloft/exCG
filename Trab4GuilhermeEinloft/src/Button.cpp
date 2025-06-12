#include "Button.h"
#include "gl_canvas2d.h"
#include "Mouse.h"

#include <string.h>

Button::Button(int x, int y, int w, int h, bool sticky) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	strcpy(this->text, "");
	this->pressed = false;
	this->selected = false;
	this->hovered = false;
	this->sticky = sticky;
}

int Button::checkClick(Mouse mouse) {
	this->hovered = true;
	if (mouse.x < this->x) this->hovered = false;
	if (mouse.x > this->x + this->w) this->hovered = false;
	if (mouse.y < this->y) this->hovered = false;
	if (mouse.y > this->y + this->h) this->hovered = false;

	if (mouse.button == 0 && mouse.state == 0) {
		if (this->hovered) {
			this->pressed = 1;
			this->selected = 1;
			return 1; //click in button
		}
		else {
			this->pressed = 0;
			this->selected = 0;
			return -1; //click not in button
		}
	}
	else if (mouse.state == 1) {
		this->pressed = 0;
		this->selected = this->selected && sticky;
		return 0; //click released
	}
	else return -2; //not clicked
}

void Button::Render() {
	if (this->selected) CV::color(0.8, 0.8, 0.8);
	else if (this->hovered) CV::color(1.0, 1.0, 1.0);
	else CV::color(0.9, 0.9, 0.9);
	CV::rectFill(this->x, this->y, this->x + this->w, this->y + this->h);
	CV::color(0, 0, 0);
	CV::rect(this->x, this->y, this->x + this->w, this->y + this->h);
	CV::text(this->x + 4, this->y + this->h - 4, this->text);
}

bool Button::isPressed() {
	bool p = this->pressed;
	this->pressed = false;
	return p;
}

bool Button::isSelected() {
	return this->selected;
}

void Button::select(bool s) {
	this->selected = s;
}

void Button::changeText(const char *text) {
	strcpy(this->text, text);
}

void Button::changePosition(int x, int y) {
	this->x = x;
	this->y = y;
}
