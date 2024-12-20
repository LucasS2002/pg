# Lista 1

## 1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?

GLSL (*OpenGL Shading Language*) é a linguagem de programação de shaders da OpenGL.

Os shaders obrigatórios são do tipo vertex e fragment, responsáveis, respectivamente, por processar os vértices do que se irá desenhar e determinar a cor de cada pixel.

---

## 2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?

As primitivas gráficas são o que a OpenGL usa para determinar como desenhar uma série de vértices.

Os vértices são armazenados em objetos chamados *Vertex Buffer Objects (VBOs)* na memória da GPU.

---

## 3. Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).

VBO (Vertex Buffer Object) é onde são armazenadas informações como as coordenadas de vértices e suas cores.

EBO (Element Buffer Object) é onde são armazenados índices do VBO a serem utilizados ao desenhar.

VAO (Vertex Array Object) guarda as informações de como serão utilizadas as informações de VBOs no shader e pode também guardar um EBO.

---

## 5. Faça o desenho de 2 triângulos na tela. Desenhe eles:
- ### Apenas com o polígono preenchido
- ### Apenas com contorno
- ### Apenas como pontos
- ### Com as 3 formas de desenho juntas

[Fonte](src/cinco.cpp)

![cinco](./img/cinco.png "cinco")

---

## 6. Faça o desenho de um círculo na tela, utilizando a equação paramétrica do círculo para gerar os vértices. Depois disso:
- ### Desenhe um octágono
- ### Desenhe um pentágono
- ### Desenhe um pac-man!
- ### Desenhe uma fatia de pizza
- ### DESAFIO: desenhe uma “estrela”

[Fonte](src/seis.cpp) (Utilize as setas para ciclar entra as formas)

![seis](./img/seis.gif "seis")

---

## 7. Desenhe uma espiral

[Fonte](src/sete.cpp)

![sete](./img/sete.png "sete")

---

## 8. Considerando o seguinte triângulo abaixo, formado pelos vértices P1, P2 e P3, respectivamente com as cores vermelho, verde e azul.
- ### Descreva uma possível configuração dos buffers (VBO, VAO e EBO) para representá-lo.
- ### Como estes atributos seriam identificados no vertex shader?
- ### Agora implemente!

[Fonte](src/oito.cpp)

![oito](./img/oito.png "oito")

---

## 9. Faça um desenho em um papel quadriculado (pode ser no computador mesmo) e reproduza-o utilizando primitivas em OpenGL. Neste exercício você poderá criar mais de um VAO e fazer mais de uma chamada de desenho para poder utilizar primitivas diferentes, se necessário.

[Fonte](src/nove.cpp)

![nove](./img/nove.png "nova")

---