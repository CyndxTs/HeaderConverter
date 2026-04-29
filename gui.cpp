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
    auto *btnEditKeywords = new QPushButton("Edit");
    layoutKeywords->addWidget(btnEditKeywords);
    grupoKeywords->setLayout(layoutKeywords);

    QObject::connect(btnEditKeywords, &QPushButton::clicked, [=]() {
        cargarListaDePalabrasClave();

        QDialog *dialogo = new QDialog(ventana);
        dialogo->setWindowTitle("Edit Keywords");
        // input(150) + btn(30) + scroll(16) + dialog margins(24) ≈ 220
        dialogo->setFixedWidth(220);
        dialogo->resize(220, 450);

        auto *layoutDialogo = new QVBoxLayout(dialogo);

        auto *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);

        auto *contenedor = new QWidget();
        auto *layoutLista = new QVBoxLayout(contenedor);
        layoutLista->setAlignment(Qt::AlignTop);
        scrollArea->setWidget(contenedor);

        // Poblar con keywords actuales
        for (int i = 0; keywords[i].identificador[0]; i++) {
            auto *fila = new QHBoxLayout();
            auto *input = new QLineEdit(QString::fromLocal8Bit(keywords[i].identificador));
            input->setReadOnly(true);
            input->setMaximumWidth(150);
            auto *btnEliminar = new QPushButton("x");
            btnEliminar->setFixedWidth(30);

            QObject::connect(btnEliminar, &QPushButton::clicked, [=]() {
                QLayoutItem *item;
                while ((item = fila->takeAt(0)) != nullptr) {
                    if (item->widget()) item->widget()->deleteLater();
                    delete item;
                }
                layoutLista->removeItem(fila);
                delete fila;
            });

            fila->addWidget(input);
            fila->addWidget(btnEliminar);
            // sin addStretch() para evitar espacio en blanco sobrante
            layoutLista->addLayout(fila);
        }

        // Botón "+" para agregar nueva keyword
        auto *btnAgregar = new QPushButton("+ Add keyword");
        QObject::connect(btnAgregar, &QPushButton::clicked, [=]() {
            auto *fila = new QHBoxLayout();
            auto *input = new QLineEdit();
            input->setMaximumWidth(150);
            auto *btnEliminar = new QPushButton("x");
            btnEliminar->setFixedWidth(30);

            QObject::connect(btnEliminar, &QPushButton::clicked, [=]() {
                QLayoutItem *item;
                while ((item = fila->takeAt(0)) != nullptr) {
                    if (item->widget()) item->widget()->deleteLater();
                    delete item;
                }
                layoutLista->removeItem(fila);
                delete fila;
            });

            fila->addWidget(input);
            fila->addWidget(btnEliminar);
            // sin addStretch() para evitar espacio en blanco sobrante
            layoutLista->addLayout(fila);
        });

        auto *botones = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        QObject::connect(botones, &QDialogButtonBox::accepted, [=]() {
            Keyword nuevasKeywords[max_KW] {};
            int count = 0;

            for (int i = 0; i < layoutLista->count() && count < max_KW; i++) {
                QLayoutItem *item = layoutLista->itemAt(i);
                if (!item) continue;
                QLayout *filaLayout = item->layout();
                if (!filaLayout) continue;
                QLayoutItem *primerItem = filaLayout->itemAt(0);
                if (!primerItem || !primerItem->widget()) continue;
                QLineEdit *lineEdit = qobject_cast<QLineEdit *>(primerItem->widget());
                if (!lineEdit) continue;
                QString texto = lineEdit->text().trimmed();
                if (texto.isEmpty()) continue;

                QByteArray bytes = texto.toLocal8Bit();
                strncpy(nuevasKeywords[count].identificador, bytes.constData(), med_KW - 1);
                nuevasKeywords[count].identificador[med_KW - 1] = '\0';
                count++;
            }

            if (count < max_KW) nuevasKeywords[count].identificador[0] = '\0';

            actualizarListaDePalabrasClave(nuevasKeywords);
            dialogo->accept();
        });

        QObject::connect(botones, &QDialogButtonBox::rejected, dialogo, &QDialog::reject);

        auto *layoutBotonesKW = new QHBoxLayout();
        layoutBotonesKW->addStretch();
        layoutBotonesKW->addWidget(botones);
        layoutBotonesKW->addStretch();

        layoutDialogo->addWidget(scrollArea);
        layoutDialogo->addWidget(btnAgregar);
        layoutDialogo->addLayout(layoutBotonesKW);

        dialogo->exec();
    });

    // Operators
    auto *grupoOperadores = new QGroupBox("Operators");
    auto *layoutOperadores = new QVBoxLayout();
    auto *btnEditOperators = new QPushButton("Edit");
    layoutOperadores->addWidget(btnEditOperators);
    grupoOperadores->setLayout(layoutOperadores);

    QObject::connect(btnEditOperators, &QPushButton::clicked, [=]() {
        cargarListaDeOperadores();

        QDialog *dialogo = new QDialog(ventana);
        dialogo->setWindowTitle("Edit Operators");
        // anchoOp(120) + anchoChk(90)*2 + scrollbar(17) + dialog margins(24) + spacing(8) = 349 -> 360 holgado
        dialogo->setFixedWidth(360);
        dialogo->resize(360, 450);

        auto *layoutDialogo = new QVBoxLayout(dialogo);

        auto *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        auto *contenedor = new QWidget();
        auto *layoutLista = new QVBoxLayout(contenedor);
        layoutLista->setAlignment(Qt::AlignTop);
        scrollArea->setWidget(contenedor);

        // Anchos de columna
        const int anchoOp  = 120;
        const int anchoChk = 90;

        auto *filaEncabezado = new QHBoxLayout();

        auto *lblNombre = new QLabel("Operator");
        lblNombre->setFixedWidth(anchoOp);

        // Columna "Boundable" con botón ℹ
        auto *layoutEncAcotable = new QHBoxLayout();
        layoutEncAcotable->setContentsMargins(0,0,0,0);
        auto *lblAcotable = new QLabel("Boundable");
        lblAcotable->setAlignment(Qt::AlignCenter);
        auto *btnInfoAcotable = new QPushButton("ℹ");
        btnInfoAcotable->setFixedWidth(18);
        layoutEncAcotable->addStretch();
        layoutEncAcotable->addWidget(lblAcotable);
        layoutEncAcotable->addWidget(btnInfoAcotable);
        layoutEncAcotable->addStretch();
        auto *wEncAcotable = new QWidget();
        wEncAcotable->setFixedWidth(anchoChk);
        wEncAcotable->setLayout(layoutEncAcotable);

        QObject::connect(btnInfoAcotable, &QPushButton::clicked, [=]() {
            QMessageBox::information(dialogo, "Boundable",
                "Description of boundable.");
        });

        // Columna "Segmenter" con botón ℹ
        auto *layoutEncSegment = new QHBoxLayout();
        layoutEncSegment->setContentsMargins(0,0,0,0);
        auto *lblSegmentador = new QLabel("Segmenter");
        lblSegmentador->setAlignment(Qt::AlignCenter);
        auto *btnInfoSegment = new QPushButton("ℹ");
        btnInfoSegment->setFixedWidth(18);
        layoutEncSegment->addStretch();
        layoutEncSegment->addWidget(lblSegmentador);
        layoutEncSegment->addWidget(btnInfoSegment);
        layoutEncSegment->addStretch();
        auto *wEncSegment = new QWidget();
        wEncSegment->setFixedWidth(anchoChk);
        wEncSegment->setLayout(layoutEncSegment);

        QObject::connect(btnInfoSegment, &QPushButton::clicked, [=]() {
            QMessageBox::information(dialogo, "Segmenter",
                "Description of segmenter.");
        });

        filaEncabezado->addWidget(lblNombre);
        filaEncabezado->addWidget(wEncAcotable);
        filaEncabezado->addWidget(wEncSegment);
        // sin addStretch() — evita que el encabezado se expanda y cause scroll horizontal
        layoutLista->addLayout(filaEncabezado);

        auto *sep = new QFrame();
        sep->setFrameShape(QFrame::HLine);
        layoutLista->addWidget(sep);

        // Poblar operadores actuales
        for (int i = 0; operators[i].identificador[0]; i++) {
            auto *fila = new QHBoxLayout();
            fila->setAlignment(Qt::AlignVCenter);

            auto *lblOp = new QLabel(QString::fromLocal8Bit(operators[i].identificador));
            lblOp->setFixedWidth(anchoOp);
            lblOp->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            // Checkbox "Boundable" centrado en su celda
            auto *layoutCeldaAcotable = new QHBoxLayout();
            layoutCeldaAcotable->setContentsMargins(0,0,0,0);
            auto *chkAcotable = new QCheckBox();
            chkAcotable->setChecked(operators[i].esAcotable);
            layoutCeldaAcotable->addStretch();
            layoutCeldaAcotable->addWidget(chkAcotable);
            layoutCeldaAcotable->addStretch();
            auto *wCeldaAcotable = new QWidget();
            wCeldaAcotable->setFixedWidth(anchoChk);
            wCeldaAcotable->setLayout(layoutCeldaAcotable);

            // Checkbox "Segmenter" centrado en su celda
            auto *layoutCeldaSegment = new QHBoxLayout();
            layoutCeldaSegment->setContentsMargins(0,0,0,0);
            auto *chkSegmentador = new QCheckBox();
            chkSegmentador->setChecked(operators[i].esSegmentador);
            layoutCeldaSegment->addStretch();
            layoutCeldaSegment->addWidget(chkSegmentador);
            layoutCeldaSegment->addStretch();
            auto *wCeldaSegment = new QWidget();
            wCeldaSegment->setFixedWidth(anchoChk);
            wCeldaSegment->setLayout(layoutCeldaSegment);

            fila->addWidget(lblOp);
            fila->addWidget(wCeldaAcotable);
            fila->addWidget(wCeldaSegment);

            layoutLista->addLayout(fila);
        }

        auto *botones = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        QObject::connect(botones, &QDialogButtonBox::accepted, [=]() {
            Operator operadoresActualizados[max_OP] {};
            int count = 0;

            // Saltar índice 0 (encabezado layout) y 1 (separador widget)
            for (int i = 2; i < layoutLista->count() && count < max_OP; i++) {
                QLayoutItem *item = layoutLista->itemAt(i);
                if (!item) continue;
                QLayout *filaLayout = item->layout();
                if (!filaLayout || filaLayout->count() < 3) continue;

                // item 0: QLabel con nombre
                QLabel *lbl = qobject_cast<QLabel *>(filaLayout->itemAt(0)->widget());
                // item 1: QWidget contenedor de chkAcotable
                QWidget *wA = filaLayout->itemAt(1)->widget();
                // item 2: QWidget contenedor de chkSegmentador
                QWidget *wS = filaLayout->itemAt(2)->widget();

                if (!lbl || !wA || !wS) continue;

                QCheckBox *chkA = wA->findChild<QCheckBox *>();
                QCheckBox *chkS = wS->findChild<QCheckBox *>();

                if (!chkA || !chkS) continue;

                QByteArray bytes = lbl->text().toLocal8Bit();
                strncpy(operadoresActualizados[count].identificador, bytes.constData(), med_OP - 1);
                operadoresActualizados[count].identificador[med_OP - 1] = '\0';
                operadoresActualizados[count].esAcotable    = chkA->isChecked();
                operadoresActualizados[count].esSegmentador = chkS->isChecked();
                count++;
            }

            if (count < max_OP) operadoresActualizados[count].identificador[0] = '\0';

            actualizarListaDeOperadores(operadoresActualizados);
            dialogo->accept();
        });

        QObject::connect(botones, &QDialogButtonBox::rejected, dialogo, &QDialog::reject);

        auto *layoutBotonesOP = new QHBoxLayout();
        layoutBotonesOP->addStretch();
        layoutBotonesOP->addWidget(botones);
        layoutBotonesOP->addStretch();

        layoutDialogo->addWidget(scrollArea);
        layoutDialogo->addLayout(layoutBotonesOP);

        dialogo->exec();
    });

    // =========================
    // 🔸 FORMAT
    // =========================
    auto *grupoFormato = new QGroupBox("Format");
    auto *layoutFormato = new QVBoxLayout();

    // --- Adjust margin ---
    auto *layoutMargen = new QHBoxLayout();
    auto *chkMargen = new QCheckBox("Adjust to margin");
    auto *btnInfoMargen = new QPushButton("ℹ");
    btnInfoMargen->setFixedWidth(20);
    layoutMargen->addWidget(chkMargen);
    layoutMargen->addStretch();
    layoutMargen->addWidget(btnInfoMargen);

    // Suboption: margin limit (indentada con símbolo └)
    auto *layoutSubMargen = new QHBoxLayout();
    auto *lblArrowMargen = new QLabel("  └");
    lblArrowMargen->setFixedWidth(22);
    auto *inputMargen = new QLineEdit();
    layoutSubMargen->addWidget(lblArrowMargen);
    layoutSubMargen->addWidget(inputMargen);
    inputMargen->setVisible(false);
    lblArrowMargen->setVisible(false);

    QObject::connect(chkMargen, &QCheckBox::toggled, [=](bool checked) {
        inputMargen->setVisible(checked);
        lblArrowMargen->setVisible(checked);
    });

    QObject::connect(btnInfoMargen, &QPushButton::clicked, [=]() {
        QMessageBox::information(ventana, "Adjust Margin",
        "Limits line length.\nMargin limit defines max characters.");
    });

    // --- Sort ---
    auto *layoutSort = new QHBoxLayout();
    auto *chkOrdenar = new QCheckBox("Sort declarations");
    auto *btnInfoSort = new QPushButton("ℹ");
    btnInfoSort->setFixedWidth(20);
    layoutSort->addWidget(chkOrdenar);
    layoutSort->addStretch();
    layoutSort->addWidget(btnInfoSort);

    // Suboption: criterios (indentada)
    auto *layoutSubSort = new QHBoxLayout();
    auto *lblArrowSort = new QLabel("  └");
    lblArrowSort->setFixedWidth(22);
    auto *comboCriterios = new QComboBox();
    layoutSubSort->addWidget(lblArrowSort);
    layoutSubSort->addWidget(comboCriterios);
    comboCriterios->setVisible(false);
    lblArrowSort->setVisible(false);

    QString letras = "AD";
    for (auto a : letras)
        for (auto b : letras)
            for (auto c : letras)
                comboCriterios->addItem(QString("%1%2%3").arg(a).arg(b).arg(c));

    QObject::connect(chkOrdenar, &QCheckBox::toggled, [=](bool checked) {
        comboCriterios->setVisible(checked);
        lblArrowSort->setVisible(checked);
    });

    QObject::connect(btnInfoSort, &QPushButton::clicked, [=]() {
        QMessageBox::information(ventana, "Sort Declarations",
        "Orders declarations.\nCriteria defines sorting priority.");
    });

    // --- Space subelements ---
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

    // --- Assignments ---
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

    // --- Functions ---
    auto *layoutFunc = new QHBoxLayout();
    auto *chkFunciones = new QCheckBox("Process functions");
    auto *btnInfoFunc = new QPushButton("ℹ");
    btnInfoFunc->setFixedWidth(20);
    layoutFunc->addWidget(chkFunciones);
    layoutFunc->addStretch();
    layoutFunc->addWidget(btnInfoFunc);

    // Suboption: suppress variables (indentada)
    auto *layoutSubSuprimir = new QHBoxLayout();
    auto *lblArrowSuprimir = new QLabel("  └");
    lblArrowSuprimir->setFixedWidth(22);
    auto *chkSuprimir = new QCheckBox("Suppress variables");
    layoutSubSuprimir->addWidget(lblArrowSuprimir);
    layoutSubSuprimir->addWidget(chkSuprimir);
    chkSuprimir->setVisible(false);
    lblArrowSuprimir->setVisible(false);

    // Suboption: delimiter (indentada)
    auto *layoutSubDelim = new QHBoxLayout();
    auto *lblArrowDelim = new QLabel("  └");
    lblArrowDelim->setFixedWidth(22);
    auto *comboDelimitador = new QComboBox();
    comboDelimitador->addItems({";", "{"});
    layoutSubDelim->addWidget(lblArrowDelim);
    layoutSubDelim->addWidget(comboDelimitador);
    comboDelimitador->setVisible(false);
    lblArrowDelim->setVisible(false);

    QObject::connect(chkFunciones, &QCheckBox::toggled, [=](bool checked) {
        chkSuprimir->setVisible(checked);
        lblArrowSuprimir->setVisible(checked);
        comboDelimitador->setVisible(checked);
        lblArrowDelim->setVisible(checked);
    });

    QObject::connect(btnInfoFunc, &QPushButton::clicked, [=]() {
        QMessageBox::information(ventana, "Process Functions",
        "Processes functions.\nSuppress hides parameter names.\nDelimiter defines output symbol.");
    });

    layoutFormato->addLayout(layoutMargen);
    layoutFormato->addLayout(layoutSubMargen);
    layoutFormato->addLayout(layoutSort);
    layoutFormato->addLayout(layoutSubSort);
    layoutFormato->addLayout(layoutEsp);
    layoutFormato->addLayout(layoutAsig);
    layoutFormato->addLayout(layoutFunc);
    layoutFormato->addLayout(layoutSubSuprimir);
    layoutFormato->addLayout(layoutSubDelim);

    grupoFormato->setLayout(layoutFormato);

    layoutIzquierdo->addWidget(grupoKeywords);
    layoutIzquierdo->addWidget(grupoOperadores);
    layoutIzquierdo->addWidget(grupoFormato);
    layoutIzquierdo->addStretch();

    // =========================
    // 🔹 DERECHA
    // =========================
    auto *layoutDerecho = new QVBoxLayout();

    // --- Input (con Convert adentro) ---
    auto *grupoInput = new QGroupBox("Input");
    auto *layoutInput = new QVBoxLayout();
    auto *textoEntrada = new QTextEdit();
    auto *botonConvertir = new QPushButton("Convert");
    layoutInput->addWidget(textoEntrada);
    layoutInput->addWidget(botonConvertir);
    grupoInput->setLayout(layoutInput);

    // cargar input
    {
        ifstream src = abrirArchivo_IFS("../resources/CodeSource.txt");
        string contenido((istreambuf_iterator<char>(src)), istreambuf_iterator<char>());
        textoEntrada->setText(QString::fromStdString(contenido));
        src.close();
    }

    // --- Barra separadora centrada verticalmente ---
    auto *contenedorBarra = new QWidget();
    auto *layoutBarra = new QVBoxLayout(contenedorBarra);
    layoutBarra->setContentsMargins(0, 0, 0, 0);
    auto *barra = new QFrame();
    barra->setFrameShape(QFrame::HLine);
    barra->setFixedHeight(2);
    layoutBarra->addStretch();
    layoutBarra->addWidget(barra);
    layoutBarra->addStretch();
    contenedorBarra->setFixedHeight(30);

    // --- Output (con Copy adentro) ---
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
    lblArrowMargen->setVisible(pf.ajustarPorMargen);

    chkOrdenar->setChecked(pf.ordenarDeclaraciones);
    comboCriterios->setCurrentText(pf.criteriosDeOrdenamiento);
    comboCriterios->setVisible(pf.ordenarDeclaraciones);
    lblArrowSort->setVisible(pf.ordenarDeclaraciones);

    chkEspaciar->setChecked(pf.espaciarSubelementos);
    chkAsignaciones->setChecked(pf.procesarAsignaciones);

    chkFunciones->setChecked(pf.procesarFunciones);
    chkSuprimir->setChecked(pf.suprimirVariables);
    comboDelimitador->setCurrentText(QString(pf.simboloDelimitador));

    chkSuprimir->setVisible(pf.procesarFunciones);
    lblArrowSuprimir->setVisible(pf.procesarFunciones);
    comboDelimitador->setVisible(pf.procesarFunciones);
    lblArrowDelim->setVisible(pf.procesarFunciones);

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
    layoutDerecho->addWidget(contenedorBarra);
    layoutDerecho->addWidget(grupoOutput);

    // Columna izquierda ~15% más ancha que original: 260 -> 300
    auto *widgetIzquierdo = new QWidget();
    widgetIzquierdo->setLayout(layoutIzquierdo);
    widgetIzquierdo->setMaximumWidth(300);

    layoutPrincipal->addWidget(widgetIzquierdo);
    layoutPrincipal->addLayout(layoutDerecho, 1);

    ventana->setLayout(layoutPrincipal);
    ventana->show();
}