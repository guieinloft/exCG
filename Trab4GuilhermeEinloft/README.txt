# Trabalho 4 - Guilherme Meneghetti Einloft
## Requisitos:
- Edição da curva Bezier em tempo real
- Atualização instantânea do modelo
- Parametrização do número de vértices do objeto
- Translação no eixo Y para fazer molas
- Visualização do objeto em diferentes posições/ângulos
- Projeções ortográfica e em perspectiva

## Extras:
- Adição e remoção de pontos na curva
- Geração e exibição de normais de cada face
- Back-face culling (remoção de faces ocultas)
- Z-buffer com iluminação por pixel (Phong)
- Salvamento e carregamento de curvas

## OBS: insira os pontos da curva preferencialmente de cima para baixo

# Manual
## Modo de visualização
### Translação:
    -y
     ^  PgUp: -z
     |
-x <- -> +x 
     |
     v  PgDn: +z
    +y

### Rotação (Shift) / Direção da Luz (Ctrl):
    -pitch
       ^   PgUp: -roll
       |
+yaw <- -> -yaw
       |
       v   PgDn: +roll
    +pitch

## Modo de edição
- Ctrl: encaixa pontos no grid
- Shift: adiciona ponto no início da curva ao invés do final
