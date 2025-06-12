Trabalho 3 de Mathias Recktenvald

2025/1 - Implementação de um jogo de tanque utilizando a biblioteca canvas 2D

Requisitos obrigatórios:
    - A curva é desenhada utilizando uma B-Spline;
    - No início do jogo o jogador define as pistas, cada pista (interna e externa) deve ter pelo menos 4 pontos para fechar a curva.
    Botão esquerdo do mouse para colocar um ponto, e direito para remover o ultimo ponto colocado;
    - A movimentação do tanque ocorre conforme descrito no trabalho;
    - O tanque dispara e os alvos são destruídos;
    - Tem um placar de pontuação. Exibido quando o jogo termina. Apenas pede para colocar o nome caso tenha pontuação necessária para o placar;
    - Projétil colide com a pista e os alvos;
    - Tanque tem geometria simples, é um retangulo;
    - Tem controle de FPS, limitado a 60 no arquivo fps.h;
    - Roda em windows;
    - O tanque recebe dano com colisões/balas e possui uma barra de vida;
    - Jogo fluído.

Extras:
    - Alvos com lógicas diferentes: dois inimigos. Os dois andam até o jogador, um atira de volta com um delay entre os tiros e o outro da dashes em direção ao jogador com um delay;
    - Alvos com diferentes visuais:
        inimigos são triângulos e pentágonos, o barril é um círculo.
    - Power ups: foram implemtados quatro tipos diferentes de power ups:
        shield: diminui danos de colisão e protege de tiros por 8 segundos - powerup azul;
        speed: dobra a velocidade do tanque por 8 segundos - powerup verde;
        shootAmp: diminui o delay de tiros do tanque para 0,1s por 4s - powerup laranja;
        health: regenera 30 de vida - powerup vermelho.
    - Efeitos de explosão para o barril.