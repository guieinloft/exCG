# Trabalho 3 - Guilherme Meneghetti Einloft
## Requisitos:
- Curvas B-spline
- Edição de pistas
- Movimentação do tanque
- Mísseis do tanque
- Pontuação
- Projéteis com colisões
- Tanque e alvos simples
- Controle de FPS (DeltaTime)
- Barra de vida e danos na colisão
- Geração aleatória de alvos

## Extras:
- Alvos com movimentos
- Alvos com formas compostas
- Power-ups de disparo, vida extra e escudo
- Explosões
- Preenchimento da pista
- Dois níveis de dificuldade

# Manual - Tank Game:
## Tela de título
Local onde você pode escolher se vai jogar ou editar uma fase.
- Para jogar:
	- Clique em jogar;
	- Escolha uma das 3 fases predefinidas ou digite o caminho de uma fase (precisa estar na pasta levels)
	- Escolha a dificuldade e jogue!
- Para editar:
	- Clique em editar;
	- Digite o caminho de uma fase (extensão: .trk)
	- OBS: caso a fase não exista ou não esteja na pasta levels, uma pista nova será criada.
	- Escolha a dificuldade e jogue!

## Jogo
Destrua todos os alvos sem morrer!

### Controles:
- A e seta para esquerda: viram o tanque para a esquerda
- S e seta para direita: viram o tanque para a direita
- Mouse: mira com movimento, disparos com botão esquerdo
- Enter: pausa/despausa o jogo

### Inimigos:
- Barril: Inimigo mais simples, pode ser gerado aleatoriamente ou posicionado pelo usuário no editor
	- Comportamento: fica parado
	- Vida: 1 pt.

- Chaser (perseguidor): Inimigo que persegue o jogador, só pode ser posicionado no editor
	- Comportamento: fica parado até receber dano, após isso persegue o jogador
	- Vida: 4 (6) pts.

- Torre: Inimigo que atira no jogador, só pode ser posicionado no editor
	- Comportamento: fica parado e atira em direção ao jogador
	- Vida: 8 (12) pts.

### Power-ups:
- Escudo (azul): adiciona um escudo ao jogador com 4 pts. de vida
- Vida (verde): recupera 3 pts. de vida do jogador
- Super (amarelo): diminui o tempo entre disparos do jogador

## Editor de pistas
### Menu principal:
Possui 5 opções:
- Edição
- Adição
- Remoção
- Salvamento
- Saída

### Edição:
Edita a posição das entidades ou dos pontos de controle

### Adição:
Adicionar novas entidades à pista
Entidades adicionáveis:
- Barril
- Chaser
- Torre
- Power-ups (escudo, vida e super)

### Remoção:
Remover entidades removíveis (todas menos o tanque)

### Salvamento:
Salvar as alterações feitas à pista
OBS: só pode salvar caso nenhuma das entidades esteja colidindo com outra entidade ou com as bordas
