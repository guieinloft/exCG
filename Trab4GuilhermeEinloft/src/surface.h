Vector3 rotate(Vector3 p, Vector3 a);
Vector3 translate(Vector3 p, Vector3 d);
Vector2 proj_ortho(Vector3 p, float d);
Vector2 proj_perp(Vector3 p, float d);
void draw_surface(Vector2[MAX_RES][MAX_RES], int m, int n);
void generate_surface(Vector3 out[MAX_RES][MAX_RES],
		Vector2 in[MAX_RES], int m, int n, float d);
