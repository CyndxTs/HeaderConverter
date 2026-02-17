# 馃搨 HEADER CONVERTER

**Autor:** CyndxTs o.0?!\
**Versi贸n:** 2.0

------------------------------------------------------------------------

## 馃搶 Descripci贸n

Este programa est谩 ideado principalmente para transcribir archivos
`.cpp` hacia archivos `.h`.

Tambi茅n permite:

-   Ordenar un mismo `.h`
-   Evitar copiar funci贸n por funci贸n las cabeceras
-   Ajustar m谩rgenes autom谩ticamente
-   Suprimir variables en par谩metros
-   Aplicar m煤ltiples controladores de formato

------------------------------------------------------------------------

# 馃殌 Resumen de Novedades (v2.0)

-   Validaci贸n de palabras clave compuestas

-   Soporte para asignaciones predefinidas en par谩metros

-   Soporte para sobrecargas de operadores

-   Ordenamiento configurable por:

    `{Tipo de Declaracion} || {KeyWord} || {Identificador}`

-   Nuevos warnings

-   Mejora en comentarios generados en `.h`

-   Cambio interno: `LSE 鈫?LDX`

-   Eliminado ajuste de `subElementos`

------------------------------------------------------------------------

# 鈿?Controladores Generales

## ajustarPorMargen (bool)

Ajusta autom谩ticamente las sentencias cuando superan el l铆mite.

## limitePorMargen (int)

Por defecto: 80 caracteres

## ordenarDeclaraciones (bool)

Ordena declaraciones seg煤n:

criteriosDeOrdenamiento = "ADA"

Valores: - A 鈫?Ascendente - D 鈫?Descendente - C 鈫?Consecuente

Orden aplicado: {Tipo} {KeyWord} {Identificador}

------------------------------------------------------------------------

# 馃Ь Controladores de Asignaciones

## procesarAsignaciones (bool)

Si est谩 deshabilitado: WARNING POR ARCHIVO VACIO

Si est谩 habilitado: const int numQuesos = 10; const char
especimenID\[10\] = "0x983";

------------------------------------------------------------------------

# 馃敡 Controladores de Funciones

## procesarFunciones (bool)

Controla si se transcriben funciones.

## suprimirVariables (bool)

Ejemplo habilitado: bool esFemboy(int,char);

Ejemplo deshabilitado: bool esFemboy(int edad,char genero);

## simboloDelimitador (char)

';' 鈫?Para archivos .h\
'{' 鈫?Para ordenar .cpp

------------------------------------------------------------------------

# 馃洜 Configuraci贸n Recomendada

ajustarPorMargen = true\
limitePorMargen = 80\
ordenarDeclaraciones = true\
criteriosDeOrdenamiento = "ACC"\
procesarAsignaciones = true\
procesarFunciones = true\
suprimirVariables = true\
simboloDelimitador = ';'

------------------------------------------------------------------------

# 鉂?FAQ

**驴Lo puedo compartir?**\
S铆. Es de uso libre.

**驴D贸nde se publicar谩n actualizaciones?**\
En el Discord del autor.

------------------------------------------------------------------------

# 馃憫 Autor

CyndxTs o.0?!\
Versi贸n 2.0
