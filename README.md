## PaREM

Proyecto perteneciente al curso de Programación Paralela y Distribuída. Se ha implementado la propuesta de la investigación *Parallel Regular Expression Matching* por medio de OpenMP utilizando C++.

### Integrantes

- Gabriel Spranger
- Macarena Oyague
- Alejandro Goicochea

### Implementación

En el repositorio se encuentran la implementación el algoritmo en paralelo en *PaREM.h*, en conjunto con la implementación de un Autómata Finito Determinístico en *DFA.h* debido a que será el input del algoritmo. En el constructor del DFA se debe incluir el nombre del archivo donde se deben encontrar {Q, Σ, q0, F, δ} en saltos de línea distintos y siendo δ una matriz de |Q| x |Σ|. Se adjunta además un ejemplo en *Automata.txt*.

### Documentos

Se adjunta el proyecto de investigación de referencia (*PAREM.pdf*) y un informe que tiene como finalidad tanto explicar los procedimientos del algoritmo implementado como realizar un análisis experimental respecto al speedup del algoritmo al incrementar el número del problema y el número de threads.

### Compilación

Se han incluído un archivo de inputs de diversos tamaños para correr pruebas con números de procesos distintos y un archivo *main.cpp* donde se encuentra la ejecución de las mismas. Es posible compilarlo de la manera siguiente:

MacOS: clang++ -Xpreprocessor -fopenmp -lomp  main.cpp
Linux: -fopenmp main.cpp

*Se debe tener instalado OpenMP*
