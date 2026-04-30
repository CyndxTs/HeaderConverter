# 🔧 HeaderConverter
HeaderConverter es una herramienta con interfaz gráfica (Qt) escrita en C++. Lee un archivo `.cpp`, extrae todas las declaraciones independientes y las transcribe a formato de cabecera `.h` con formato limpio, ordenado y configurable.

1. Abre el programa.
2. Pega o edita tu código fuente en el panel *Input*.
3. Configura los controladores en el panel izquierdo.
4. Pulsa *Convert* y copia el resultado desde el panel *Output*.

---

## 📖 Vocabulario

| Término | Definición |
|---|---|
| **Identificador** | Cadena de caracteres que nombra una variable, función, operando, etc. |
| **Agrupador** | Par de caracteres que encierra uno o varios elementos (`()`, `[]`, `{}`, `""`, `''`). |
| **Espaciador** | Carácter de separación en texto: espacio, tab o enter. |
| **Modificador** | Carácter que varía el tipo de dato hacia puntero o referencia de memoria (`*`, `&`). |
| **Separador** | Carácter que diferencia elementos dentro de un agrupador o declaración (`,`, `)`, `}`, `;`). |
| **Operador** | Símbolo que representa una operación entre operandos (`+`, `-`, `==`, `>=`, etc.). |
| **Palabra clave** | Palabra reservada del lenguaje usada para ubicar el inicio de una declaración (`int`, `const`, `void`, `struct`, etc.). |
| **Declaración** | Función o asignación independiente, es decir, que no esté definida dentro de otra función o clase. |
| **Asignación** | Declaración que asocia un identificador a un valor o expresión. Puede ser simple o agrupada (inicialización con `{}`). |
| **Operando** | Cada uno de los valores o expresiones que conforman el valor de una asignación. |
| **Función** | Declaración que agrupa un conjunto de parámetros bajo un identificador ejecutable. |
| **Parámetro** | Cada elemento de una función, compuesto por una palabra clave y un identificador opcional. |

---

## 🔑 Palabras clave

Las palabras clave se configuran desde el panel `Keywords` de la interfaz. El programa las usa para ubicar el inicio de una declaración, por lo que si defines un nuevo TAD con `typedef` o `struct`, debes agregar su identificador a la lista de palabras clave. Si no lo haces, el programa no puede diferenciarlo de una palabra cualquiera y puede emitir un error de partición.

---

## 🗂️ Operadores

Los operadores se configuran desde el panel `Operators` de la interfaz. Son los símbolos que el programa usa para segmentar correctamente las expresiones lógicas y aritméticas. Además, cada uno puede marcarse como *segmentador*, lo que indica que este se espaciará entre sus operandos en la salida. Los operadores que no sean segmentadores se mantendrán comprimidos.

***> Output:*** `['+'->segmentador=true]` `['/'->segmentador=false]`
```cpp
int suma = a + b;
int fraccion = a/b;
```

---

## ⚙️ Formato de procesamiento

El formato de procesamiento se configura desde el panel `Format` de la interfaz. El programa lo usa para determinar cómo leer, filtrar, ordenar e imprimir las declaraciones extraídas del archivo fuente.

#### Ajustar por margen · `bool`
Cuando está activo, las sentencias saltan a la siguiente línea al alcanzar el margen definido, alineándose con la apertura de la declaración. Si está desactivado, las sentencias siguen en la misma línea sin importar su longitud.

| Sub-opción | Tipo | Descripción |
|---|---|---|
| Límite por margen | `int` | Cantidad de caracteres hasta el margen. Valor estándar: **80**. |

***> Output:*** `[ajustarPorMargen=false]`
```cpp
int sumarEpicamente(int a,int b,int c);
```
***> Output:*** `[ajustarPorMargen=true]` `[limitePorMargen=40]`
```
int sumarEpicamente(int a,int b,int c);
```
***> Output:*** `[ajustarPorMargen=true]` `[limitePorMargen=32]`
```
int sumarEpicamente(int a,int b,
                    int c);
```

Si el margen es tan pequeño que ni el nombre de la función cabe, el ajuste se ignora completamente y la declaración se imprime sin cortes.

***> Output:*** `[ajustarPorMargen=true]` `[limitePorMargen=10]`
```
int sumarEpicamente(int a,int b,int c);
```

#### Ordenar declaraciones · `bool`
Cuando está activo, las declaraciones se imprimen ordenadas según los criterios definidos. Si está desactivado, se imprimen en el mismo orden del archivo fuente.

| Sub-opción | Tipo | Descripción |
|---|---|---|
| Criterios de ordenamiento | `char[3]` | Tres caracteres que definen el criterio de ordenamiento por cada eje. |

Los ejes se evalúan en orden: el primero es el **tipo de declaración** (asignación o función), el segundo es la **palabra clave** (`int`, `bool`, `const char`, etc.) y el tercero es el **identificador** (nombre de la variable o función). Cada eje puede tener un valor independiente:

| Valor | Significado |
|---|---|
| `A` | Ascendente |
| `D` | Descendente |
| `C` | Consecuente — ignora ese eje y pasa al siguiente |

Por ejemplo, `"ADA"` ordena ascendente por tipo, descendente por palabra clave y ascendente por identificador. Con `"ACC"` solo se ordena por tipo y los demás criterios se ignoran, manteniendo el orden relativo del archivo fuente.

***> Input:*** `[ordenarDeclaraciones=true]` `[criteriosDeOrdenamiento="ADA"]`
```cpp
int algo_1 = queso[1];
int algo_3 = queso[3];
int algo_2 = queso[2];
const char queque2 = ';';
const char queque3 = ')';
char queque1 = ']';
bool como_1 = a;
bool como_3 = c;
bool como_2 = b;
int algoInt_1(int a){ ... }
int algoInt_3(int a,int b,int c){ ... }
int algoInt_2(int a,int b){ ... }
char algoChar_3(int a,int b,int c){ ... }
char algoChar_1(int a,int b,int c){ ... }
const char algoChar_2(int a,int b,int c){ ... }
void algoVoid_1(int &a){ ... }
void algoVoid_3(int &a,int b,int c){ ... }
void algoVoid_2(int &a,int b){ ... }
```
***> Output:***
```cpp
int algo_1 = queso[1];
int algo_2 = queso[2];
int algo_3 = queso[3];

const char queque2 = ';';
const char queque3 = ')';   // "const char" > "char"

char queque1 = ']';

bool como_1 = a;
bool como_2 = b;
bool como_3 = c;

void algoVoid_1(int &a);
void algoVoid_2(int &a,int b);
void algoVoid_3(int &a,int b,int c);

int algoInt_1(int a);
int algoInt_2(int a,int b);
int algoInt_3(int a,int b,int c);

const char algoChar_2(int a,int b,int c);  // "const char" > "char"

char algoChar_1(int a,int b,int c);
char algoChar_3(int a,int b,int c);
```

#### Espaciar subelementos · `bool`
Cuando está activo, agrega un espacio tras cada separador de los subelementos de una declaración, excepto el último.

***> Output:*** `[espaciarSubelementos=false]`
```cpp
bool f(int a,char b,float c);
```
***> Output:*** `[espaciarSubelementos=true]`
```cpp
bool f(int a, char b, float c);
```

#### Procesar asignaciones · `bool`
Procesa y transcribe las asignaciones globales del archivo fuente. Si está desactivado, las asignaciones se ignoran completamente.

***> Output:*** `[procesarAsignaciones=true]`
```cpp
const int numQuesos = 10;

const char especimenID[10] = "0x983";
```

#### Procesar funciones · `bool`
Procesa y transcribe las funciones del archivo fuente. Si está desactivado, las funciones se ignoran completamente.

| Sub-opción | Tipo | Descripción |
|---|---|---|
| Suprimir variables | `bool` | Omite los identificadores de los parámetros en la salida. |
| Símbolo delimitador | `char` | Símbolo con el que termina cada función. `;` para `.h`, `{` para `.cpp`. |

***> Output:*** `[suprimirVariables=false]` `[simboloDelimitador=';']`
```cpp
bool esFemboy(int edad, char genero);

bool puedeChambear(int edad);
```
***> Output:*** `[suprimirVariables=true]` `[simboloDelimitador=';']`
```cpp
bool esFemboy(int, char);

bool puedeChambear(int);
```
***> Output:*** `[suprimirVariables=false]` `[simboloDelimitador='{']`
```cpp
bool esFemboy(int edad, char genero){

bool puedeChambear(int edad){
```

---

## 📐 Aclaraciones

- El programa es independiente de tu estilo de codificación. Por ejemplo, `const int *` puede escribirse como `const int*` o `const int          *   ` y el resultado será el mismo, siempre que el compilador lo valide.

- Las inicializaciones de arreglos con `{}` se ajustan por cada elemento de primer nivel; por ejemplo, en `'char vocales[5] = {'a','e','i','o','u'};'` los elementos de primer nivel son cada letra con sus comas. En cambio, si se tiene un arreglo de arreglos como `'int triadas[3][3] = {{1,2,3},{4,5,6},{7,8,9}};'`, el primer nivel serían las triadas completas, por lo que, en caso de ajuste de margen, lo que bajará será toda la triada como sentencia (por ejemplo, `{7,8,9}};`). Cabe aclarar que el programa no reajusta un mismo elemento más de una vez; si la sentencia se excede del margen, se mantendrá así.

- En los casos en que se asignan valores constantes como resultado de una función con operandos adicionales, el programa retoma el ajuste al primer nivel y segmenta los operandos de la asignación y los suboperandos de las funciones de primer nivel para dar un mejor estilo.

***> Input:***
```cpp
const int  * numVida[18][queso] = dameAlgoRaro('a'*12+2.5-abc,
                                    arr[12+';'
                                         ]) or 12-1;
```
***> Output:***
```cpp
const int *numVida[18][queso] = dameAlgoRaro('a'*12 + 2.5 - abc,arr[12+';']) or
                                12 - 1;
```

- Los parámetros pueden inicializarse con valores. El formato aplicado es equivalente al de los operadores en asignaciones.

***> Input:***
```cpp
void ola(const int *palabra[';']['a']=12+vida(abc[12][12]+12*15+7+1)){
    cout<<"bai bai";
}
```
***> Output:***
```cpp
void ola(const int *[';']['a'] = 12 + vida(abc[12][12] + 12*15 + 7 + 1));
```

---

## 🛡️ Sugerencias anti-errores

- Si usas `typedef` o defines una `struct` propia, **debes agregar su identificador** al arreglo de *Keywords* a través de la interfaz. El programa ubica las declaraciones buscando palabras clave como `int` o `const`; si no registras el nuevo tipo, el programa no podrá diferenciarlo de una palabra cualquiera y puede saltar un error de partición.

- Los identificadores de parámetros, palabras clave, operadores, etc. tienen tamaños distintos porque se agrupan de manera diferente. Revisa el archivo `declarations.h` para verificar las capacidades y evitar errores de desbordamiento de memoria.

- El programa valida la existencia de comentarios en las situaciones más comunes, pero siempre puede haber un caso no contemplado. Si obtienes resultados inesperados, intenta primero remover los comentarios del archivo fuente y volver a intentar.

- El programa acepta muchas formas de escribir, pero todas deben ser validadas por el compilador. No funcionará con código malformado. Algunos warnings detendrán la ejecución para reportar el error, pero puede haber casos no cubiertos.

- Para un funcionamiento óptimo, se recomienda usar espacios en lugar de tabs en los archivos fuente. Algunas validaciones aún no están 100% aseguradas con respecto al manejo de tabs.

---

## 🛠️ Recursos

- 🖥️ **CLion** `2025.2` — IDE utilizado para escribir y compilar el proyecto en C++.
- 🧩 **Qt** `6.11.0` — Framework utilizado para la realización de la interfaz gráfica.
