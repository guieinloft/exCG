/* 
 * bezier.h
 * possui todas as funções específicas da curva de bezier
 * como mudança de ponto, avaliação da curva e plot da curva
 */


#define MAX_RES 128

/* desenha a curva na tela */
void plot_curve(Vector2 points[MAX_RES], int size);
/* desenha os pontos de controle na tela */
void plot_points(Vector2 points[MAX_RES], int size);
/* muda a posição de um ponto de controle */
void change_point(Vector2 *point, float x, float y);
/* avalia a curva e salva os pontos gerados em um vetor */
void evaluate_curve(Vector2 points[MAX_RES], int pnum,
		Vector2 bezier[MAX_RES], int bnum);
/* salva a curva no arquivo "curva.txt" */
void save_curve(Vector2 points[MAX_RES], int pnum);
/* carrega a curva do arquivo "curva.txt" */
void load_curve(Vector2 points[MAX_RES], int *pnum);
