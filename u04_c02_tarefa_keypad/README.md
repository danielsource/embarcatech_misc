# keypad - Tarefa 1 (unidade 4, capítulo 2)

**[VÍDEO DA TAREFA](https://www.youtube.com/watch?v=t0fjIUusSis)**.

Projeto para Raspberry Pi Pico com teclado matricial 4x4, 3 LEDs e 1
buzzer. O teclado suporta um pressionamento de tecla por vez, sendo
impresso na saída serial o valor da tecla a cada aperto registrado.
As teclas `A`, `B`, `C`, `D` ligam e desligam o LED vermelho, LED
verde, LED azul e buzzer, respectivamente.

A tarefa em si foi feita de forma totalmente virtual graças ao [simulador Wokwi](https://wokwi.com/),
que além de fornecer um web site para simular diversos microcontroladores,
também pode ser [integrado no VS Code](https://docs.wokwi.com/vscode/getting-started)... 🤭

## Exemplo de compilação em GNU/Linux

```
cmake -B build
make -C build
```

## Simulação

Para simular esse projeto, instale ["Wokwi for VS Code"](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode).
Abra o diretório do projeto no Visual Studio Code, presione **F1** e
selecione "Wokwi: Start Simulator".

<!-- vim: set spelllang=pt_br: -->
