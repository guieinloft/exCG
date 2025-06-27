#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "vec3.h"
#include "model.h"

void model_load(struct model *model, char *path)
{
	char str_buf[60];
	int v_size, vn_size, vt_size, f_size;
	int v_cur, vn_cur, vt_cur, f_cur;
	struct vec3 *temp_v, *temp_vn, *temp_vt;
	int *temp_f, *temp_fn, *temp_ft;
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		model->size = 0;
		return;
	}

	/* lendo tamanho do modelo */

	v_size = 0;
	vn_size = 0;
	vt_size = 0;
	f_size = 0;
	while (fscanf(file, "%s", str_buf) != EOF) {
		if (strcmp(str_buf, "v") == 0)
			v_size++;
		else if (strcmp(str_buf, "vn") == 0)
			vn_size++;
		else if (strcmp(str_buf, "vt") == 0)
			vt_size++;
		else if (strcmp(str_buf, "f") == 0)
			f_size++;
		fscanf(file, "%*[^\n]");
	}

	/* lendo o modelo */
	fseek(file, 0, SEEK_SET);

	temp_v = (struct vec3 *)malloc(sizeof(struct vec3) * v_size);
	temp_vn = (struct vec3 *)malloc(sizeof(struct vec3) * vn_size);
	temp_vt = (struct vec3 *)malloc(sizeof(struct vec3) * vt_size);
	temp_f = (int *)malloc(sizeof(int) * 3 * f_size);
	temp_ft = (int *)malloc(sizeof(int) * 3 * f_size);
	temp_fn = (int *)malloc(sizeof(int) * 3 * f_size);

	v_cur = 0;
	vn_cur = 0;
	vt_cur = 0;
	f_cur = 0;
	while (fscanf(file, "%s", str_buf) != EOF) {
		if (strcmp(str_buf, "v") == 0) {
			fscanf(file, "%f %f %f",
					&temp_v[v_cur].x,
					&temp_v[v_cur].y,
					&temp_v[v_cur].z);
			v_cur++;
		} else if (strcmp(str_buf, "vn") == 0) {
			fscanf(file, "%f %f %f",
					&temp_vn[vn_cur].x,
					&temp_vn[vn_cur].y,
					&temp_vn[vn_cur].z);
			vn_cur++;
		} else if (strcmp(str_buf, "vt") == 0) {
			fscanf(file, "%f %f",
					&temp_vt[vt_cur].x,
					&temp_vt[vt_cur++].y);
			vt_cur++;
		} else if (strcmp(str_buf, "f") == 0) {
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d",
					&temp_f[f_cur * 3 + 0],
					&temp_ft[f_cur * 3 + 0],
					&temp_fn[f_cur * 3 + 0],
					&temp_f[f_cur * 3 + 1],
					&temp_ft[f_cur * 3 + 1],
					&temp_fn[f_cur * 3 + 1],
					&temp_f[f_cur * 3 + 2],
					&temp_ft[f_cur * 3 + 2],
					&temp_fn[f_cur * 3 + 2]);
			f_cur++;
		} else {
			fscanf(file, "%*[^\n]");
		}
	}
	fclose(file);

	/* indexando o modelo */

	model->v = (float *)malloc(sizeof(float) * f_size * 9);
	model->vt = (float *)malloc(sizeof(float) * f_size * 6);
	model->vn = (float *)malloc(sizeof(float) * f_size * 9);

	for (int i = 0; i < f_size; i++) {
		for (int j = 0; j < 3; j++) {
			model->v[i * 9 + j * 3 + 0] = temp_v[temp_f[i * 3 + j] - 1].x;
			model->v[i * 9 + j * 3 + 1] = temp_v[temp_f[i * 3 + j] - 1].y;
			model->v[i * 9 + j * 3 + 2] = temp_v[temp_f[i * 3 + j] - 1].z;
			model->vn[i * 9 + j * 3 + 0] = temp_vn[temp_fn[i * 3 + j] - 1].x;
			model->vn[i * 9 + j * 3 + 1] = temp_vn[temp_fn[i * 3 + j] - 1].y;
			model->vn[i * 9 + j * 3 + 2] = temp_vn[temp_fn[i * 3 + j] - 1].z;
			model->vt[i * 6 + j * 2 + 0] = temp_vt[temp_ft[i * 3 + j] - 1].x;
			model->vt[i * 6 + j * 2 + 1] = temp_vt[temp_ft[i * 3 + j] - 1].y;
		}
	}

	free(temp_v);
	free(temp_vt);
	free(temp_vn);
	free(temp_ft);
	free(temp_fn);

	model->size = f_size;

	/* criando buffers */
	/*
	glGenBuffers(1, &model->vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, model->vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->v), model->v,
			GL_STATIC_DRAW);
	
	glGenBuffers(1, &model->vtbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, model->vtbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->vt), model->vt,
			GL_STATIC_DRAW);
	
	glGenBuffers(1, &model->vnbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, model->vnbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->vn), model->vn,
			GL_STATIC_DRAW);
	*/
}

void model_render(struct model *model)
{
	/*
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, model->vbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, model->size * 3);
	glDisableVertexAttribArray(0);
	*/
	for (int i = 0; i < model->size; i++) {
		glBegin(GL_TRIANGLES);
		glVertex3f(model->v[i * 9], model->v[i * 9 + 1],
				model->v[i * 9 + 2]);
		glVertex3f(model->v[i * 9 + 3], model->v[i * 9 + 4],
				model->v[i * 9 + 5]);
		glVertex3f(model->v[i * 9 + 6], model->v[i * 9 + 7],
				model->v[i * 9 + 8]);
		glEnd();
	}
}
