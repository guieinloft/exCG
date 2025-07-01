/*
 * vec3
 * guarda vetor (x, y, z)
 */
struct vec3 {
	float x;
	float y;
	float z;
};

/* seta um valor ao vetor */
struct vec3 vset(float x, float y, float z);
/* soma dois vetores e retorna o vetor resultante */
struct vec3 vadd(struct vec3 v1, struct vec3 v2);
