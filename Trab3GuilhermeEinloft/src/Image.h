#ifndef IMAGE__H
#define IMAGE__H

#include <stdint.h>

class Image {
    public:
    //construtor
    Image();
    //fecha imagem
    void close_image();
    //carrega bmp
    void bmp_load(const char *path);
    //renderiza imagem
    void render(float x, float y, int o);
    //pega altura e largura
    int get_w();
    int get_h();
    //copia imagem src para a imagem atual
    void copy(Image *src);
    //comprime a imagem
    // - converte color space
    // - aplica a DCT
    // - quantiza cada bloco
    // - faz as operações inversas
    void compress(uint8_t quality[8]);

    private:
    uint32_t w;
    uint32_t h;
    uint8_t *img;
};

#endif
