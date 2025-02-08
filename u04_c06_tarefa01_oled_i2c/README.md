# oled_i2c - Tarefa 1 (unidade 4, capítulo 6)

**[VÍDEO DA TAREFA](https://www.youtube.com/watch?v=lCFql4CgwLI)**.

Projeto para a placa [BitDogLab](https://github.com/BitDogLab/BitDogLab)
(contendo Pico W).

| controles (serial)         | função      |
|----------------------------|-------------|
| <kbd>a</kbd>-<kbd>z</kbd>,<kbd>A</kbd>-<kbd>Z</kbd>,<kbd>0</kbd>-<kbd>9</kbd>,<kbd>Space</kbd> | Inserir texto no display SSD1306 |
| <kbd>0</kbd>-<kbd>9</kbd>  | Exibir dígito na matriz WS2812              |
| <kbd>⌫ Backspace</kbd>     | Deletar último caractere no display SSD1306 |
| <kbd>⏎ Enter</kbd>         | Deletar todos caracteres no display SSD1306 |

| controles (BitDogLab)      | função                   |
|----------------------------|--------------------------|
| Botão A                    | Ligar/desligar LED verde |
| Botão B                    | Ligar/desligar LED azul  |

## Exemplo de utilização no Linux

```bash
# compilação
cmake -B build  # só precisa chamar cmake uma vez...
make -C build

# carregar programa no Pico W
picotool load -f build/oled_i2c.uf2

# acesso serial com Minicom
minicom -D /dev/ttyACM0 -b 115200
```

<!-- vim: set spelllang=pt_br: -->
