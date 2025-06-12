#define MAX_RES 128

void plot_curve(Vector2 points[MAX_RES], int size);
void plot_points(Vector2 points[MAX_RES], int size);
void change_point(Vector2 *point, float x, float y);
void evaluate_curve(Vector2 points[MAX_RES], int pnum,
		Vector2 bezier[MAX_RES], int bnum);
void save_curve(Vector2 points[MAX_RES], int pnum);
void load_curve(Vector2 points[MAX_RES], int *pnum);
