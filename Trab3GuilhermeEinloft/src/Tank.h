#ifndef TANK__H
#define TANK__H

#include "Vector2.h"
#include "Poly.h"
#include "Mouse.h"
#include "Keys.h"
#include "Missile.h"
#include "Entity.h"
#include "Powerup.h"

class Tank : public Entity {
protected:
	//corpo do tanque
	Poly body;
	//canhao do tanque
	Poly cannon;
	//velocidade
	float speed;
	//angulo do tanque
	float angle;
	//incremento do angulo
	float angle_inc;
	//angulo do canhao
	float c_angle;
	//posicao do mouse
	Vector2 m_pos;
	//posicao da ponta do canhao
	//onde spawnar misseis
	Vector2 cannon_pos;
	//tempo de espera entre misseis
	int cooldown;
	//imunidade pos hit
	int invincibility;
	//define se deve ser arremessado pos hit
	bool knockback;
	//direcao do arremesso
	int knockback_dir;
	//define se esta no estado super
	bool super;
	//barra de vida do escudo
	HealthBar sbar;

public:
	Tank(Vector2 center);
	//trata teclado
	void checkKeys(Keys keys);
	//trata mouse (atira misseis)
	void checkMouse(Mouse mouse, Entity *entities[]);	
	void render(bool show_hbar = true);
	bool move(Vector2 target, float deltaTime, Entity **entities);
	bool hit();
	//funcao de hit especial caso bata na barreira
	bool hit_border(int dir);
	//funcao de hit especial caso bata em um powerup
	//define qual powerup o tanque recebe
	void hit_powerup(EntityType type);
	void setPosition(float x, float y);
};

#endif
