# Header Converter
**Autor:** CyndxTs o.0?!  
**Versión:** 2.0

Este programa está principalmente ideado para ser un parser de archivos `.cpp` hacia archivos `.h`.. aunque claro, no de una manera tan profesional, pero ahí va mi intento 100% hardcodeado (bromita). Además, como tal también funciona para ordenar un mismo `.h`, nada te impide probar.. Para resumir la utilidad del programa, ya no tendrás que estar copiando función a función cada cabezera para hacer tu header, sino hacer la del ave maría directamente al final del lab! Además, por si no fuera poco, este programa añade varios controladores para realizar diferentes ajustes adicionales! Por ejemplo, la supresión de variables de los parámetros de las funciones en definición, el ajuste de cabezeras al margen de página, y muchas más!

---

## Índice

| Sección | Fila |
|---|---|
| Título & Introducción & Índice | 002 |
| Resumen de Novedades | 027 |
| Vocabulario | 069 |
| Controladores Generales | 103 |
| Controladores de Asignaciones | 269 |
| Controladores de Funciones | 307 |
| Aclaraciones de Formato | 472 |
| Sugerencias Anti-Errores | 537 |
| FAQ & Cositas | 588 |

---

## Resumen de Novedades

### Versión 2.0

**[+]** Ahora el programa puede validar por su cuenta la posibilidad de palabras claves compuestas, asignaciones predefinidas en parámetros de funciones, y sobrecargas de operadores.

**[+]** Ahora el usuario puede elegir que operadores segmentan a los de operandos de `Asignacion` y subOperandos de `Funcion`. Es decir, ahora podrás elegir que operadores se espacian entre los operandos.

> **Ejemplo:**
> ```
> '+' => {Segmentacion = true}
>     INPUT:  int suma=a+b;
>     OUTPUT: int suma = a + b;
>
> '/' => {Segmentacion = false}
>     INPUT:  int fraccion=a / b;
>     OUTPUT: int fraccion = a/b;
> ```
> **Disclaimer:** Los subOperandos dentro de otros agrupadores simplemente se ajustaran/comprimiran dentro de estos.

**[+]** Ahora las declaraciones se podrán ordenar independiente o simultaneamente por cada Tipo de Ordenamiento.
```
{Tipo de Declaracion} || {KeyWord} || {Identificador}
```

**[+]** Se agregaron nuevos warnings, y se modifico la descripción de las acciones recomendadas de estos en la consola. Además, ahora solo algunos cancelaran la ejecución del programa.

**[+]** Se agregaron comentarios precisos a los archivos `.h` con el fin de poder agilizar el entendimiento de los controladores del programa.

**[+]** Se modificó la declaración de `LSE` con el fin de tener una mayor eficiencia en casos en donde no se precise ordenar las declaraciones.
- Cambio de Identificador: `LSE` → `LDX`
- Nuevo atributo: `final` [Tipo: `Nodo *`]

**[-]** Por el momento, se decidió descartar la función de ajuste hacia margen en `subElementos`. Es decir, ya no se maneja una `subApertura` en las declaraciones.

---

## Vocabulario

**"Declaracion"**  
Función o Asignación independiente. Es decir, que no sea definida dentro de otra función o clase.

**"KeyWord"**  
También conocidas como 'Palabra clave' o 'Palabra Reservada', son palabras con funcionalidades especiales (Como idenfiticadores de tipo de dato o inicializadores de estructuras de control). Aunque, almenos para este programa se las utiliza para ubicar las declaraciones.

**"Identificador"**  
Cadena de caracteres que simboliza el nombre de alguna variable, `KeyWord`, `Operando`, etc.

**"Modificador"**  
Caracter que varia el tipo de dato hacia puntero o dirección de memoria del tipo descrito.

**"Agrupador"**  
Caracter que encierra uno o varios elementos.

**"Aperturador" & "Delimitador"**  
Agrupadores de 'Apertura' y 'Cierre', respectivamente.

**"Espaciador"**  
Caracter que se usa para separar caracteres en archivos de texto.

**"Separador"**  
Caracter que se usa para diferenciar a un elemento de un conjunto de un agrupador o declaracion.

**"Sentencia"**  
Grupo de elementos que conformen un tipo de dato predefinido. Por ejemplo, todos los elementos de un parámetro en conjunto. `[KeyWord + Identificador]`

**"Nivel" & "SubNivel"**  
Cantidad de Aperturadores previos a la posición en la que se encuentre el elemento. Por ejemplo, en `char what = cad[arr['#'][2]];`, el `'#'` esta en el tercer `subNivel` de aperturas, y en el cuarto `nivel` de aperturas.

---

## Controladores Generales

### `ajustarPorMargen` [Tipo: `const bool`]
Cuando está habilitado, las sentencias pasan automáticamente a la siguiente linea en su posicion de apertura respectiva cuando se alcanza el margen de página preestablecido. Cuando está deshabilitado (o si el ajuste es en el primer caracter despues de la apertura de la declaracion), simplemente las sentencias seguiran su curso por la misma linea independientemente de como sea en el archivo fuente.

> #### `limitePorMargen` [Tipo: `const int`]
> Indica la cantidad de caracteres hasta el margen de Pagina deseado. Para el caso estándar, este margen aparece después de 80 caracteres (NO ESPECIALES).

### `ordenarDeclaraciones` [Tipo: `const bool`]
Cuando está habilitado, las declaraciones se imprimiran ordenadas conforme al valor del controlador de los criterios de ordenamiento. Cuando está deshabilitado, simplemente se imprimirán conforme se lean del archivo fuente.

> #### `criteriosDeOrdenamiento` [Tipo: `const char[4]`]
> Indica el tipo de Ordenamiento con el que se imprimiran las declaraciones obtenidas del archivo fuente. Los valores para estos son conforme a las letras: `['A'scendente | 'D'escendente | 'C'onsecuente]`
> Estos se deben colocar segun la posición de los criterios a evaluar:
> ```
> {Tipo de Declaracion} {KeyWord} {Identificador}
> ```

---

### Casos de Uso

#### Caso A — `ajustarPorMargen = true`, `limitePorMargen = 10`
> *Se indica la posicion del margen de pagina con `-|`*

**INPUT:**
```cpp
         -|
int sumarEpicamente(int a,int b,int c){
    return a+b+(int)'e'+(int)'p'+(int)'i'+(int)'c';
}
```

**OUTPUT:**
```
[#] >> WARNING POR IGNORAMIENTO DE LIMITE

         -|
int sumarEpicamente(int a,int b,int c);
```

---

#### Caso B — `ajustarPorMargen = false`, `limitePorMargen = 10`

**INPUT:**
```cpp
         -|
int sumarEpicamente(int a,int b,int c){
    return a+b+(int)'e'+(int)'p'+(int)'i'+(int)'c';
}
```

**OUTPUT:**
```
         -|
int sumarEpicamente(int a,int b,int c);
```

---

#### Caso C — `ajustarPorMargen = true`, `limitePorMargen = 32`

**INPUT:**
```cpp
                               -|
int sumarEpicamente(int a,int b,int c){
    return a+b+(int)'e'+(int)'p'+(int)'i'+(int)'c';
}
```

**OUTPUT:**
```
                               -|
int sumarEpicamente(int a,int b,
                    int c);
```

---

#### Caso D — `ordenarDeclaraciones = true`, `criteriosDeOrdenamiento = "ADA"`
> `{Ascendente por Tipo de Declaracion}` `{Descendente por KeyWord}` `{Ascendente por Identificador}`

**INPUT:**
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
int algoInt_1(int a){
    return a+(int)'a'-(int)'l'*(int)'g'/(int)'o';
}
int algoInt_3(int a,int b,int c){
    return a-b*c/(int)'a'+(int)'l'-(int)'g'*(int)'o';
}
int algoInt_2(int a,int b){
    return a*b/(int)'a'+(int)'l'-(int)'g'*(int)'o';
}
char algoChar_3(int a,int b,int c){
    return a-b*c/(int)'a'+(int)'l'-(int)'g'*(int)'o';
}
char algoChar_1(int a,int b,int c){
    return a+b+(int)'e'+(int)'p'+(int)'i'+(int)'c';
}
const char algoChar_2(int a,int b,int c){
    return a*b/(int)'a'+(int)'l'-(int)'g'*(int)'o';
}
void algoVoid_1(int &a){
    a = (int)'e'+(int)'p'+(int)'i'+(int)'c';
}
void algoVoid_3(int &a,int b,int c){
    a = b*c/(int)'a'+(int)'l'-(int)'g'*(int)'o';
}
void algoVoid_2(int &a,int b){
    a = b/(int)'a'+(int)'l'-(int)'g'*(int)'o';
}
```

**OUTPUT:**
```cpp
int algo_1 = queso[1];

int algo_2 = queso[2];

int algo_3 = queso[3];

const char queque2 = ';';

const char queque3 = ')';   // Notar que "const char" > "char"

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

const char algoChar_2(int a,int b,int c);   // Notar que "const char" > "char"

char algoChar_1(int a,int b,int c);

char algoChar_3(int a,int b,int c);
```

---

## Controladores de Asignaciones

### `procesarAsignaciones` [Tipo: `const bool`]
Cuando está habilitado, se procesarán las asignaciones declaradas en el archivo fuente. Si está deshabilitado, las asignaciones no se procesarán ni transcribirán.

---

#### Caso A — `procesarAsignaciones = false`

**INPUT:**
```cpp
const int numQuesos =   10  ;

const char especimenID[ 10 ] =  "0x983" ;
```

**OUTPUT:**
```
[#] >> WARNING POR ARCHIVO VACIO
```

---

#### Caso B — `procesarAsignaciones = true`

**INPUT:**
```cpp
const int numQuesos =   10  ;

const char especimenID[ 10 ] =  "0x983" ;
```

**OUTPUT:**
```cpp
const int numQuesos = 10;

const char especimenID[10] = "0x983";
```

---

## Controladores de Funciones

### `procesarFunciones` [Tipo: `const bool`]
Cuando está habilitado, se procesarán las funciones declaradas en el archivo fuente. Si está deshabilitado, las funciones no se procesarán ni se transcribirán.

> #### `suprimirVariables` [Tipo: `const bool`]
> Cuando está habilitado, los identificadores de las variables de los parámetros de la función no serán procesados en la conversión. Si está deshabilitado, se considerarán los identificadores y serán transcritos normalmente.

> #### `simboloDelimitador` [Tipo: `const char`]
> Caracter específico con el cual se finaliza la impresión de una función. Por ejemplo, puedes poner `';'` para los archivos `.h`, y `'{'` para ordenar las cabezeras de tu archivo `.cpp`.

---

#### Caso A — `mostrarFunciones = false`

**INPUT:**
```cpp
bool esFemboy(int edad   , char genero){
    return (edad > 15 and genero == 'M' and strcmp(gustoMusical,"Kpop") == 0);
}

bool puedeChambear (  int    edad ){
    return edad >= 18;
}

bool esPeruano(char color){
    return     color ==  'C';
}

void mostrarDerechos( struct  Persona  usu  ){
    if(not(esPeruano(usu.color) and strcmp(usu.apellido,"Quispe") == 0)){
        imprimirConstitucionNazi();
    }else{
        cout<<"..."<<endl;
        exit(1);
    }
}
```

**OUTPUT:**
```
[#] >> WARNING POR ARCHIVO VACIO
```

---

#### Caso B — `mostrarFunciones = true`, `suprimirVariables = false`, `simboloDelimitador = ';'`

**INPUT:**
```cpp
bool esFemboy(int edad   , char genero){
    return (edad > 15 and genero == 'M' and strcmp(gustoMusical,"Kpop") == 0);
}

bool puedeChambear (  int    edad ){
    return edad >= 18;
}

bool esPeruano(char color){
    return     color ==  'C';
}

void mostrarDerechos( struct  Persona  usu  ){
    if(not(esPeruano(usu.color) and strcmp(usu.apellido,"Quispe") == 0)){
        imprimirConstitucionNazi();
    }else{
        cout<<"..."<<endl;
        exit(1);
    }
}
```

**OUTPUT:**
```cpp
bool esFemboy(int edad,char genero);

bool puedeChambear(int edad);

bool esPeruano(char color);

void mostrarDerechos(struct Persona usu);
```

---

#### Caso C — `mostrarFunciones = true`, `suprimirVariables = true`, `simboloDelimitador = ';'`

**INPUT:**
```cpp
bool esFemboy(int edad   , char genero){
    return (edad > 15 and genero == 'M' and strcmp(gustoMusical,"Kpop") == 0);
}

bool puedeChambear (  int    edad ){
    return edad >= 18;
}

bool esPeruano(char color){
    return     color ==  'C';
}

void mostrarDerechos( struct  Persona  usu  ){
    if(not(esPeruano(usu.color) and strcmp(usu.apellido,"Quispe") == 0)){
        imprimirConstitucionNazi();
    }else{
        cout<<"..."<<endl;
        exit(1);
    }
}
```

**OUTPUT:**
```cpp
bool esFemboy(int,char);

bool puedeChambear(int);

bool esPeruano(char);

void mostrarDerechos(struct Persona);
```

---

#### Caso D — `mostrarFunciones = true`, `suprimirVariables = false`, `simboloDelimitador = '{'`

**INPUT:**
```cpp
bool esFemboy(int edad   , char genero){
    return (edad > 15 and genero == 'M' and strcmp(gustoMusical,"Kpop") == 0);
}

bool puedeChambear (  int    edad ){
    return edad >= 18;
}

bool esPeruano(char color){
    return     color ==  'C';
}

void mostrarDerechos( struct  Persona  usu  ){
    if(not(esPeruano(usu.color) and strcmp(usu.apellido,"Quispe") == 0)){
        imprimirConstitucionNazi();
    }else{
        cout<<"..."<<endl;
        exit(1);
    }
}
```

**OUTPUT:**
```cpp
bool esFemboy(int edad,char genero){

bool puedeChambear(int edad){

bool esPeruano(char color){

void mostrarDerechos(struct Persona usu){
```

---

## Aclaraciones de Formato

### Escritura "inclusive"
Este programa se estandarizó de manera que es independiente de la forma de codificar de cada uno de ustedes. Por ejemplo, pueden escribir `const int *` de cualquier manera con tal que el compilador lo valide; es decir, almenos para este ejemplo, podrían escribir `const int*` o `const int          *   ` y no habría ningun problema.

### Agrupación por Elementos vs Agrupación por Sentencias
Las inicializaciones de arreglos por `{}` se ajustan por cada elemento de primer nivel. Por ejemplo, si se tiene:
```cpp
char vocales[5] = {'a','e','i','o','u'};
```
Los elementos de primer nivel son cada una de las letras incluyendo a las comillas y comas. No obstante, si se tiene un arreglo de arreglos, entonces el primer nivel de elementos de estos sería una 'sentencia'. Por ejemplo, si se tiene:
```cpp
int triadas[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
```
Ahora el elemento los elementos de primer nivel son cada triada incluyendo las comillas, comas y llaves. Entonces, en caso de algun ajuste de margen, lo que bajará será toda la triada como sentencia. Por ejemplo, `{7,8,9}};`.

Cabe resaltar que el programa no reajusta a un mismo elemento más de una vez, por lo que si la sentencia o elemento se excede del margen de página, se mantendrá así, pues en realidad se ve mejor de esta forma..

### Operadores en Asignaciones
Existen casos muuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuy raros en los cuales se asignan valores constantes en los archivos `.cpp` como el resultado de una función con uno que otro valor adicional. Para estos casos, el programa toma el ajuste a margen otra vez hacia el primer nivel. Además, los operandos de la signación y los subOperandos de las funciones de primer nivel utilizadas aquí se segmentarán para dar un mejor estilo.

**INPUT:**
```cpp
const int  * numVida[18][queso] = dameAlgoRaro('a'*12+2.5-abc,
                                        arr[12+';' 
                                             ]) or 12-1;
```

**OUTPUT:**
```cpp
const int *numVida[18][queso] = dameAlgoRaro('a'*12 + 2.5 - abc,arr[12+';']) or
                                12 - 1;
```

### Asignaciones en Parámetros
Una de las features implica a que se puedan inicializar parámetros. Por ello, el formato de esta parte es básicamente la misma que la de los operadores en asignaciones.

**INPUT:**
```cpp
void ola(const int *palabra[';']['a']=12+vida(abc[12][12]+12*15+7+1)){
    cout<<"bai bai * mewing *";
    cout<<endl;
}
```

**OUTPUT:**
```cpp
void ola(const int *[';']['a'] = 12 + vida(abc[12][12] + 12*15 + 7 + 1));
```

> *Si no te sale algo, la verdad no se que pudieses haber puesto xD*

---

## Sugerencias Anti-Errores

### Configuración de Controladores Recomendada
Asumiendo que el 99.937% de la gente que lea esto, no habrá leído todo lo de arriba, les dejo los valores de "fábrica" de los controladores para que así puedan usar el programa rápidamente..

```
ajustarPorMargen     = true
limitePorMargen      = 80
ordenarDeclaraciones = true
criteriosDeOrdenamiento = "ACC"

procesarAsignaciones = true

procesarFunciones    = true
suprimirVariables    = true
simboloDelimitador   = ';'
```

### Añadir nuevas TADs al diccionario de Keywords
En caso uses `typedef` o simplemente definas una `struct`, debes añadir este nuevo identificador al arreglo de `Keywords` con el formato preestablecido, pues la ubicacion de declaraciones se hace hasta encontrar una nueva palabra clave como `int` o `const` para poder continuar. Por ello si no agregas el nuevo identificador, no se podría diferenciar de una palabra cualquiera del archivo, o incluso saltar algun error de partición.

### Corroborar el tamaño de tus identificadores
Los idenfiticadores de los parámetros, palabras clave, operadores, etc. tienen tamaños distintos, porque se agrupan de manera distinta. Por ello, deberás acceder al archivo `declaraciones.h` para verificarlo y así no tener un error de capacidad de memoria.

### Corroborar los Comentarios
Se ha validado en las situaciones necesarias la posible existencia de comentarios que puedan alterar la secuencia del programa, no obstante, siempre puede haber un caso por ahí que no se haya tomado en cuenta.

### Mandar programas correctos
Este programa valida varias formas de escribir, pero todas son validadas por el compilador de C++, por lo que, obviamente, no va a funcionar si se mandan cosas como `"bool queso int papaNoel esNavidadAssembler(que,bool)--"`. No obstante, uno que otro warning interrumpirá el programa para reportar un error, pero enfatizando el punto anterior, puede ser que haya un caso por ahí que no se haya tomado en cuenta.

### Espacios >> Tabs
Preferiblemente, para un buen funcionamiento del programa, se recomienda usar espacios en lugar de los tabs en los archivos, pues seguro una que otra validacion aun no está 100% asegurada con respecto a estos.

---

## FAQ & Cositas

En caso tengas problemas con el programa, o consideres que faltan funciones, puedes encontrarme en el discord de informática!

> **INF Syndicate🐧:** https://discord.gg/YMhYSs2PP3

---

> **DISCLAIMER A:**  
> Obviamente existen millones de casos en los que se puede escribir un programa en este lenguaje, y que justo de los justos seas más salado que café con sal y tu forma no esté validada por el programa.. El programa se hizo pensando en DEMASIADOS casos a mi parecer, y bueno, si faltan alguno.. dimelo! Aunque cabe resaltar que los casos mencionados en las aclaraciones NO son errores, sino que, almenos a mi parecer, ese formato es el más ordenado. Además que es medianamente estándar para un público general..

> **DISCLAIMER B:**  
> Ya se que en algunos casos la modulación y complejidad del programa no es la mejor.. esto se irá mejorando en futuras versiones.. o no xd  
> Pero por favor, 30ms - 80ms en promedio para transcribir 40 funciones es muy bueno a mi parecer!

---

**Q: Mucho texto.jpg ¿Cual seria la configuracion estándar?**  
A: `ajustarPorMargen = true` `limiteMargen = 80` `ordenarDeclaraciones = true` `tipoOrdenamiento = "ACC"` `procesarAsignaciones = true` `procesarFunciones = true` `suprimirVariables = true` `simboloDelimitador = ';'`

**Q: ¿Lo puedo compartir?**  
A: jaja como si valiera xD El programa es de uso libre! Si lo comparten, sería chevere que me mencionen o almenos compartan el discord para que pueda llegar a más gente!

**Q: No me gusta el formato de ordenamiento.. ¿Cómo lo cambio?**  
A: Haciendo tu propio programa! :thumbs-up:!  
Ehh, como mencioné, el formato es predeterminado por obvias razones, y lo hice de una manera estándar para que les pueda servir a todos.. Aunque, almenos a mi parecer, con el buen uso de los controladores debería ser suficiente.

**Q: Estoy en un nivel más avanzado y uso más palabras claves a las predefinidas. ¿Las debo agregar? ¿cómo lo hago?**  
A: Claro que sí! Literalmente es lo que mencioné en las sugerencias.. Bueno, en resumen, simplemente añade tu TAD o nueva KW a la lista de KW.

**Q: ¿Dónde y cómo enviarás las actualizaciones?**  
A: Simplemente mandaré el programa actualizado en formato zip a través del discord mencionado anteriormente. Considero que es lo más fácil por ahora.

**Q: Pero.. ¿eres o no eres?**  
A: Esas preguntas no me dejan dormir de noche.jpg

**Q: ¿Cuando subirás un nuevo vídeo a tu canal de Youtube?**  
A: Bueno! creo que acabamos con las preguntas u-u!

---

*Autor: CyndxTs o.0?! — Versión: 2.0*
