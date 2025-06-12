/*
 * surface.h
 * possui funções relativas a superfícies e normais
 */

#define MAX_SW 1920
#define MAX_SH 1080

/* rotaciona ponto nos eixos x, y e z */
Vector3 rotate(Vector3 p, Vector3 a);
/* translada ponto nos eixos x, y e z */
Vector3 translate(Vector3 p, Vector3 d);
/* faz projeção ortográfica do ponto */
Vector3 proj_ortho(Vector3 p, float d);
/* faz projeção em perspectiva do ponto */
Vector3 proj_perp(Vector3 p, float d);
/* gera uma superfície baseada em uma curva de bezier */
void generate_surface(Vector3 out[MAX_RES][MAX_RES],
		Vector2 in[MAX_RES], int m, int n, float d, float offset);
/* transforma (rotaciona e translada) uma superfície */
void transform_surface(Vector3 in[MAX_RES][MAX_RES],
		Vector3 out[MAX_RES][MAX_RES],
		int m, int n, Vector3 t, Vector3 ang);
/* projeta uma superfície */
void project_surface(Vector3 in[MAX_RES][MAX_RES],
		Vector3 out[MAX_RES][MAX_RES],
		int m, int n, int d, bool perp);
/* desenha a superfície na tela */
void draw_surface(Vector3 out[MAX_RES][MAX_RES], int m, int n);
/* desenha a superfície na tela com back-face culling */
void draw_surface_culled(Vector3 out[MAX_RES][MAX_RES], int m, int n);
/* gera as normais de cada face */
void generate_face_normals(Vector3 in[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		int m, int n);
/* projeta as normais sobre os pontos centrais de cada face */
void project_face_normals(Vector3 out[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		Vector3 normals_proj[MAX_RES][MAX_RES][2][2],
		int m, int n, int d, bool perp);
/* desenha as normais de cada face */
void draw_face_normals(Vector3 out_proj[MAX_RES][MAX_RES],
		Vector3 normals_proj[MAX_RES][MAX_RES][2][2], int m, int n);
/* desenha as normais de cada face com back-face culling */
void draw_face_normals_culled(Vector3 out_proj[MAX_RES][MAX_RES],
		Vector3 normals_proj[MAX_RES][MAX_RES][2][2], int m, int n);
/* gera as normais de cada vértice */
void generate_vertex_normals(Vector3 normals[MAX_RES][MAX_RES][2],
		Vector3 v_normals[MAX_RES][MAX_RES], int m, int n);
/* calcula o z-buffer da superfície e aplica a iluminação */
void update_z_buffer(Vector3 v[MAX_RES][MAX_RES], Vector3 vn[MAX_RES][MAX_RES],
		int m, int n, float z_depth[MAX_SW][MAX_SH],
		float colors[MAX_SW][MAX_SH], Vector3 lightdir);
/* desenha o objeto com iluminação aplicada */
void draw_z_buffer(float colors[MAX_SW][MAX_SH], int screenW, int screenH);
