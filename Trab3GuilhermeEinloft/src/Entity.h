#ifndef ENTITY_H
#define ENTITY_H

#include "CollisionShape.h"
#include "Circle.h"
#include "HealthBar.h"

//tamanho do array
#define ENTITY_NUM 64
//tamanho da parte fixa do array (criada no inicio da execucao)
#define ENTITY_FIXED_NUM 32

/* 
 * Tipos de entidade
 * usados principalmente no carregamento/salvamento
 */
enum EntityType {
	ETYPE_BARREL,
	ETYPE_CHASER,
	ETYPE_TOWER,
	ETYPE_POWERUP_SHIELD,
	ETYPE_POWERUP_HEALTH,
	ETYPE_POWERUP_SUPER,
	ETYPE_FIXED_NUM,
	ETYPE_MISSILE,
	ETYPE_EXPLOSION,
	ETYPE_TANK = -1
};

class Entity {
protected:
	//centro do objeto
	Vector2 center;
	//circulo de limite de spawn (distancia minima entre outras entidades)
	Circle scircle;
	//forma de colisao (pode ser composta, formada por varios poligonos)
	CollisionShape cshape;
	//barra de vida
	HealthBar hbar;
	//tipo da entidade
	EntityType type;

public:
	//construtor e destrutor
	Entity(Vector2 center, float radius, int max_health, EntityType type);
	virtual ~Entity();
	//tratador de colisao (chamada caso a entidade colidir)
	virtual bool hit();
	//tratador de movimento (chamada a cada update)
	//recebe um alvo caso a entidade tenha
	//e o array de entidades p/ spawnar misseis caso spawne
	virtual bool move(Vector2 target, float deltaTime, Entity **entities);
	//render (mostrar ou nao barra de vida)
	virtual void render(bool show_hbar = true);
	//definir posicao (usado no editor)
	virtual void setPosition(float x, float y);
	//getters
	CollisionShape getCollisionShape();
	Vector2 getCenter();
	Circle getSpawnCircle();
	EntityType getType();
	int getMaxHealth();
	//retorna se eh powerup
	bool isPowerup();
	//renderiza circulo de spawn no editor
	virtual void renderSpawnCircle(bool colliding);
};

#endif
