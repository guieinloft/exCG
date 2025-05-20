#include "Editor.h"
#include "Barrel.h"
#include "Chaser.h"
#include "Powerup.h"
#include "Tank.h"
#include "Tower.h"

#include "Vector2.h"
#include "gl_canvas2d.h"
#include "collisions.h"

Editor::Editor() {
	bt_edit = new Button(8, 8, 32, 32);
	bt_edit->loadIcons("Trab3GuilhermeEinloft/images/bt_edit.bmp");
	bt_add = new Button(48, 8, 32, 32);
	bt_add->loadIcons("Trab3GuilhermeEinloft/images/bt_add.bmp");
	bt_remove = new Button(88, 8, 32, 32);
	bt_remove->loadIcons("Trab3GuilhermeEinloft/images/bt_remove.bmp");

	bt_save = new Button(1200, 8, 32, 32);
	bt_save->loadIcons("Trab3GuilhermeEinloft/images/bt_save.bmp");
	bt_back = new Button(1240, 8, 32, 32);
	bt_back->loadIcons("Trab3GuilhermeEinloft/images/bt_back.bmp");

	for (int i = 0; i < ETYPE_FIXED_NUM; i++)
		bt_add_entities[i] = new Button(8 + i * 40, 8, 32, 32);

	bt_add_entities[ETYPE_BARREL]->loadIcons("Trab3GuilhermeEinloft/images/bt_add_barrel.bmp");
	bt_add_entities[ETYPE_CHASER]->loadIcons("Trab3GuilhermeEinloft/images/bt_add_chaser.bmp");
	bt_add_entities[ETYPE_TOWER]->loadIcons("Trab3GuilhermeEinloft/images/bt_add_tower.bmp");
	bt_add_entities[ETYPE_POWERUP_SHIELD]->loadIcons("Trab3GuilhermeEinloft/images/bt_add_powerup_shield.bmp");
	bt_add_entities[ETYPE_POWERUP_HEALTH]->loadIcons("Trab3GuilhermeEinloft/images/bt_add_powerup_health.bmp");
	bt_add_entities[ETYPE_POWERUP_SUPER]->loadIcons("Trab3GuilhermeEinloft/images/bt_add_powerup_super.bmp");

	mouse.state = -2;
	mouse.button = -2;
	curMenu = 0;
	curEntity = -1;
	curPointOuter = -1;
	curPointInner = -1;
}

void Editor::loadLevel(char *path) {
	levelpath = path;
	FILE *level = fopen(path, "r");
	if (level == NULL)
		level = fopen("Trab3GuilhermeEinloft/levels/base/base.trk", "r");

	for (int i = 0; i < 12; i++) {
		fscanf(level, "%f %f %f %f", &p_out[i].x, &p_out[i].y,
			&p_in[i].x, &p_in[i].y);
	}
	b_spline_generate(p_out, 12, b_out);
	b_spline_generate(p_in, 12, b_in);
	EntityType type;
	Vector2 center;
	for (int i = 0; i < ENTITY_FIXED_NUM; i++) {
		if (fscanf(level, "%d", (int*)&type) == EOF)
			break;
		fscanf(level, "%f %f", &center.x, &center.y);
		switch (type) {
		case ETYPE_TANK:
			entities[i] = new Tank(center);
			break;
		case ETYPE_BARREL:
			entities[i] = new Barrel(center, 32);
			break;
		case ETYPE_CHASER:
			entities[i] = new Chaser(center, 0);
			break;
		case ETYPE_TOWER:
			entities[i] = new Tower(center, 0);
			break;
		case ETYPE_POWERUP_SUPER:
			entities[i] = new Powerup(center, ETYPE_POWERUP_SUPER);
			break;
		case ETYPE_POWERUP_SHIELD:
			entities[i] = new Powerup(center, ETYPE_POWERUP_SHIELD);
			break;
		case ETYPE_POWERUP_HEALTH:
			entities[i] = new Powerup(center, ETYPE_POWERUP_HEALTH);
			break;
		default:
			break;
		}
	}
	fclose(level);
}

void Editor::saveLevel() {
	bool collision = false;
	for (int i = 0; i < ENTITY_FIXED_NUM; i++) {
		collision |= collisions[i];
	}
	if (collision) {
		strcpy(errortext, "Alguns objetos estao colidindo");
		return;
	}
	FILE *level = fopen(levelpath, "w");
	for (int i = 0; i < 12; i++) {
		fprintf(level, "%f %f %f %f\n", p_out[i].x, p_out[i].y,
			p_in[i].x, p_in[i].y);
	}

	for (int i = 0; i < ENTITY_FIXED_NUM; i++) {
		if (entities[i] != nullptr) {
			fprintf(level, "%d %f %f\n", entities[i]->getType(),
				entities[i]->getCenter().x,
				entities[i]->getCenter().y);
		}
	}
	fclose(level);
	strcpy(errortext, "Salvo com sucesso!");
}

void Editor::addEntity() {
	int i = 0;
	while (i < ENTITY_FIXED_NUM) {
		if (entities[i] == nullptr)
			break;
		i++;
	}
	if (i == ENTITY_FIXED_NUM) {
		strcpy(errortext, "Nao pode ter mais que 32 objetos");
		return;
	}
	strcpy(errortext, "");
	Vector2 center(mouse.x, mouse.y);
	switch (curEntity) {
	case ETYPE_TANK:
		entities[i] = new Tank(center);
		break;
	case ETYPE_BARREL:
		entities[i] = new Barrel(center, 32);
		break;
	case ETYPE_CHASER:
		entities[i] = new Chaser(center, 0);
		break;
	case ETYPE_TOWER:
		entities[i] = new Tower(center, 0);
		break;
	case ETYPE_POWERUP_SUPER:
		entities[i] = new Powerup(center, ETYPE_POWERUP_SUPER);
		break;
	case ETYPE_POWERUP_SHIELD:
		entities[i] = new Powerup(center, ETYPE_POWERUP_SHIELD);
		break;
	case ETYPE_POWERUP_HEALTH:
		entities[i] = new Powerup(center, ETYPE_POWERUP_HEALTH);
		break;
	default:
		break;
	}
}

bool Editor::moveEntity() {
	if (mouse.button == 0 && mouse.state == 0) {
		int i = 0;
		while (i < ENTITY_FIXED_NUM) {
			if (entities[i] == nullptr) {
				i++;
				continue;
			}
			Vector2 mouse_pos(mouse.x, mouse.y);
			if (collide_point_c(mouse_pos, entities[i]->getSpawnCircle())) {
				curEntity = i;
				break;
			}
			i++;
		}
		if (i == ENTITY_FIXED_NUM) {
			curEntity = -1;
		}
	}
	if (mouse.l && curEntity != -1) {
		entities[curEntity]->setPosition(mouse.x, mouse.y);
		return 1;
	}
	curEntity = -1;
	return 0;
}

bool Editor::removeEntity() {
	if (mouse.button == 0 && mouse.state == 0) {
		for (int i = 0; i < ENTITY_FIXED_NUM; i++) {
			if (entities[i] == nullptr)
				continue;
			Vector2 mouse_pos(mouse.x, mouse.y);
			if (collide_point_c(mouse_pos, entities[i]->getSpawnCircle())) {
				if (i > 0) {
					delete entities[i];
					entities[i] = nullptr;
					return 1;
					strcpy(errortext, "");
				}
				else {
					strcpy(errortext, "Nao pode apagar o tanque");
				}
			}
		}
	}
	return 0;
}

bool Editor::movePointOuter() {
	if (mouse.button == 0 && mouse.state == 0) {
		int i = 0;
		while (i < 12) {
			if (mouse.x >= p_out[i].x - 4
				&& mouse.x <= p_out[i].x + 4
				&& mouse.y >= p_out[i].y - 4
				&& mouse.y <= p_out[i].y + 4) {
				curPointOuter = i;
				break;
			}
			i++;
		}
		if (i == 12) {
			curPointOuter = -1;
		}
	}
	if (mouse.l && curPointOuter != -1) {
		p_out[curPointOuter].set(mouse.x, mouse.y);
		return 1;
	}
	if (mouse.state == 1) {
		b_spline_generate(p_out, 12, b_out);
	}
	curPointOuter = -1;
	return 0;
}

bool Editor::movePointInner() {
	if (mouse.button == 0 && mouse.state == 0) {
		int i = 0;
		while (i < 12) {
			if (mouse.x >= p_in[i].x - 4
				&& mouse.x <= p_in[i].x + 4
				&& mouse.y >= p_in[i].y - 4
				&& mouse.y <= p_in[i].y + 4) {
				curPointInner = i;
				break;
			}
			i++;
		}
		if (i == 12) {
			curPointInner = -1;
		}
	}
	if (mouse.l && curPointInner != -1) {
		p_in[curPointInner].set(mouse.x, mouse.y);
		return 1;
	}
	if (mouse.state == 1) {
		b_spline_generate(p_in, 12, b_in);
	}
	curPointInner = -1;
	return 0;
}

int Editor::update_main() {
	bt_edit->Render();
	bt_add->Render();
	bt_remove->Render();

	bt_save->Render();
	bt_back->Render();

	curEntity = -1;

	if (bt_edit->checkClick(mouse) == 1) {
		curMenu = 1;
		strcpy(errortext, "");
	}
	else if (bt_add->checkClick(mouse) == 1) {
		curMenu = 2;
		strcpy(errortext, "");
	}
	else if (bt_remove->checkClick(mouse) == 1) {
		curMenu = 3;
		strcpy(errortext, "");
	}
	else if (bt_save->checkClick(mouse) == 1) {
		saveLevel();
	}
	else if (bt_back->checkClick(mouse) == 1) {
		return 1;
	}

	return 0;
}

int Editor::update_add() {
	for (int i = 0; i < ETYPE_FIXED_NUM; i++) {
		bt_add_entities[i]->Render();
	}

	bt_back->Render();

	if (bt_back->checkClick(mouse) == 1) {
		strcpy(errortext, "");
		curMenu = 0;
		return 0;
	}
	bool button_selected = false;
	for (int i = 0; i < ETYPE_FIXED_NUM; i++) {
		int bt_status = bt_add_entities[i]->checkClick(mouse);
		if (bt_status == 1) {
			button_selected = true;
			curEntity = i;
		}
	}
	for (int i = 0; i < ETYPE_FIXED_NUM; i++) {
		bt_add_entities[i]->select(i == curEntity);
	}
	if (button_selected)
		return 0;
	if (mouse.button == 0 && mouse.state == 0)
		addEntity();

	return 0;
}

int Editor::update_edit() {
	bt_back->Render();
	if (bt_back->checkClick(mouse) == 1) {
		strcpy(errortext, "");
		curMenu = 0;
		b_spline_generate(p_out, 12, b_out);
        b_spline_generate(p_in, 12, b_in);
		return 0;
	}
	if (moveEntity())
		return 0;
	if (movePointOuter())
		return 0;
	if (movePointInner())
		return 0;

	return 0;
}

int Editor::update_remove() {
	bt_back->Render();
	if (bt_back->checkClick(mouse) == 1) {
		strcpy(errortext, "");
		curMenu = 0;
		return 0;
	}
	removeEntity();

	return 0;
}

int Editor::update() {
	CV::color(0.5f, 0.5f, 0.5f);
	CV::rectFill(0, 0, 1280, 720);
	b_spline_draw(b_out, b_in);
	b_spline_points_draw(p_out, p_in);
	for (int i = 0; i < ENTITY_FIXED_NUM; i++) {
		if (entities[i] == nullptr)
			continue;
		entities[i]->render(false);
		entities[i]->renderSpawnCircle(collisions[i]);
	}
	checkSpawnCollisions(entities, collisions);
	checkSpawnCollisionsBorder(entities, collisions, b_out, b_in);
	CV::color(1, 1, 1);
	CV::text(900, 20, errortext);

	int ret = 0;
	switch (curMenu) {
	case 0:
		ret = update_main();
		break;
	case 1:
		ret = update_edit();
		break;
	case 2:
		ret = update_add();
		break;
	case 3:
		ret = update_remove();
		break;
	}
	mouse.button = -2;
	mouse.state = -2;
	return ret;
}

void Editor::checkMouse(int button, int state, int wheel, int direction, int x, int y) {
	mouse.xp = mouse.x;
	mouse.yp = mouse.y;
	mouse.x = x;
	mouse.y = y;
	mouse.button = button;
	mouse.state = state;
	mouse.wheel = wheel;
	mouse.direction = direction;
	switch (button) {
		case 0:
		mouse.l = !state;
		break;
		case 1:
		mouse.m = !state;
		break;
		case 2:
		mouse.r = !state;
		break;
	}
}

