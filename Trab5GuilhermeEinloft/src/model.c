#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "image.h"
#include "vec2.h"
#include "vec3.h"
#include "model.h"

void model_load_obj(struct model *model, char *path)
{
	char str_buf[60];
	int v_size, vn_size, vt_size, f_size;
	int v_cur, vn_cur, vt_cur, f_cur;
	struct vec3 *temp_v, *temp_vn;
	struct vec2 *temp_vt;
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
	temp_vt = (struct vec2 *)malloc(sizeof(struct vec2) * vt_size);
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
					&temp_vt[vt_cur].y);
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
	model->v = (struct vec3 *)malloc(sizeof(struct vec3) * f_size * 3);
	model->vt = (struct vec2 *)malloc(sizeof(struct vec2) * f_size * 3);
	model->vn = (struct vec3 *)malloc(sizeof(struct vec3) * f_size * 3);

	for (int i = 0; i < f_size; i++) {
		for (int j = 0; j < 3; j++) {
			model->v[i * 3 + j] = temp_v[temp_f[i * 3 + j] - 1];
			model->vn[i * 3 + j] = temp_vn[temp_fn[i * 3 + j] - 1];
			model->vt[i * 3 + j] = temp_vt[temp_ft[i * 3 + j] - 1];
		}
	}

	free(temp_v);
	free(temp_vt);
	free(temp_vn);
	free(temp_f);
	free(temp_ft);
	free(temp_fn);

	model->size = f_size;
}

void model_load_texture(struct model *model, char *path)
{
	struct image img;
	image_load(&img, path);
	glGenTextures(1, (GLuint*)&model->texture);
	glBindTexture(GL_TEXTURE_2D, model->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.w, img.h, 0, GL_RGB,
			GL_UNSIGNED_BYTE, img.img);
	image_close(&img);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void model_render(struct model *model)
{
	glBindTexture(GL_TEXTURE_2D, model->texture);
	for (int i = 0; i < model->size; i++) {
		glBegin(GL_TRIANGLES);
		glTexCoord2f(model->vt[3 * i].x, model->vt[3 * i].y);
		glNormal3f(model->vn[3 * i].x, model->vn[3 * i].y,
				model->vn[3 * i].z);
		glVertex3f(model->v[3 * i].x, model->v[3 * i].y,
				model->v[3 * i].z);
		glTexCoord2f(model->vt[3 * i + 1].x, model->vt[3 * i + 1].y);
		glNormal3f(model->vn[3 * i + 1].x, model->vn[3 * i + 1].y,
				model->vn[3 * i + 1].z);
		glVertex3f(model->v[3 * i + 1].x, model->v[3 * i + 1].y,
				model->v[3 * i + 1].z);
		glTexCoord2f(model->vt[3 * i + 2].x, model->vt[3 * i + 2].y);
		glNormal3f(model->vn[3 * i + 2].x, model->vn[3 * i + 2].y,
				model->vn[3 * i + 2].z);
		glVertex3f(model->v[3 * i + 2].x, model->v[3 * i + 2].y,
				model->v[3 * i + 2].z);
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void model_close(struct model *model)
{
    free(model->v);
    free(model->vn);
    free(model->vt);
}
