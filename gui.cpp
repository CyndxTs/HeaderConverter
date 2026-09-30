/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  gui.cpp
 * Autor:               CyndxTs
/*/

#include "gui.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QFrame>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QDialog>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

// Declaracion de constantes
const int anc_OP = 120;                                     // Ancho de columna 'Operador' en el panel de edicion de operadores.
const int anc_SG = 90;                                      // Ancho de columna 'Segmentador' en el panel de edicion de operadores.
const char opcionesDeOrdenamiento[] = {'A', 'C', 'D', 0};   // Tipos de ordenamiento por eje. [Ascendente, Consecuente, Descendente]

                      /* - / Funciones Principales / - */

// Modulo de inicializacion de interfaz grafica
void initGUI() {
    // Preparacion de carpeta de archivos de trabajo
    prepararCarpetaDeTrabajo();
    // Declaracion & Inicializacion de variables
    FormatControls cf {};
    QTextEdit *t_Entrada = nullptr, *t_Salida = nullptr;
    QPushButton *b_Convertir = nullptr;
    QWidget *v_Principal = new QWidget();
    QHBoxLayout *ch_Principal = new QHBoxLayout();
    // Configuracion de ventana principal
    v_Principal->setWindowTitle("HeaderConverter");
    v_Principal->resize(1000, 600);
    // Construccion de lado izquierdo y lado derecho
    ch_Principal->addWidget(crearLadoIzquierdo(v_Principal, cf));
    ch_Principal->addLayout(crearLadoDerecho(t_Entrada, t_Salida, b_Convertir), 1);
    v_Principal->setLayout(ch_Principal);
    // Carga de estado inicial de controles y textos
    cargarFormatoEnControles(cf);
    cargarArchivoEnTexto(rutaDeRecurso("Source.txt").c_str(), t_Entrada);
    cargarArchivoEnTexto(rutaDeRecurso("Conversion.txt").c_str(), t_Salida);
    // Conexion de boton de conversion
    conectarBotonDeConversion(b_Convertir, t_Entrada, t_Salida, cf);
    // Muestra de ventana principal
    v_Principal->show();
}

                      /* - / Funciones Secundarias / - */

// Modulo de preparacion de carpeta de trabajo [AppData del usuario, con copia inicial de valores por defecto]
void prepararCarpetaDeTrabajo() {
    // Declaracion & Inicializacion de variables
    const char *archivos[] = {"Keywords.csv", "Operators.csv", "ProcessingFormat.csv",
                              "Source.txt", "Conversion.txt", nullptr};
    QString carpetaTrabajo = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString carpetaDefectos = QCoreApplication::applicationDirPath() + "/../resources";
    // Creacion de carpeta de trabajo
    QDir().mkpath(carpetaTrabajo);
    // Copia de valores por defecto faltantes
    for (int i = 0; archivos[i]; i++) {
        QString destino = carpetaTrabajo + "/" + archivos[i];
        QString origen = carpetaDefectos + "/" + archivos[i];
        if (QFile::exists(destino)) continue;
        if (not QFile::copy(origen, destino)) {
            // Creacion de archivo vacio si no existe valor por defecto
            QFile archVacio(destino);
            archVacio.open(QIODevice::WriteOnly);
            archVacio.close();
        }
    }
    // Definicion de carpeta de trabajo para el conversor
    fijarCarpetaDeTrabajo(carpetaTrabajo.toLocal8Bit().constData());
}

// Modulo de creacion de lado izquierdo [Palabras clave, operadores y formato]
QWidget *crearLadoIzquierdo(QWidget *v_Principal, FormatControls &cf) {
    // Declaracion & Inicializacion de variables
    QWidget *w_LadoIzquierdo = new QWidget();
    QVBoxLayout *cv_LadoIzquierdo = new QVBoxLayout();
    // Insercion de grupos de controladores
    cv_LadoIzquierdo->addWidget(crearGrupoDeEdicion(v_Principal, "Keywords", abrirPanelDeEdicionDePalabrasClave));
    cv_LadoIzquierdo->addWidget(crearGrupoDeEdicion(v_Principal, "Operators", abrirPanelDeEdicionDeOperadores));
    cv_LadoIzquierdo->addWidget(crearGrupoDeFormato(v_Principal, cf));
    cv_LadoIzquierdo->addStretch();
    // Configuracion de contenedor
    w_LadoIzquierdo->setLayout(cv_LadoIzquierdo);
    w_LadoIzquierdo->setMaximumWidth(320);
    return w_LadoIzquierdo;
}
// Modulo de creacion de lado derecho [Entrada, separador y salida]
QVBoxLayout *crearLadoDerecho(QTextEdit *&t_Entrada, QTextEdit *&t_Salida, QPushButton *&b_Convertir) {
    // Declaracion & Inicializacion de variables
    QVBoxLayout *cv_LadoDerecho = new QVBoxLayout();
    // Insercion de paneles
    cv_LadoDerecho->addWidget(crearPanelDeEntrada(t_Entrada, b_Convertir));
    cv_LadoDerecho->addWidget(crearSeparadorDeSecciones());
    cv_LadoDerecho->addWidget(crearPanelDeSalida(t_Salida));
    return cv_LadoDerecho;
}
// Modulo de carga de formato de procesamiento en controles
void cargarFormatoEnControles(const FormatControls &cf) {
    // Carga de formato de procesamiento vigente
    cargarFormatoDeProcesamiento();
    // Asignacion de valores a controladores y subcontroladores
    cf.c_AjustarPorMargen->setChecked(pf.ajustarPorMargen);
    cf.i_LimitePorMargen->setText(QString::number(pf.limitePorMargen));
    cf.c_OrdenarDeclaraciones->setChecked(pf.ordenarDeclaraciones);
    cf.i_CriteriosDeOrdenamiento->setText(pf.criteriosDeOrdenamiento);
    cf.c_EspaciarSubelementos->setChecked(pf.espaciarSubelementos);
    cf.c_ProcesarAsignaciones->setChecked(pf.procesarAsignaciones);
    cf.c_ProcesarFunciones->setChecked(pf.procesarFunciones);
    cf.c_SuprimirVariables->setChecked(pf.suprimirVariables);
    cf.s_SimboloDelimitador->setCurrentText(QString(pf.simboloDelimitador));
}
// Modulo de conexion de boton de conversion
void conectarBotonDeConversion(QPushButton *b_Convertir, QTextEdit *t_Entrada, QTextEdit *t_Salida, const FormatControls &cf) {
    QObject::connect(b_Convertir, &QPushButton::clicked, [=]() {
        procesarConversion(t_Entrada, t_Salida, cf);
    });
}

                      /* - / Funciones Derivadas / - */

// Modulo de creacion de grupo de edicion [Boton que abre un panel de edicion]
QGroupBox *crearGrupoDeEdicion(QWidget *v_Principal, const char *titulo, void (*abrirPanel)(QWidget *)) {
    // Declaracion & Inicializacion de variables
    QGroupBox *g_Edicion = new QGroupBox(titulo);
    QVBoxLayout *cv_Edicion = new QVBoxLayout();
    QPushButton *b_Editar = new QPushButton("Edit");
    // Configuracion de grupo
    cv_Edicion->addWidget(b_Editar);
    g_Edicion->setLayout(cv_Edicion);
    // Conexion de boton de edicion
    QObject::connect(b_Editar, &QPushButton::clicked, [=]() {
        abrirPanel(v_Principal);
    });
    return g_Edicion;
}
// Modulo de creacion de grupo de formato de procesamiento
QGroupBox *crearGrupoDeFormato(QWidget *v_Principal, FormatControls &cf) {
    // Declaracion & Inicializacion de variables
    QGroupBox *g_Formato = new QGroupBox("Format");
    QVBoxLayout *cv_Formato = new QVBoxLayout();
    // Insercion de controladores de formato
    agregarControladorDeMargen(v_Principal, cv_Formato, cf);
    agregarControladorDeOrdenamiento(v_Principal, cv_Formato, cf);
    agregarControladorDeEspaciado(v_Principal, cv_Formato, cf);
    agregarControladorDeAsignaciones(v_Principal, cv_Formato, cf);
    agregarControladorDeFunciones(v_Principal, cv_Formato, cf);
    // Configuracion de grupo
    g_Formato->setLayout(cv_Formato);
    return g_Formato;
}
// Modulo de creacion de panel de entrada
QGroupBox *crearPanelDeEntrada(QTextEdit *&t_Entrada, QPushButton *&b_Convertir) {
    // Declaracion & Inicializacion de variables
    QGroupBox *g_Entrada = new QGroupBox("Input");
    QVBoxLayout *cv_Entrada = new QVBoxLayout();
    t_Entrada = new QTextEdit();
    b_Convertir = new QPushButton("Convert");
    // Configuracion de panel
    cv_Entrada->addWidget(t_Entrada);
    cv_Entrada->addWidget(b_Convertir);
    g_Entrada->setLayout(cv_Entrada);
    return g_Entrada;
}
// Modulo de creacion de separador de secciones
QWidget *crearSeparadorDeSecciones() {
    // Declaracion & Inicializacion de variables
    QWidget *w_Separador = new QWidget();
    QVBoxLayout *cv_Separador = new QVBoxLayout(w_Separador);
    QFrame *f_Barra = new QFrame();
    // Configuracion de separador
    cv_Separador->setContentsMargins(0, 0, 0, 0);
    f_Barra->setFrameShape(QFrame::HLine);
    f_Barra->setFixedHeight(2);
    cv_Separador->addStretch();
    cv_Separador->addWidget(f_Barra);
    cv_Separador->addStretch();
    w_Separador->setFixedHeight(30);
    return w_Separador;
}
// Modulo de creacion de panel de salida
QGroupBox *crearPanelDeSalida(QTextEdit *&t_Salida) {
    // Declaracion & Inicializacion de variables
    QGroupBox *g_Salida = new QGroupBox("Output");
    QVBoxLayout *cv_Salida = new QVBoxLayout();
    QPushButton *b_Copiar = new QPushButton("Copy");
    t_Salida = new QTextEdit();
    t_Salida->setReadOnly(true);
    // Configuracion de panel
    cv_Salida->addWidget(t_Salida);
    cv_Salida->addWidget(b_Copiar);
    g_Salida->setLayout(cv_Salida);
    // Conexion de boton de copia
    QObject::connect(b_Copiar, &QPushButton::clicked, [=]() {
        QApplication::clipboard()->setText(t_Salida->toPlainText());
    });
    return g_Salida;
}
// Modulo de procesamiento de conversion [Boton 'Convert']
void procesarConversion(QTextEdit *t_Entrada, QTextEdit *t_Salida, const FormatControls &cf) {
    // Actualizacion de archivo fuente
    ofstream archOrigen = abrirArchivo_OFS(rutaDeRecurso("Source.txt").c_str());
    archOrigen<<t_Entrada->toPlainText().toStdString();
    archOrigen.close();
    // Actualizacion de archivo de formato de procesamiento
    actualizarArchivoDeFormatoDeProcesamiento(leerFormatoDeControles(cf));
    // Carga de configuraciones vigentes
    cargarFormatoDeProcesamiento();
    cargarListaDePalabrasClave();
    cargarListaDeOperadores();
    // Conversion de archivo
    ConversionWarning aviso {};
    bool convertido = headerConversion(aviso);
    // Actualizacion de panel de salida
    t_Salida->clear();
    cargarArchivoEnTexto(rutaDeRecurso("Conversion.txt").c_str(), t_Salida);
    // Validacion de conversion cancelada
    if (not convertido) {
        if (aviso.id != 0) {
            // Cancelacion por warning emitido
            QString titulo, mensaje;
            obtenerTextoDeWarning(aviso, titulo, mensaje);
            abrirModal(t_Salida->window(), titulo, mensaje);
        } else {
            // Cancelacion por tiempo limite excedido
            abrirModal(t_Salida->window(), "Error",
                QString("The conversion exceeded the time limit of %1 seconds and was cancelled. "
                        "Check the source code for unclosed brackets, quotes or comments, and try again.").arg(lim_CV));
        }
    }
}

                        /* - / Funciones SubDerivadas / - */

// Modulo de insercion de controlador de ajuste por margen
void agregarControladorDeMargen(QWidget *v_Principal, QVBoxLayout *cv_Formato, FormatControls &cf) {
    // Inicializacion de controlador y subcontrolador
    cf.c_AjustarPorMargen = new QCheckBox();
    cf.i_LimitePorMargen = new QLineEdit();
    cf.i_LimitePorMargen->setMaximumWidth(45);
    // Vinculacion de subcontrolador
    QWidget *w_LimitePorMargen = crearFilaDeSubcontrolador("Margin limit", cf.i_LimitePorMargen, nullptr);
    vincularSubcontrolador(cf.c_AjustarPorMargen, w_LimitePorMargen);
    // Insercion de filas
    cv_Formato->addLayout(crearFilaDeControlador(v_Principal, "Adjust to margin",
        "When active, statements wrap to the next line upon reaching the defined margin, "
        "aligning with the opening of the declaration. When inactive, statements continue "
        "on the same line regardless of their length.\n\n"
        "> Margin limit: maximum number of characters allowed per line before wrapping occurs.",
        cf.c_AjustarPorMargen));
    cv_Formato->addWidget(w_LimitePorMargen);
}
// Modulo de insercion de controlador de ordenamiento de declaraciones
void agregarControladorDeOrdenamiento(QWidget *v_Principal, QVBoxLayout *cv_Formato, FormatControls &cf) {
    // Inicializacion de controlador y subcontrolador
    cf.c_OrdenarDeclaraciones = new QCheckBox();
    cf.i_CriteriosDeOrdenamiento = new QLineEdit("AAA");
    cf.i_CriteriosDeOrdenamiento->setReadOnly(true);
    cf.i_CriteriosDeOrdenamiento->setMaximumWidth(40);
    cf.i_CriteriosDeOrdenamiento->setAlignment(Qt::AlignCenter);
    QPushButton *b_EditarCriterios = new QPushButton("...");
    b_EditarCriterios->setFixedWidth(28);
    // Conexion de boton de edicion de criterios
    QLineEdit *i_Criterios = cf.i_CriteriosDeOrdenamiento;
    QObject::connect(b_EditarCriterios, &QPushButton::clicked, [=]() {
        abrirPanelDeEdicionDeCriterios(v_Principal, i_Criterios);
    });
    // Vinculacion de subcontrolador
    QWidget *w_Criterios = crearFilaDeSubcontrolador("Sort criteria", i_Criterios, b_EditarCriterios);
    vincularSubcontrolador(cf.c_OrdenarDeclaraciones, w_Criterios);
    // Insercion de filas
    cv_Formato->addLayout(crearFilaDeControlador(v_Principal, "Sort declarations",
        "When active, declarations are printed sorted according to the defined criteria. "
        "When inactive, they are printed in the same order as in the source file.\n\n"
        "> Sort criteria: three characters defining the sorting criterion for each axis. "
        "The axes are evaluated in order: declaration type, keyword, and identifier. "
        "Each axis is independent and can be configured separately.",
        cf.c_OrdenarDeclaraciones));
    cv_Formato->addWidget(w_Criterios);
}
// Modulo de insercion de controlador de espaciado de subelementos
void agregarControladorDeEspaciado(QWidget *v_Principal, QVBoxLayout *cv_Formato, FormatControls &cf) {
    // Inicializacion de controlador
    cf.c_EspaciarSubelementos = new QCheckBox();
    // Insercion de fila
    cv_Formato->addLayout(crearFilaDeControlador(v_Principal, "Space subelements",
        "When active, a space is added after each separator between the subelements of a "
        "declaration, except after the last one. This applies to function parameters and "
        "grouped assignment elements.",
        cf.c_EspaciarSubelementos));
}
// Modulo de insercion de controlador de procesamiento de asignaciones
void agregarControladorDeAsignaciones(QWidget *v_Principal, QVBoxLayout *cv_Formato, FormatControls &cf) {
    // Inicializacion de controlador
    cf.c_ProcesarAsignaciones = new QCheckBox();
    // Insercion de fila
    cv_Formato->addLayout(crearFilaDeControlador(v_Principal, "Process assignments",
        "When active, global assignments from the source file are processed and included in "
        "the output. When inactive, all assignments are ignored completely and will not appear "
        "in the result.",
        cf.c_ProcesarAsignaciones));
}
// Modulo de insercion de controlador de procesamiento de funciones
void agregarControladorDeFunciones(QWidget *v_Principal, QVBoxLayout *cv_Formato, FormatControls &cf) {
    // Inicializacion de controlador y subcontroladores
    cf.c_ProcesarFunciones = new QCheckBox();
    cf.c_SuprimirVariables = new QCheckBox();
    cf.s_SimboloDelimitador = new QComboBox();
    cf.s_SimboloDelimitador->addItems({";", "{"});
    cf.s_SimboloDelimitador->setMaximumWidth(70);
    // Vinculacion de subcontroladores
    QWidget *w_SuprimirVariables = crearFilaDeSubcontrolador("Suppress variables", cf.c_SuprimirVariables, nullptr);
    QWidget *w_SimboloDelimitador = crearFilaDeSubcontrolador("Delimiter", cf.s_SimboloDelimitador, nullptr);
    vincularSubcontrolador(cf.c_ProcesarFunciones, w_SuprimirVariables);
    vincularSubcontrolador(cf.c_ProcesarFunciones, w_SimboloDelimitador);
    // Insercion de filas
    cv_Formato->addLayout(crearFilaDeControlador(v_Principal, "Process functions",
        "When active, functions from the source file are processed and included in the output. "
        "When inactive, all functions are ignored completely.\n\n"
        "> Suppress variables: when active, the parameter identifiers (variable names) are "
        "omitted from the output, keeping only their types.\n\n"
        "> Delimiter: symbol used to close each function in the output. "
        "Use ';' for header files and '{' for source files.",
        cf.c_ProcesarFunciones));
    cv_Formato->addWidget(w_SuprimirVariables);
    cv_Formato->addWidget(w_SimboloDelimitador);
}
// Modulo de apertura de panel de edicion de palabras clave
void abrirPanelDeEdicionDePalabrasClave(QWidget *v_Principal) {
    // Carga de lista de palabras clave vigente
    cargarListaDePalabrasClave();
    // Declaracion & Inicializacion de variables
    QVBoxLayout *cv_Lista = nullptr;
    QDialog *p_PalabrasClave = new QDialog(v_Principal);
    QVBoxLayout *cv_PalabrasClave = new QVBoxLayout(p_PalabrasClave);
    QScrollArea *d_PalabrasClave = crearAreaDeLista(cv_Lista);
    QPushButton *b_Agregar = new QPushButton("+ Add keyword");
    // Configuracion de panel
    p_PalabrasClave->setWindowTitle("Edit Keywords");
    p_PalabrasClave->setFixedWidth(220);
    p_PalabrasClave->resize(220, 450);
    // Carga de palabras clave en lista
    for (int i = 0; keywords[i].identificador[0]; i++) agregarFilaDePalabraClave(cv_Lista, keywords[i].identificador, true);
    // Conexion de boton de agregar
    QObject::connect(b_Agregar, &QPushButton::clicked, [=]() {
        // Validacion de limite de palabras clave
        if (contarFilasDeLista(cv_Lista) >= max_KW) {
            abrirModal(p_PalabrasClave, "Warning",
                QString("You cannot have more than %1 keywords defined at a time.").arg(max_KW));
            return;
        }
        agregarFilaDePalabraClave(cv_Lista, "", false);
    });
    // Insercion de lista y boton de agregar
    cv_PalabrasClave->addWidget(d_PalabrasClave);
    cv_PalabrasClave->addWidget(b_Agregar);
    // Conexion de botones de confirmacion
    QDialogButtonBox *bb_Confirmacion = agregarBotonesDeConfirmacion(p_PalabrasClave, cv_PalabrasClave);
    QObject::connect(bb_Confirmacion, &QDialogButtonBox::accepted, [=]() {
        Keyword nuevasKeywords[max_KW] {};
        leerPalabrasClaveDeLista(cv_Lista, nuevasKeywords);
        actualizarArchivoDePalabrasClave(nuevasKeywords);
        p_PalabrasClave->accept();
    });
    // Ejecucion de panel
    p_PalabrasClave->exec();
}
// Modulo de apertura de panel de edicion de operadores
void abrirPanelDeEdicionDeOperadores(QWidget *v_Principal) {
    // Carga de lista de operadores vigente
    cargarListaDeOperadores();
    // Declaracion & Inicializacion de variables
    QVBoxLayout *cv_Lista = nullptr;
    QDialog *p_Operadores = new QDialog(v_Principal);
    QVBoxLayout *cv_Operadores = new QVBoxLayout(p_Operadores);
    QScrollArea *d_Operadores = crearAreaDeLista(cv_Lista);
    // Configuracion de panel
    p_Operadores->setWindowTitle("Edit Operators");
    p_Operadores->setFixedWidth(280);
    p_Operadores->resize(280, 450);
    d_Operadores->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Carga de cabecera y operadores en lista
    agregarCabeceraDeOperadores(p_Operadores, cv_Lista);
    for (int i = 0; operators[i].identificador[0]; i++) agregarFilaDeOperador(cv_Lista, operators[i]);
    // Insercion de lista
    cv_Operadores->addWidget(d_Operadores);
    // Conexion de botones de confirmacion
    QDialogButtonBox *bb_Confirmacion = agregarBotonesDeConfirmacion(p_Operadores, cv_Operadores);
    QObject::connect(bb_Confirmacion, &QDialogButtonBox::accepted, [=]() {
        Operator operadoresActualizados[max_OP] {};
        leerOperadoresDeLista(cv_Lista, operadoresActualizados);
        actualizarArchivoDeOperadores(operadoresActualizados);
        p_Operadores->accept();
    });
    // Ejecucion de panel
    p_Operadores->exec();
}
// Modulo de apertura de panel de edicion de criterios de ordenamiento
void abrirPanelDeEdicionDeCriterios(QWidget *v_Principal, QLineEdit *i_Criterios) {
    // Declaracion & Inicializacion de variables
    const char *nombresEjes[3] = {"Type", "Keyword", "Identifier"};
    QString criterioActual = i_Criterios->text();
    QButtonGroup *gb_Ejes[3];
    QDialog *p_Criterios = new QDialog(v_Principal);
    QVBoxLayout *cv_Criterios = new QVBoxLayout(p_Criterios);
    QGridLayout *cg_Criterios = new QGridLayout();
    // Configuracion de panel
    p_Criterios->setWindowTitle("Sort Criteria");
    p_Criterios->setFixedWidth(360);
    cg_Criterios->setColumnStretch(0, 1);
    // Carga de cabecera y ejes en cuadricula
    agregarCabeceraDeCriterios(p_Criterios, cg_Criterios);
    for (int eje = 0; eje < 3; eje++) {
        char valorEje = (eje < criterioActual.length()) ? criterioActual[eje].toLatin1() : 'A';
        gb_Ejes[eje] = agregarEjeDeCriterios(p_Criterios, cg_Criterios, eje, nombresEjes[eje], valorEje);
    }
    // Insercion de cuadricula
    cv_Criterios->addLayout(cg_Criterios);
    // Conexion de botones de confirmacion
    QDialogButtonBox *bb_Confirmacion = agregarBotonesDeConfirmacion(p_Criterios, cv_Criterios);
    QObject::connect(bb_Confirmacion, &QDialogButtonBox::accepted, [=]() {
        QString nuevoCriterio = "";
        for (int eje = 0; eje < 3; eje++) {
            int sel = gb_Ejes[eje]->checkedId();
            nuevoCriterio += (sel >= 0 and sel < 3) ? opcionesDeOrdenamiento[sel] : 'A';
        }
        i_Criterios->setText(nuevoCriterio);
        p_Criterios->accept();
    });
    // Ejecucion de panel
    p_Criterios->exec();
}
// Modulo de lectura de formato de procesamiento desde controles
ProcessingFormat leerFormatoDeControles(const FormatControls &cf) {
    // Declaracion & Inicializacion de variables
    ProcessingFormat nuevoPF {};
    // Lectura de controladores y subcontroladores
    nuevoPF.ajustarPorMargen = cf.c_AjustarPorMargen->isChecked();
    nuevoPF.limitePorMargen = cf.i_LimitePorMargen->text().toInt();
    nuevoPF.ordenarDeclaraciones = cf.c_OrdenarDeclaraciones->isChecked();
    strcpy(nuevoPF.criteriosDeOrdenamiento, cf.i_CriteriosDeOrdenamiento->text().toStdString().c_str());
    nuevoPF.espaciarSubelementos = cf.c_EspaciarSubelementos->isChecked();
    nuevoPF.procesarAsignaciones = cf.c_ProcesarAsignaciones->isChecked();
    nuevoPF.procesarFunciones = cf.c_ProcesarFunciones->isChecked();
    nuevoPF.suprimirVariables = cf.c_SuprimirVariables->isChecked();
    nuevoPF.simboloDelimitador = cf.s_SimboloDelimitador->currentText().toStdString()[0];
    return nuevoPF;
}
// Modulo de insercion de fila de palabra clave en lista
void agregarFilaDePalabraClave(QVBoxLayout *cv_Lista, const char *palabraClave, bool soloLectura) {
    // Declaracion & Inicializacion de variables
    QHBoxLayout *ch_Fila = new QHBoxLayout();
    QLineEdit *i_PalabraClave = new QLineEdit(QString::fromLocal8Bit(palabraClave));
    QPushButton *b_Eliminar = new QPushButton("x");
    // Configuracion de fila
    i_PalabraClave->setReadOnly(soloLectura);
    i_PalabraClave->setMaximumWidth(150);
    b_Eliminar->setFixedWidth(30);
    QObject::connect(b_Eliminar, &QPushButton::clicked, [=]() {
        eliminarFilaDeLista(cv_Lista, ch_Fila);
    });
    ch_Fila->addWidget(i_PalabraClave);
    ch_Fila->addWidget(b_Eliminar);
    cv_Lista->addLayout(ch_Fila);
}
// Modulo de lectura de palabras clave desde lista
void leerPalabrasClaveDeLista(QVBoxLayout *cv_Lista, Keyword *palabrasClave) {
    // Declaracion & Inicializacion de variables
    int cantKw = 0;
    // Lectura de palabras clave por fila
    for (int i = 0; i < cv_Lista->count() and cantKw < max_KW; i++) {
        QLayoutItem *pItem = cv_Lista->itemAt(i);
        if (not pItem) continue;
        QLayout *ch_Fila = pItem->layout();
        if (not ch_Fila) continue;
        QLayoutItem *pPrimero = ch_Fila->itemAt(0);
        if (not pPrimero or not pPrimero->widget()) continue;
        QLineEdit *i_PalabraClave = qobject_cast<QLineEdit *>(pPrimero->widget());
        if (not i_PalabraClave) continue;
        QString texto = i_PalabraClave->text().trimmed();
        if (texto.isEmpty()) continue;
        QByteArray bytes = texto.toLocal8Bit();
        strncpy(palabrasClave[cantKw].identificador, bytes.constData(), med_KW - 1);
        palabrasClave[cantKw].identificador[med_KW - 1] = '\0';
        cantKw++;
    }
    // Validacion de delimitacion de lista
    if (cantKw < max_KW) palabrasClave[cantKw].identificador[0] = '\0';
}
// Modulo de insercion de cabecera de lista de operadores
void agregarCabeceraDeOperadores(QDialog *p_Operadores, QVBoxLayout *cv_Lista) {
    // Declaracion & Inicializacion de variables
    QHBoxLayout *ch_Cabecera = new QHBoxLayout();
    QLabel *e_Operador = new QLabel("Operator");
    QLabel *e_Segmentador = new QLabel("Segmenter");
    QFrame *f_Separador = new QFrame();
    QPushButton *b_Informacion = crearBotonDeInformacion(p_Operadores,
        "When active, the operator is spaced between its operands in the output. "
        "For example, if an operator is marked as a segmenter, it will appear surrounded "
        "by spaces when printed, separating it visually from the values on each side.", 18);
    // Configuracion de cabecera
    e_Operador->setAlignment(Qt::AlignCenter);
    e_Segmentador->setAlignment(Qt::AlignCenter);
    f_Separador->setFrameShape(QFrame::HLine);
    ch_Cabecera->addWidget(crearCeldaCentrada(e_Operador, nullptr, anc_OP, false));
    ch_Cabecera->addWidget(crearCeldaCentrada(e_Segmentador, b_Informacion, anc_SG, false));
    // Insercion de cabecera y separador
    cv_Lista->addLayout(ch_Cabecera);
    cv_Lista->addWidget(f_Separador);
}
// Modulo de insercion de fila de operador en lista
void agregarFilaDeOperador(QVBoxLayout *cv_Lista, const Operator &operador) {
    // Declaracion & Inicializacion de variables
    QHBoxLayout *ch_Fila = new QHBoxLayout();
    QLabel *e_Operador = new QLabel(QString::fromLocal8Bit(operador.identificador));
    QCheckBox *c_Segmentador = new QCheckBox();
    // Configuracion de fila
    ch_Fila->setAlignment(Qt::AlignVCenter);
    e_Operador->setAlignment(Qt::AlignCenter);
    c_Segmentador->setChecked(operador.esSegmentador);
    ch_Fila->addWidget(crearCeldaCentrada(e_Operador, nullptr, anc_OP, false));
    ch_Fila->addWidget(crearCeldaCentrada(c_Segmentador, nullptr, anc_SG, false));
    cv_Lista->addLayout(ch_Fila);
}
// Modulo de lectura de operadores desde lista
void leerOperadoresDeLista(QVBoxLayout *cv_Lista, Operator *operadoresActualizados) {
    // Declaracion & Inicializacion de variables
    int cantOp = 0;
    // Lectura de operadores por fila [Se omiten la cabecera y el separador]
    for (int i = 2; i < cv_Lista->count() and cantOp < max_OP; i++) {
        QLayoutItem *pItem = cv_Lista->itemAt(i);
        if (not pItem) continue;
        QLayout *ch_Fila = pItem->layout();
        if (not ch_Fila or ch_Fila->count() < 2) continue;
        QWidget *w_Operador = ch_Fila->itemAt(0)->widget();
        QWidget *w_Segmentador = ch_Fila->itemAt(1)->widget();
        if (not w_Operador or not w_Segmentador) continue;
        QLabel *e_Operador = w_Operador->findChild<QLabel *>();
        QCheckBox *c_Segmentador = w_Segmentador->findChild<QCheckBox *>();
        if (not e_Operador or not c_Segmentador) continue;
        QByteArray bytes = e_Operador->text().toLocal8Bit();
        strncpy(operadoresActualizados[cantOp].identificador, bytes.constData(), med_OP - 1);
        operadoresActualizados[cantOp].identificador[med_OP - 1] = '\0';
        operadoresActualizados[cantOp].esAcotable = operators[cantOp].esAcotable;
        operadoresActualizados[cantOp].esSegmentador = c_Segmentador->isChecked();
        cantOp++;
    }
    // Validacion de delimitacion de lista
    if (cantOp < max_OP) operadoresActualizados[cantOp].identificador[0] = '\0';
}
// Modulo de insercion de cabecera de cuadricula de criterios [Fila 0: titulos, fila 1: separador]
void agregarCabeceraDeCriterios(QDialog *p_Criterios, QGridLayout *cg_Criterios) {
    // Declaracion & Inicializacion de variables
    const char *informacion[3] = {
        "Ascending order. Declarations are sorted from the lowest to the highest value "
        "on this axis, following alphabetical or type order depending on the axis.",
        "Consecutive order. This axis is ignored and the evaluation moves on to the next one. "
        "Declarations that share the same value on the previous axes maintain their relative "
        "order from the source file.",
        "Descending order. Declarations are sorted from the highest to the lowest value "
        "on this axis, following reverse alphabetical or type order depending on the axis."
    };
    QLabel *e_OrdenarPor = new QLabel("Sort by");
    QFrame *f_Separador = new QFrame();
    // Insercion de titulo de ejes
    e_OrdenarPor->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cg_Criterios->addWidget(e_OrdenarPor, 0, 0);
    // Insercion de titulos de opciones
    for (int i = 0; i < 3; i++) {
        QLabel *e_Opcion = new QLabel(QString(QChar::fromLatin1(opcionesDeOrdenamiento[i])));
        e_Opcion->setAlignment(Qt::AlignCenter);
        QPushButton *b_Informacion = crearBotonDeInformacion(p_Criterios, informacion[i], 18);
        cg_Criterios->addWidget(crearCeldaCentrada(e_Opcion, b_Informacion, 0, true), 0, i + 1);
    }
    // Insercion de separador que ocupa todas las columnas
    f_Separador->setFrameShape(QFrame::HLine);
    cg_Criterios->addWidget(f_Separador, 1, 0, 1, 4);
}
// Modulo de insercion de eje de criterios en cuadricula [Filas 2-4: Type, Keyword, Identifier]
QButtonGroup *agregarEjeDeCriterios(QDialog *p_Criterios, QGridLayout *cg_Criterios, int eje, const char *nombre, char valorEje) {
    // Declaracion & Inicializacion de variables
    QButtonGroup *gb_Eje = new QButtonGroup(p_Criterios);
    QLabel *e_Eje = new QLabel(nombre);
    // Validacion de valor de eje
    if (valorEje != 'C' and valorEje != 'D') valorEje = 'A';
    // Insercion de nombre de eje
    e_Eje->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    cg_Criterios->addWidget(e_Eje, eje + 2, 0);
    // Insercion de opciones de eje
    for (int i = 0; i < 3; i++) {
        QRadioButton *r_Opcion = new QRadioButton();
        gb_Eje->addButton(r_Opcion, i);
        cg_Criterios->addWidget(crearCeldaCentrada(r_Opcion, nullptr, 0, false), eje + 2, i + 1);
        if (valorEje == opcionesDeOrdenamiento[i]) r_Opcion->setChecked(true);
    }
    return gb_Eje;
}

                       /* - / Funciones Auxiliares / - */

// Modulo de creacion de boton de informacion [Abre un modal con el mensaje]
QPushButton *crearBotonDeInformacion(QWidget *v_Padre, const QString &mensaje, int ancho) {
    QPushButton *b_Informacion = new QPushButton("ℹ");
    b_Informacion->setFixedWidth(ancho);
    QObject::connect(b_Informacion, &QPushButton::clicked, [=]() {
        abrirModal(v_Padre, "Information", mensaje);
    });
    return b_Informacion;
}
// Modulo de creacion de celda centrada [Contenido y adicional opcional, con ancho fijo opcional]
QWidget *crearCeldaCentrada(QWidget *w_Contenido, QWidget *w_Adicional, int ancho, bool conMargenes) {
    QWidget *w_Celda = new QWidget();
    QHBoxLayout *ch_Celda = new QHBoxLayout(w_Celda);
    if (not conMargenes) ch_Celda->setContentsMargins(0, 0, 0, 0);
    ch_Celda->addStretch();
    ch_Celda->addWidget(w_Contenido);
    if (w_Adicional != nullptr) ch_Celda->addWidget(w_Adicional);
    ch_Celda->addStretch();
    if (ancho > 0) w_Celda->setFixedWidth(ancho);
    return w_Celda;
}
// Modulo de creacion de fila de controlador [Informacion, nombre y casilla]
QHBoxLayout *crearFilaDeControlador(QWidget *v_Padre, const char *nombre, const QString &informacion, QCheckBox *c_Controlador) {
    QHBoxLayout *ch_Controlador = new QHBoxLayout();
    ch_Controlador->addWidget(crearBotonDeInformacion(v_Padre, informacion, 20));
    ch_Controlador->addWidget(new QLabel(nombre));
    ch_Controlador->addStretch();
    ch_Controlador->addWidget(c_Controlador);
    return ch_Controlador;
}
// Modulo de creacion de fila de subcontrolador [Flecha, nombre y hasta dos controles]
QWidget *crearFilaDeSubcontrolador(const char *nombre, QWidget *w_Control, QWidget *w_Adicional) {
    QWidget *w_Subcontrolador = new QWidget();
    QHBoxLayout *ch_Subcontrolador = new QHBoxLayout(w_Subcontrolador);
    QLabel *e_Flecha = new QLabel("  └");
    e_Flecha->setFixedWidth(22);
    ch_Subcontrolador->setContentsMargins(0, 0, 0, 0);
    ch_Subcontrolador->addWidget(e_Flecha);
    ch_Subcontrolador->addWidget(new QLabel(nombre));
    ch_Subcontrolador->addStretch();
    ch_Subcontrolador->addWidget(w_Control);
    if (w_Adicional != nullptr) ch_Subcontrolador->addWidget(w_Adicional);
    return w_Subcontrolador;
}
// Modulo de vinculacion de visibilidad de subcontrolador a su controlador
void vincularSubcontrolador(QCheckBox *c_Controlador, QWidget *w_Subcontrolador) {
    w_Subcontrolador->setVisible(c_Controlador->isChecked());
    QObject::connect(c_Controlador, &QCheckBox::toggled, w_Subcontrolador, &QWidget::setVisible);
}
// Modulo de creacion de area desplazable de lista
QScrollArea *crearAreaDeLista(QVBoxLayout *&cv_Lista) {
    QScrollArea *d_Lista = new QScrollArea();
    QWidget *w_Contenedor = new QWidget();
    d_Lista->setWidgetResizable(true);
    cv_Lista = new QVBoxLayout(w_Contenedor);
    cv_Lista->setAlignment(Qt::AlignTop);
    d_Lista->setWidget(w_Contenedor);
    return d_Lista;
}
// Modulo de insercion de botones de confirmacion [Ok / Cancel] en panel
QDialogButtonBox *agregarBotonesDeConfirmacion(QDialog *p_Panel, QVBoxLayout *cv_Panel) {
    QDialogButtonBox *bb_Confirmacion = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QHBoxLayout *ch_Confirmacion = new QHBoxLayout();
    ch_Confirmacion->addStretch();
    ch_Confirmacion->addWidget(bb_Confirmacion);
    ch_Confirmacion->addStretch();
    cv_Panel->addLayout(ch_Confirmacion);
    QObject::connect(bb_Confirmacion, &QDialogButtonBox::rejected, p_Panel, &QDialog::reject);
    return bb_Confirmacion;
}
// Modulo de eliminacion de fila de lista
void eliminarFilaDeLista(QVBoxLayout *cv_Lista, QHBoxLayout *ch_Fila) {
    QLayoutItem *pItem;
    while ((pItem = ch_Fila->takeAt(0)) != nullptr) {
        if (pItem->widget()) pItem->widget()->deleteLater();
        delete pItem;
    }
    cv_Lista->removeItem(ch_Fila);
    delete ch_Fila;
}
// Modulo de conteo de filas de lista
int contarFilasDeLista(QVBoxLayout *cv_Lista) {
    int cantFilas = 0;
    for (int i = 0; i < cv_Lista->count(); i++) {
        QLayoutItem *pItem = cv_Lista->itemAt(i);
        if (pItem and pItem->layout()) cantFilas++;
    }
    return cantFilas;
}
// Modulo de carga de archivo en cuadro de texto
void cargarArchivoEnTexto(const char *nombArch, QTextEdit *t_Texto) {
    ifstream archEntrada = abrirArchivo_IFS(nombArch);
    string contenido((istreambuf_iterator<char>(archEntrada)), istreambuf_iterator<char>());
    t_Texto->setText(QString::fromStdString(contenido));
    archEntrada.close();
}
// Modulo de apertura de modal informativo
void abrirModal(QWidget *vPadre, const QString &titulo, const QString &mensaje) {
    //
    QDialog *vModal = new QDialog(vPadre);
    vModal->setWindowTitle(titulo);
    vModal->setFixedWidth(320);
    //
    QVBoxLayout *cvModal = new QVBoxLayout(vModal);
    cvModal->setSpacing(16);
    cvModal->setContentsMargins(16, 16, 16, 16);
    //
    QLabel *eMensaje = new QLabel(mensaje);
    eMensaje->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    eMensaje->setWordWrap(true);
    eMensaje->setTextFormat(Qt::PlainText);
    cvModal->addWidget(eMensaje);
    //
    QPushButton *bAceptar = new QPushButton("OK");
    bAceptar->setFixedWidth(80);
    QHBoxLayout *chAceptar = new QHBoxLayout();
    chAceptar->addStretch();
    chAceptar->addWidget(bAceptar);
    chAceptar->addStretch();
    cvModal->addLayout(chAceptar);
    QObject::connect(bAceptar, &QPushButton::clicked, vModal, &QDialog::accept);
    //
    vModal->exec();
}
// Modulo de obtencion de titulo y mensaje de warning segun su tipo [A, E, L, O, P, U]
void obtenerTextoDeWarning(const ConversionWarning &aviso, QString &titulo, QString &mensaje) {
    QString razon = QString::fromLocal8Bit(aviso.razon);
    switch (aviso.id) {
        case 'A':   // A -> Archive Aperture
            titulo = "ERROR DE APERTURA";
            mensaje = QString("No se encontro el archivo '%1' en el directorio.\n\n"
                "[#] Acciones recomendadas:\n"
                "[A] Verificar la ruta del archivo.\n"
                "[B] Verificar el nombre del archivo ingresado.\n"
                "[C] Verificar si se agrego la extension del archivo.").arg(razon);
            break;
        case 'E':   // E -> Empty
            titulo = "SIN RESULTADOS";
            mensaje = "No existe error como tal.\n"
                "Esto solo significa que no hay nada para convertir.\n\n"
                "[#] Acciones recomendadas:\n"
                "[A] Activar alguno de los controladores de muestra.\n"
                "[B] Editar el archivo fuente.";
            break;
        case 'L':   // L -> Limit
            titulo = "SIN AJUSTE A LIMITE";
            mensaje = QString("No existe error como tal.\n"
                "No obstante, fue imposible acomodar algunas declaraciones respecto al margen "
                "de pagina ['%1']. Por ello, se ignoro el ajuste hacia margen en estas declaraciones.\n"
                "Primera Ubicacion: %2\n\n"
                "[#] Acciones recomendadas:\n"
                "[A] Incrementar el limite de margen.\n"
                "[B] Editar el archivo fuente.\n"
                "[C] Desactivar el controlador de ajuste a margen.").arg(pf.limitePorMargen).arg(razon);
            break;
        case 'O':   // O -> Order
            titulo = "ERROR POR ORDENAMIENTO";
            mensaje = QString("El tipo de ordenamiento '%1' definido en el controlador es invalido.\n\n"
                "[#] Acciones recomendadas:\n"
                "[A] Modificar el valor del controlador de tipo de ordenamiento a alguno de los "
                "tipos predefinidos:\n"
                "    ['A'] Ascendente | ['C'] Consecuente | ['D'] Descendente\n"
                "    Recordar que la secuencia debe ser de unicamente '3' caracteres, y que la "
                "posicion de cada criterio de ordenamiento es:\n"
                "        {Tipo de Declaration}{KeyWords}{Identificadores}\n"
                "    Por ejemplo, con la secuencia 'ADA' el ordenamiento seria:\n"
                "    - Ascendente por Tipo de Declaration\n"
                "    - Descendente por Keyword\n"
                "    - Ascendente por Identificador").arg(razon);
            break;
        case 'P':   // P -> Partition
            titulo = "ERROR POR PARTICION";
            mensaje = QString("Se ha detectado la particion de un identificador.\n"
                "Ubicacion: %1\n\n"
                "[#] Acciones recomendadas:\n"
                "[A] Agregar una palabra clave faltante en el diccionario respectivo.\n"
                "[B] Editar el archivo fuente.").arg(razon);
            break;
        case 'U':   // U -> Unreachable
            titulo = "ESTADO INALCANZABLE";
            mensaje = QString("El programa llego a un punto al que no deberia haber llegado. "
                "No es un problema de tu codigo fuente, sino del propio programa.\n"
                "Detalle: %1").arg(razon);
            break;
        default:
            titulo = "Error";
            mensaje = QString("Se emitio un warning desconocido ['%1'].").arg(QChar::fromLatin1(aviso.id));
    }
}