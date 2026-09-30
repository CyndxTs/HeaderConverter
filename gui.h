/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  gui.h
 * Autor:               CyndxTs
/*/

#ifndef HEADERCONVERTER_GUI_H
#define HEADERCONVERTER_GUI_H
#include "converter.h"

// Declaracion de clases de Qt
class QString;
class QWidget;
class QDialog;
class QGroupBox;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QTextEdit;
class QScrollArea;
class QButtonGroup;
class QDialogButtonBox;

// Definicion: 'FormatControls'
typedef struct {
    QCheckBox *c_AjustarPorMargen;          // [#] Controlador de 'ajustarPorMargen'.
    QLineEdit *i_LimitePorMargen;           //  > Subcontrolador de 'limitePorMargen'.
    QCheckBox *c_OrdenarDeclaraciones;      // [#] Controlador de 'ordenarDeclaraciones'.
    QLineEdit *i_CriteriosDeOrdenamiento;   //  > Subcontrolador de 'criteriosDeOrdenamiento'.
    QCheckBox *c_EspaciarSubelementos;      // [#] Controlador de 'espaciarSubelementos'.
    QCheckBox *c_ProcesarAsignaciones;      // [#] Controlador de 'procesarAsignaciones'.
    QCheckBox *c_ProcesarFunciones;         // [#] Controlador de 'procesarFunciones'.
    QCheckBox *c_SuprimirVariables;         //  > Subcontrolador de 'suprimirVariables'.
    QComboBox *s_SimboloDelimitador;        //  > Subcontrolador de 'simboloDelimitador'.
} FormatControls;

void initGUI();

void prepararCarpetaDeTrabajo();

QWidget *crearLadoIzquierdo(QWidget *, FormatControls &);

QVBoxLayout *crearLadoDerecho(QTextEdit *&, QTextEdit *&, QPushButton *&);

void cargarFormatoEnControles(const FormatControls &);

void conectarBotonDeConversion(QPushButton *, QTextEdit *, QTextEdit *, const FormatControls &);

QGroupBox *crearGrupoDeEdicion(QWidget *, const char *, void (*)(QWidget *));

QGroupBox *crearGrupoDeFormato(QWidget *, FormatControls &);

QGroupBox *crearPanelDeEntrada(QTextEdit *&, QPushButton *&);

QWidget *crearSeparadorDeSecciones();

QGroupBox *crearPanelDeSalida(QTextEdit *&);

void procesarConversion(QTextEdit *, QTextEdit *, const FormatControls &);

void agregarControladorDeMargen(QWidget *, QVBoxLayout *, FormatControls &);

void agregarControladorDeOrdenamiento(QWidget *, QVBoxLayout *, FormatControls &);

void agregarControladorDeEspaciado(QWidget *, QVBoxLayout *, FormatControls &);

void agregarControladorDeAsignaciones(QWidget *, QVBoxLayout *, FormatControls &);

void agregarControladorDeFunciones(QWidget *, QVBoxLayout *, FormatControls &);

void abrirPanelDeEdicionDePalabrasClave(QWidget *);

void abrirPanelDeEdicionDeOperadores(QWidget *);

void abrirPanelDeEdicionDeCriterios(QWidget *, QLineEdit *);

ProcessingFormat leerFormatoDeControles(const FormatControls &);

void agregarFilaDePalabraClave(QVBoxLayout *, const char *, bool);

void leerPalabrasClaveDeLista(QVBoxLayout *, Keyword *);

void agregarCabeceraDeOperadores(QDialog *, QVBoxLayout *);

void agregarFilaDeOperador(QVBoxLayout *, const Operator &);

void leerOperadoresDeLista(QVBoxLayout *, Operator *);

void agregarCabeceraDeCriterios(QDialog *, QGridLayout *);

QButtonGroup *agregarEjeDeCriterios(QDialog *, QGridLayout *, int, const char *, char);

QPushButton *crearBotonDeInformacion(QWidget *, const QString &, int);

QWidget *crearCeldaCentrada(QWidget *, QWidget *, int, bool);

QHBoxLayout *crearFilaDeControlador(QWidget *, const char *, const QString &, QCheckBox *);

QWidget *crearFilaDeSubcontrolador(const char *, QWidget *, QWidget *);

void vincularSubcontrolador(QCheckBox *, QWidget *);

QScrollArea *crearAreaDeLista(QVBoxLayout *&);

QDialogButtonBox *agregarBotonesDeConfirmacion(QDialog *, QVBoxLayout *);

void eliminarFilaDeLista(QVBoxLayout *, QHBoxLayout *);

int contarFilasDeLista(QVBoxLayout *);

void cargarArchivoEnTexto(const char *, QTextEdit *);

void abrirModal(QWidget *, const QString &, const QString &);

void obtenerTextoDeWarning(const ConversionWarning &, QString &, QString &);

#endif //HEADERCONVERTER_GUI_H