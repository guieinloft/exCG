#ifndef IMAGE__H
#define IMAGE__H

#include <stdio.h>
#include <stdint.h>

class Image {
    public:
    //construtor e destrutor
    Image();
    ~Image();
    //libera array da imagem
    void close_image();
    //carrega a imagem definida em path
    bool bmp_load(const char *path);
    //salva a imagem em path
    void bmp_save(const char *path);
    //salva a imagem no projeto file
    void file_save(FILE *file);
    //carrega a imagem do projeto file
    void file_load(FILE *file);
    //mostra a imagem na tela
    void render(float x, float y, int o);
    //inverte a imagem horizontalmente ou verticalmente
    void flip_h();
    void flip_v();
    //redimensiona a imagem
    void resize(int new_w, int new_h);
    //retornam as dimensoes
    int get_w();
    int get_h();
    //define o pixel (x,y) com a cor {r,g,b,a}
    //ou mescla o pixel original da imagem com a cor {r,g,b,a}
    //caso o pixel (x,y) seja um pixel válido da imagem
    void put_pixel(int x, int y, int r, int g, int b, int a, bool blend);
    //pega a cor do pixel (x,y) caso seja um pixel válido da imagem
    //c.c. pega a cor {0, 0, 0, 0}
    void get_pixel(int x, int y, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);
    //cria imagem em branco
    void clear_image(int new_w, int new_h);
    //mescla a imagem src com a imagem atual
    void blend(Image *src, int x, int y, int sx, int sy, int o);
    //copia a imagem src para a imagem atual
    void copy(Image *src);
    //rotaciona a imagem no angulo rad (em radianos)
    void rotate(float rad, int *offx, int *offy);
    //aplica desfoque gaussiano na imagem
    //pode aplicar somente horizontalmente ou verticalmente
    void blur(int radius, bool horizontal, bool vertical);

    private:
    //dimensões
    uint32_t w;
    uint32_t h;
    //a imagem propriamente dita
    uint8_t *img;
};

#endif
