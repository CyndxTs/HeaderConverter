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

#include <fstream>
using namespace std;

void initGUI() {
    QWidget *ventana = new QWidget();
    ventana->setWindowTitle("HeaderConverter");
    ventana->resize(1000, 600);

    auto *layoutPrincipal = new QHBoxLayout();

    // =========================
    // 🔹 IZQUIERDA
    // =========================
    auto *layoutIzquierdo = new QVBoxLayout();

    // Keywords
    auto *grupoKeywords = new QGroupBox("Keywords");
    auto *layoutKeywords = new QVBoxLayout();
    layoutKeywords->addWidget(new QPushButton("Edit"));
    grupoKeywords->setLayout(layoutKeywords);

    // Operators
    auto *grupoOperadores = new QGroupBox("Operators");
    auto *layoutOperadores = new QVBoxLayout();
    layoutOperadores->addWidget(new QPushButton("Edit"));
    grupoOperadores->setLayout(layoutOperadores);

    // =========================
    // 🔸 FORMAT
    // =========================
    auto *grupoFormato = new QGroupBox("Format");
    auto *layoutFormato = new QVBoxLayout();

    // Adjust margin
    auto *layoutMargen = new QHBoxLayout();
    auto *chkMargen = new QCheckBox("Adjust to margin");
    auto *btnInfoMargen = new QPushButton("ℹ");
    btnInfoMargen->setFixedWidth(20);
    layoutMargen->addWidget(chkMargen);
    layoutMargen->addStretch();
    layoutMargen->addWidget(btnInfoMargen);

    auto *inputMargen = new QLineEdit();
    inputMargen->setVisible(false);

    QObject::connect(chkMargen, &QCheckBox::toggled,
                     inputMargen, &QWidget::setVisible);

    QObject::connect(btnInfoMargen, &QPushButton::clicked, [=]() {
        QMessageBox::information(ventana, "Adjust Margin",
        "Limits line length.\nMargin limit defines max characters.");
    });

    // Sort
    auto *layoutSort = new QHBoxLayout();
    auto *chkOrdenar = new QCheckBox("Sort declarations");
    auto *btnInfoSort = new QPushButton("ℹ");
    btnInfoSort->setFixedWidth(20);
    layoutSort->addWidget(chkOrdenar);
    layoutSort->addStretch();
    layoutSort->addWidget(btnInfoSort);

    auto *comboCriterios = new QComboBox();
    comboCriterios->setVisible(false);

    QString letras = "AD";
    for (auto a : letras)
        for (auto b : letras)
            for (auto c : letras)
                comboCriterios->addItem(QString("%1%2%3").arg(a).arg(b).arg(c));

    QObject::connect(chkOrdenar, &QCheckBox::toggled,
                     comboCriterios, &QWidget::setVisible);

    QObject::connect(btnInfoSort, &QPushButton::clicked, [=]() {
        QMessageBox::information(ventana, "Sort Declarations",
        "Orders declarations.\nCriteria defines sorting priority.");
    });

    // Space subelements
    auto *layoutEsp = new QHBoxLayout();
    auto *chkEspaciar = new QCheckBox("Space subelements");
    auto *btnInfoEsp = new QPushButton("ℹ");
    btnInfoEsp->setFixedWidth(20);
    layoutEsp->addWidget(chkEspaciar);
    layoutEsp->addStretch();
    layoutEsp->addWidget(btnInfoEsp);

    QObject::connect(btnInfoEsp, &QPushButton::clicked, [=]() {
        QMessageBox::information(ventana, "Space subelements",
        "Adds spaces after separators except last.");
    });

    // Assignments
    auto *layoutAsig = new QHBoxLayout();
    auto *chkAsignaciones = new QCheckBox("Process assignments");
    auto *btnInfoAsig = new QPushButton("ℹ");
    btnInfoAsig->setFixedWidth(20);
    layoutAsig->addWidget(chkAsignaciones);
    layoutAsig->addStretch();
    layoutAsig->addWidget(btnInfoAsig);

    QObject::connect(btnInfoAsig, &QPushButton::clicked, [=]() {
        QMessageBox::information(ventana, "Process Assignments",
        "Processes global assignments.");
    });

    // Functions
    auto *layoutFunc = new QHBoxLayout();
    auto *chkFunciones = new QCheckBox("Process functions");
    auto *btnInfoFunc = new QPushButton("ℹ");
    btnInfoFunc->setFixedWidth(20);
    layoutFunc->addWidget(chkFunciones);
    layoutFunc->addStretch();
    layoutFunc->addWidget(btnInfoFunc);

    auto *chkSuprimir = new QCheckBox("Suppress variables");
    auto *comboDelimitador = new QComboBox();
    comboDelimitador->addItems({";", "{"});

    chkSuprimir->setVisible(false);
    comboDelimitador->setVisible(false);

    QObject::connect(chkFunciones, &QCheckBox::toggled,
                     chkSuprimir, &QWidget::setVisible);
    QObject::connect(chkFunciones, &QCheckBox::toggled,
                     comboDelimitador, &QWidget::setVisible);

    QObject::connect(btnInfoFunc, &QPushButton::clicked, [=]() {
        QMessageBox::information(ventana, "Process Functions",
        "Processes functions.\nSuppress hides parameter names.\nDelimiter defines output symbol.");
    });

    layoutFormato->addLayout(layoutMargen);
    layoutFormato->addWidget(inputMargen);
    layoutFormato->addLayout(layoutSort);
    layoutFormato->addWidget(comboCriterios);
    layoutFormato->addLayout(layoutEsp);
    layoutFormato->addLayout(layoutAsig);
    layoutFormato->addLayout(layoutFunc);
    layoutFormato->addWidget(chkSuprimir);
    layoutFormato->addWidget(comboDelimitador);

    grupoFormato->setLayout(layoutFormato);

    auto *botonConvertir = new QPushButton("Convert");

    layoutIzquierdo->addWidget(grupoKeywords);
    layoutIzquierdo->addWidget(grupoOperadores);
    layoutIzquierdo->addWidget(grupoFormato);
    layoutIzquierdo->addStretch();
    layoutIzquierdo->addWidget(botonConvertir);

    // =========================
    // 🔹 DERECHA
    // =========================
    auto *layoutDerecho = new QVBoxLayout();

    auto *grupoInput = new QGroupBox("Input");
    auto *layoutInput = new QVBoxLayout();
    auto *textoEntrada = new QTextEdit();
    layoutInput->addWidget(textoEntrada);
    grupoInput->setLayout(layoutInput);

    // cargar input
    {
        ifstream src = abrirArchivo_IFS("../resources/CodeSource.txt");
        string contenido((istreambuf_iterator<char>(src)), istreambuf_iterator<char>());
        textoEntrada->setText(QString::fromStdString(contenido));
        src.close();
    }

    auto *barra = new QFrame();
    barra->setFrameShape(QFrame::HLine);

    auto *grupoOutput = new QGroupBox("Output");
    auto *layoutOutput = new QVBoxLayout();
    auto *textoSalida = new QTextEdit();
    textoSalida->setReadOnly(true);

    auto *btnCopiar = new QPushButton("Copy");

    layoutOutput->addWidget(textoSalida);
    layoutOutput->addWidget(btnCopiar);
    grupoOutput->setLayout(layoutOutput);

    // cargar output
    {
        ifstream res = abrirArchivo_IFS("../resources/HeaderConversion.txt");
        string contenido((istreambuf_iterator<char>(res)), istreambuf_iterator<char>());
        textoSalida->setText(QString::fromStdString(contenido));
        res.close();
    }

    // =========================
    // 🔹 CARGAR PF AL INICIO
    // =========================
    cargarFormatoDeProcesamiento();

    chkMargen->setChecked(pf.ajustarPorMargen);
    inputMargen->setText(QString::number(pf.limitePorMargen));
    inputMargen->setVisible(pf.ajustarPorMargen);

    chkOrdenar->setChecked(pf.ordenarDeclaraciones);
    comboCriterios->setCurrentText(pf.criteriosDeOrdenamiento);
    comboCriterios->setVisible(pf.ordenarDeclaraciones);

    chkEspaciar->setChecked(pf.espaciarSubelementos);
    chkAsignaciones->setChecked(pf.procesarAsignaciones);

    chkFunciones->setChecked(pf.procesarFunciones);
    chkSuprimir->setChecked(pf.suprimirVariables);
    comboDelimitador->setCurrentText(QString(pf.simboloDelimitador));

    chkSuprimir->setVisible(pf.procesarFunciones);
    comboDelimitador->setVisible(pf.procesarFunciones);

    // =========================
    // 🔹 BOTÓN CONVERTIR
    // =========================
    QObject::connect(botonConvertir, &QPushButton::clicked, [=]() {

        ofstream src = abrirArchivo_OFS("../resources/CodeSource.txt");
        src << textoEntrada->toPlainText().toStdString();
        src.close();

        ProcessingFormat nuevoPF;
        nuevoPF.ajustarPorMargen = chkMargen->isChecked();
        nuevoPF.limitePorMargen = inputMargen->text().toInt();
        nuevoPF.ordenarDeclaraciones = chkOrdenar->isChecked();
        strcpy(nuevoPF.criteriosDeOrdenamiento,
               comboCriterios->currentText().toStdString().c_str());
        nuevoPF.espaciarSubelementos = chkEspaciar->isChecked();
        nuevoPF.procesarAsignaciones = chkAsignaciones->isChecked();
        nuevoPF.procesarFunciones = chkFunciones->isChecked();
        nuevoPF.suprimirVariables = chkSuprimir->isChecked();
        nuevoPF.simboloDelimitador =
            comboDelimitador->currentText().toStdString()[0];

        actualizarFormatoDeProcesamiento(nuevoPF);
        cargarFormatoDeProcesamiento();

        cargarListaDePalabrasClave();
        cargarListaDeOperadores();

        // 🔥 LIMPIEZA REAL
        ofstream clean = abrirArchivo_OFS("../resources/HeaderConversion.txt");
        clean.close();

        ifstream in = abrirArchivo_IFS("../resources/CodeSource.txt");
        ofstream out = abrirArchivo_OFS("../resources/HeaderConversion.txt");

        HeaderConversion(in, out);

        in.close();
        out.close();

        // 🔥 LIMPIAR UI antes de cargar
        textoSalida->clear();

        ifstream res = abrirArchivo_IFS("../resources/HeaderConversion.txt");
        string contenido((istreambuf_iterator<char>(res)), istreambuf_iterator<char>());
        textoSalida->setText(QString::fromStdString(contenido));
        res.close();
    });

    QObject::connect(btnCopiar, &QPushButton::clicked, [=]() {
        QApplication::clipboard()->setText(textoSalida->toPlainText());
    });

    layoutDerecho->addWidget(grupoInput);
    layoutDerecho->addWidget(barra);
    layoutDerecho->addWidget(grupoOutput);

    layoutPrincipal->addLayout(layoutIzquierdo, 1);
    layoutPrincipal->addLayout(layoutDerecho, 3);

    ventana->setLayout(layoutPrincipal);
    ventana->show();
}