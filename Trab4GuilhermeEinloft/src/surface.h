Vector3 rotate(Vector3 p, Vector3 a);
Vector3 translate(Vector3 p, Vector3 d);
Vector2 proj_ortho(Vector3 p, float d);
Vector2 proj_perp(Vector3 p, float d);
void generate_surface(Vector3 out[MAX_RES][MAX_RES],
		Vector2 in[MAX_RES], int m, int n, float d);
void transform_surface(Vector3 in[MAX_RES][MAX_RES],
		Vector3 out[MAX_RES][MAX_RES],
		int m, int n, Vector3 t, Vector3 ang);
void project_surface(Vector3 in[MAX_RES][MAX_RES],
		Vector2 out[MAX_RES][MAX_RES],
		int m, int n, int d, bool perp);
void draw_surface(Vector2 out[MAX_RES][MAX_RES], int m, int n);
void draw_surface_culled(Vector2 out[MAX_RES][MAX_RES], int m, int n);
void generate_normals(Vector3 in[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		int m, int n);
void project_normals(Vector3 out[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		Vector2 normals_proj[MAX_RES][MAX_RES][2],
		int m, int n, int d, bool perp);
void draw_normals(Vector2 out_proj[MAX_RES][MAX_RES],
		Vector2 normals_proj[MAX_RES][MAX_RES][2], int m, int n);
