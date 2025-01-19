# bitdoglab_serial - Tarefa 2 (unidade 4, capítulo 2)

**[VÍDEO DA TAREFA](https://www.youtube.com/watch?v=82D6gPb8KjQ)**.

Projeto para o [BitDogLab](https://github.com/BitDogLab/BitDogLab) 6.3
(baseado no Pico W). Utilizando a comunicação serial, é feito o acionamento
de periféricos embutidos no BitDogLab: LED RGB (GPIO 11, 12, 13) e buzzer
(GPIO 21).

Controles no terminal serial:
  * 1 &rarr; LED vermelho e desligar demais GPIOs
  * 2 &rarr; LED verde e desligar demais GPIOs
  * 3 &rarr; LED azul e desligar demais GPIOs
  * 4 &rarr; LED branco (todos acesos)
  * 5 &rarr; Desligar todos os LEDs
  * 6 &rarr; Buzzer por 2 segundos

Algum terminal serial como PuTTY, minicom ou screen pode ser usado.

## Exemplo de compilação em GNU/Linux

```
cmake -B build
make -C build
```

## Exemplo de acesso serial via GNU Screen

```
screen /dev/ttyACM0 115200
```

<!-- vim: set spelllang=pt_br: -->
