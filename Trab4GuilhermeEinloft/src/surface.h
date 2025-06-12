#define MAX_SW 1920
#define MAX_SH 1080

Vector3 rotate(Vector3 p, Vector3 a);
Vector3 translate(Vector3 p, Vector3 d);
Vector3 proj_ortho(Vector3 p, float d);
Vector3 proj_perp(Vector3 p, float d);
void generate_surface(Vector3 out[MAX_RES][MAX_RES],
		Vector2 in[MAX_RES], int m, int n, float d, float offset);
void transform_surface(Vector3 in[MAX_RES][MAX_RES],
		Vector3 out[MAX_RES][MAX_RES],
		int m, int n, Vector3 t, Vector3 ang);
void project_surface(Vector3 in[MAX_RES][MAX_RES],
		Vector3 out[MAX_RES][MAX_RES],
		int m, int n, int d, bool perp);
void draw_surface(Vector3 out[MAX_RES][MAX_RES], int m, int n);
void draw_surface_culled(Vector3 out[MAX_RES][MAX_RES], int m, int n);
void generate_face_normals(Vector3 in[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		int m, int n);
void project_face_normals(Vector3 out[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		Vector3 normals_proj[MAX_RES][MAX_RES][2][2],
		int m, int n, int d, bool perp);
void draw_face_normals(Vector3 out_proj[MAX_RES][MAX_RES],
		Vector3 normals_proj[MAX_RES][MAX_RES][2][2], int m, int n);
void draw_face_normals_culled(Vector3 out_proj[MAX_RES][MAX_RES],
		Vector3 normals_proj[MAX_RES][MAX_RES][2][2], int m, int n);
void generate_vertex_normals(Vector3 normals[MAX_RES][MAX_RES][2],
		Vector3 v_normals[MAX_RES][MAX_RES], int m, int n);
void update_z_buffer(Vector3 v[MAX_RES][MAX_RES], Vector3 vn[MAX_RES][MAX_RES],
		int m, int n, float z_depth[MAX_SW][MAX_SH],
		float colors[MAX_SW][MAX_SH], Vector3 light_source);
void draw_z_buffer(float z_depth[MAX_SW][MAX_SH], int screenW, int screenH);
void draw_light(float colors[MAX_SW][MAX_SH], int screenW, int screenH);
