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

void initGUI() {
    QWidget *ventana = new QWidget();
    ventana->setWindowTitle("HeaderConverter");
    ventana->resize(1000, 600);

    auto *layoutPrincipal = new QHBoxLayout();

    // =========================
    // 🔹 HELPER: MODAL CUSTOM
    // Sin icono, texto alineado a la izquierda, botón OK centrado
    // =========================
    auto mostrarModal = [=](QWidget *parent, const QString &titulo, const QString &texto) {
        QDialog *modal = new QDialog(parent);
        modal->setWindowTitle(titulo);
        modal->setFixedWidth(340);
        auto *layout = new QVBoxLayout(modal);
        layout->setSpacing(16);
        layout->setContentsMargins(18, 18, 18, 14);
        auto *lbl = new QLabel(texto);
        lbl->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        lbl->setWordWrap(true);
        lbl->setTextFormat(Qt::PlainText);
        layout->addWidget(lbl);
        auto *btn = new QPushButton("OK");
        btn->setFixedWidth(80);
        auto *layoutBtn = new QHBoxLayout();
        layoutBtn->addStretch();
        layoutBtn->addWidget(btn);
        layoutBtn->addStretch();
        layout->addLayout(layoutBtn);
        QObject::connect(btn, &QPushButton::clicked, modal, &QDialog::accept);
        modal->exec();
    };

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
        dialogo->setFixedWidth(220);
        dialogo->resize(220, 450);

        auto *layoutDialogo = new QVBoxLayout(dialogo);

        auto *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);

        auto *contenedor = new QWidget();
        auto *layoutLista = new QVBoxLayout(contenedor);
        layoutLista->setAlignment(Qt::AlignTop);
        scrollArea->setWidget(contenedor);

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
            layoutLista->addLayout(fila);
        }

        auto *btnAgregar = new QPushButton("+ Add keyword");
        QObject::connect(btnAgregar, &QPushButton::clicked, [=]() {
            // Contar cuántas filas hay actualmente en la lista
            int count = 0;
            for (int i = 0; i < layoutLista->count(); i++) {
                QLayoutItem *item = layoutLista->itemAt(i);
                if (item && item->layout()) count++;
            }
            if (count >= max_KW) {
                mostrarModal(dialogo, "Warning",
                    QString("You cannot have more than %1 keywords defined at a time.").arg(max_KW));
                return;
            }

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

            actualizarArchivoDePalabrasClave(nuevasKeywords);
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
        dialogo->setFixedWidth(280);
        dialogo->resize(280, 450);

        auto *layoutDialogo = new QVBoxLayout(dialogo);

        auto *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        auto *contenedor = new QWidget();
        auto *layoutLista = new QVBoxLayout(contenedor);
        layoutLista->setAlignment(Qt::AlignTop);
        scrollArea->setWidget(contenedor);

        const int anchoOp  = 120;
        const int anchoChk = 90;

        // --- Encabezado ---
        auto *filaEncabezado = new QHBoxLayout();

        auto *layoutEncOp = new QHBoxLayout();
        layoutEncOp->setContentsMargins(0, 0, 0, 0);
        auto *lblNombre = new QLabel("Operator");
        lblNombre->setAlignment(Qt::AlignCenter);
        layoutEncOp->addStretch();
        layoutEncOp->addWidget(lblNombre);
        layoutEncOp->addStretch();
        auto *wEncOp = new QWidget();
        wEncOp->setFixedWidth(anchoOp);
        wEncOp->setLayout(layoutEncOp);

        auto *layoutEncSegment = new QHBoxLayout();
        layoutEncSegment->setContentsMargins(0, 0, 0, 0);
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
            mostrarModal(dialogo, "Information",
                "When active, the operator is spaced between its operands in the output. "
                "For example, if an operator is marked as a segmenter, it will appear surrounded "
                "by spaces when printed, separating it visually from the values on each side.");
        });

        filaEncabezado->addWidget(wEncOp);
        filaEncabezado->addWidget(wEncSegment);
        layoutLista->addLayout(filaEncabezado);

        auto *sep = new QFrame();
        sep->setFrameShape(QFrame::HLine);
        layoutLista->addWidget(sep);

        // --- Filas de operadores (operador centrado respecto a su columna) ---
        for (int i = 0; operators[i].identificador[0]; i++) {
            auto *fila = new QHBoxLayout();
            fila->setAlignment(Qt::AlignVCenter);

            // Celda del operador: centrado horizontalmente con stretch
            auto *layoutCeldaOp = new QHBoxLayout();
            layoutCeldaOp->setContentsMargins(0, 0, 0, 0);
            auto *lblOp = new QLabel(QString::fromLocal8Bit(operators[i].identificador));
            lblOp->setAlignment(Qt::AlignCenter);
            layoutCeldaOp->addStretch();
            layoutCeldaOp->addWidget(lblOp);
            layoutCeldaOp->addStretch();
            auto *wCeldaOp = new QWidget();
            wCeldaOp->setFixedWidth(anchoOp);
            wCeldaOp->setLayout(layoutCeldaOp);

            // Celda del checkbox segmentador
            auto *layoutCeldaSegment = new QHBoxLayout();
            layoutCeldaSegment->setContentsMargins(0, 0, 0, 0);
            auto *chkSegmentador = new QCheckBox();
            chkSegmentador->setChecked(operators[i].esSegmentador);
            layoutCeldaSegment->addStretch();
            layoutCeldaSegment->addWidget(chkSegmentador);
            layoutCeldaSegment->addStretch();
            auto *wCeldaSegment = new QWidget();
            wCeldaSegment->setFixedWidth(anchoChk);
            wCeldaSegment->setLayout(layoutCeldaSegment);

            fila->addWidget(wCeldaOp);
            fila->addWidget(wCeldaSegment);
            layoutLista->addLayout(fila);
        }

        auto *botones = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        QObject::connect(botones, &QDialogButtonBox::accepted, [=]() {
            Operator operadoresActualizados[max_OP] {};
            int count = 0;

            for (int i = 2; i < layoutLista->count() && count < max_OP; i++) {
                QLayoutItem *item = layoutLista->itemAt(i);
                if (!item) continue;
                QLayout *filaLayout = item->layout();
                if (!filaLayout || filaLayout->count() < 2) continue;

                QWidget *wOp = filaLayout->itemAt(0)->widget();
                QWidget *wS  = filaLayout->itemAt(1)->widget();
                if (!wOp || !wS) continue;
                QLabel    *lbl  = wOp->findChild<QLabel *>();
                QCheckBox *chkS = wS->findChild<QCheckBox *>();
                if (!lbl || !chkS) continue;

                QByteArray bytes = lbl->text().toLocal8Bit();
                strncpy(operadoresActualizados[count].identificador, bytes.constData(), med_OP - 1);
                operadoresActualizados[count].identificador[med_OP - 1] = '\0';
                operadoresActualizados[count].esAcotable    = operators[count].esAcotable;
                operadoresActualizados[count].esSegmentador = chkS->isChecked();
                count++;
            }

            if (count < max_OP) operadoresActualizados[count].identificador[0] = '\0';

            actualizarArchivoDeOperadores(operadoresActualizados);
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
    // Patron controlador:    [ℹ]  Nombre  [checkbox]
    // Patron subcontrolador: └  Nombre  [valor]
    // =========================
    auto *grupoFormato = new QGroupBox("Format");
    auto *layoutFormato = new QVBoxLayout();

    // --- Adjust to margin ---
    auto *layoutMargen = new QHBoxLayout();
    auto *btnInfoMargen = new QPushButton("ℹ");
    btnInfoMargen->setFixedWidth(20);
    auto *lblMargen = new QLabel("Adjust to margin");
    auto *chkMargen = new QCheckBox();
    layoutMargen->addWidget(btnInfoMargen);
    layoutMargen->addWidget(lblMargen);
    layoutMargen->addStretch();
    layoutMargen->addWidget(chkMargen);

    // Suboption: margin limit
    auto *layoutSubMargen = new QHBoxLayout();
    auto *lblArrowMargen = new QLabel("  └");
    lblArrowMargen->setFixedWidth(22);
    auto *lblMargenLimit = new QLabel("Margin limit");
    auto *inputMargen = new QLineEdit();
    inputMargen->setMaximumWidth(45);
    layoutSubMargen->addWidget(lblArrowMargen);
    layoutSubMargen->addWidget(lblMargenLimit);
    layoutSubMargen->addStretch();
    layoutSubMargen->addWidget(inputMargen);
    lblArrowMargen->setVisible(false);
    lblMargenLimit->setVisible(false);
    inputMargen->setVisible(false);

    QObject::connect(chkMargen, &QCheckBox::toggled, [=](bool checked) {
        lblArrowMargen->setVisible(checked);
        lblMargenLimit->setVisible(checked);
        inputMargen->setVisible(checked);
    });

    QObject::connect(btnInfoMargen, &QPushButton::clicked, [=]() {
        mostrarModal(ventana, "Information",
            "When active, statements wrap to the next line upon reaching the defined margin, "
            "aligning with the opening of the declaration. When inactive, statements continue "
            "on the same line regardless of their length.\n\n"
            "> Margin limit: maximum number of characters allowed per line before wrapping occurs.");
    });

    // --- Sort declarations ---
    auto *layoutSort = new QHBoxLayout();
    auto *btnInfoSort = new QPushButton("ℹ");
    btnInfoSort->setFixedWidth(20);
    auto *lblSort = new QLabel("Sort declarations");
    auto *chkOrdenar = new QCheckBox();
    layoutSort->addWidget(btnInfoSort);
    layoutSort->addWidget(lblSort);
    layoutSort->addStretch();
    layoutSort->addWidget(chkOrdenar);

    // Suboption: sort criteria → valor en QLineEdit no editable + botón "..."
    auto *layoutSubSort = new QHBoxLayout();
    auto *lblArrowSort = new QLabel("  └");
    lblArrowSort->setFixedWidth(22);
    auto *lblCriterios = new QLabel("Sort criteria");
    auto *inputCriteriosValor = new QLineEdit("AAA");
    inputCriteriosValor->setReadOnly(true);
    inputCriteriosValor->setMaximumWidth(40);
    inputCriteriosValor->setAlignment(Qt::AlignCenter);
    auto *btnEditCriterios = new QPushButton("...");
    btnEditCriterios->setFixedWidth(28);
    layoutSubSort->addWidget(lblArrowSort);
    layoutSubSort->addWidget(lblCriterios);
    layoutSubSort->addStretch();
    layoutSubSort->addWidget(inputCriteriosValor);
    layoutSubSort->addWidget(btnEditCriterios);
    lblArrowSort->setVisible(false);
    lblCriterios->setVisible(false);
    inputCriteriosValor->setVisible(false);
    btnEditCriterios->setVisible(false);

    QObject::connect(chkOrdenar, &QCheckBox::toggled, [=](bool checked) {
        lblArrowSort->setVisible(checked);
        lblCriterios->setVisible(checked);
        inputCriteriosValor->setVisible(checked);
        btnEditCriterios->setVisible(checked);
    });

    QObject::connect(btnInfoSort, &QPushButton::clicked, [=]() {
        mostrarModal(ventana, "Information",
            "When active, declarations are printed sorted according to the defined criteria. "
            "When inactive, they are printed in the same order as in the source file.\n\n"
            "> Sort criteria: three characters defining the sorting criterion for each axis. "
            "The axes are evaluated in order: declaration type, keyword, and identifier. "
            "Each axis is independent and can be configured separately.");
    });

    // Diálogo de edición de criterios de ordenamiento
    QObject::connect(btnEditCriterios, &QPushButton::clicked, [=]() {
        QDialog *diagCriterios = new QDialog(ventana);
        diagCriterios->setWindowTitle("Sort Criteria");
        diagCriterios->setFixedWidth(360);

        auto *layoutDiag = new QVBoxLayout(diagCriterios);

        // Un único grid para cabecera + separador + filas → columnas perfectamente alineadas
        // Filas: 0=cabecera, 1=separador (span 4 cols), 2-4=ejes
        auto *grid = new QGridLayout();
        grid->setColumnStretch(0, 1);

        // --- Fila 0: cabecera ---
        auto *hdrSortBy = new QLabel("Sort by");
        hdrSortBy->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        grid->addWidget(hdrSortBy, 0, 0);

        // Columna A
        auto *layoutHdrA = new QHBoxLayout();
        auto *lblHdrA = new QLabel("A");
        lblHdrA->setAlignment(Qt::AlignCenter);
        auto *btnInfoA = new QPushButton("ℹ");
        btnInfoA->setFixedWidth(18);
        layoutHdrA->addStretch();
        layoutHdrA->addWidget(lblHdrA);
        layoutHdrA->addWidget(btnInfoA);
        layoutHdrA->addStretch();
        auto *wHdrA = new QWidget();
        wHdrA->setLayout(layoutHdrA);
        grid->addWidget(wHdrA, 0, 1);
        QObject::connect(btnInfoA, &QPushButton::clicked, [=]() {
            mostrarModal(diagCriterios, "Information",
                "Ascending order. Declarations are sorted from the lowest to the highest value "
                "on this axis, following alphabetical or type order depending on the axis.");
        });

        // Columna C
        auto *layoutHdrC = new QHBoxLayout();
        auto *lblHdrC = new QLabel("C");
        lblHdrC->setAlignment(Qt::AlignCenter);
        auto *btnInfoC = new QPushButton("ℹ");
        btnInfoC->setFixedWidth(18);
        layoutHdrC->addStretch();
        layoutHdrC->addWidget(lblHdrC);
        layoutHdrC->addWidget(btnInfoC);
        layoutHdrC->addStretch();
        auto *wHdrC = new QWidget();
        wHdrC->setLayout(layoutHdrC);
        grid->addWidget(wHdrC, 0, 2);
        QObject::connect(btnInfoC, &QPushButton::clicked, [=]() {
            mostrarModal(diagCriterios, "Information",
                "Consecutive order. This axis is ignored and the evaluation moves on to the next one. "
                "Declarations that share the same value on the previous axes maintain their relative "
                "order from the source file.");
        });

        // Columna D
        auto *layoutHdrD = new QHBoxLayout();
        auto *lblHdrD = new QLabel("D");
        lblHdrD->setAlignment(Qt::AlignCenter);
        auto *btnInfoD = new QPushButton("ℹ");
        btnInfoD->setFixedWidth(18);
        layoutHdrD->addStretch();
        layoutHdrD->addWidget(lblHdrD);
        layoutHdrD->addWidget(btnInfoD);
        layoutHdrD->addStretch();
        auto *wHdrD = new QWidget();
        wHdrD->setLayout(layoutHdrD);
        grid->addWidget(wHdrD, 0, 3);
        QObject::connect(btnInfoD, &QPushButton::clicked, [=]() {
            mostrarModal(diagCriterios, "Information",
                "Descending order. Declarations are sorted from the highest to the lowest value "
                "on this axis, following reverse alphabetical or type order depending on the axis.");
        });

        // --- Fila 1: separador que ocupa todas las columnas ---
        auto *sepGrid = new QFrame();
        sepGrid->setFrameShape(QFrame::HLine);
        grid->addWidget(sepGrid, 1, 0, 1, 4);

        // --- Filas 2-4: ejes (Type, Keyword, Identifier) ---
        const char *nombresEjes[3] = {"Type", "Keyword", "Identifier"};
        QRadioButton *rbGroups[3][3];
        QButtonGroup *btnGroups[3];
        QString criterioActual = inputCriteriosValor->text();

        for (int eje = 0; eje < 3; eje++) {
            auto *lblEje = new QLabel(nombresEjes[eje]);
            lblEje->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            grid->addWidget(lblEje, eje + 2, 0);

            btnGroups[eje] = new QButtonGroup(diagCriterios);

            auto *rbA = new QRadioButton(); rbGroups[eje][0] = rbA;
            auto *wCeldaA = new QWidget();
            auto *lA = new QHBoxLayout(wCeldaA);
            lA->setContentsMargins(0,0,0,0); lA->addStretch(); lA->addWidget(rbA); lA->addStretch();
            grid->addWidget(wCeldaA, eje + 2, 1);

            auto *rbC = new QRadioButton(); rbGroups[eje][1] = rbC;
            auto *wCeldaC = new QWidget();
            auto *lC = new QHBoxLayout(wCeldaC);
            lC->setContentsMargins(0,0,0,0); lC->addStretch(); lC->addWidget(rbC); lC->addStretch();
            grid->addWidget(wCeldaC, eje + 2, 2);

            auto *rbD = new QRadioButton(); rbGroups[eje][2] = rbD;
            auto *wCeldaD = new QWidget();
            auto *lD = new QHBoxLayout(wCeldaD);
            lD->setContentsMargins(0,0,0,0); lD->addStretch(); lD->addWidget(rbD); lD->addStretch();
            grid->addWidget(wCeldaD, eje + 2, 3);

            btnGroups[eje]->addButton(rbA, 0);
            btnGroups[eje]->addButton(rbC, 1);
            btnGroups[eje]->addButton(rbD, 2);

            char valorEje = (eje < criterioActual.length()) ? criterioActual[eje].toLatin1() : 'A';
            if      (valorEje == 'A') rbA->setChecked(true);
            else if (valorEje == 'C') rbC->setChecked(true);
            else if (valorEje == 'D') rbD->setChecked(true);
            else                       rbA->setChecked(true);
        }

        layoutDiag->addLayout(grid);

        // Botones Ok/Cancel centrados
        auto *botones = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        auto *layoutBotonesCrit = new QHBoxLayout();
        layoutBotonesCrit->addStretch();
        layoutBotonesCrit->addWidget(botones);
        layoutBotonesCrit->addStretch();

        QObject::connect(botones, &QDialogButtonBox::accepted, [=]() {
            QString nuevoCriterio = "";
            const char opciones[3] = {'A', 'C', 'D'};
            for (int eje = 0; eje < 3; eje++) {
                int sel = btnGroups[eje]->checkedId();
                nuevoCriterio += (sel >= 0 && sel < 3) ? opciones[sel] : 'A';
            }
            inputCriteriosValor->setText(nuevoCriterio);
            diagCriterios->accept();
        });

        QObject::connect(botones, &QDialogButtonBox::rejected, diagCriterios, &QDialog::reject);

        layoutDiag->addLayout(layoutBotonesCrit);
        diagCriterios->exec();
    });

    // --- Space subelements ---
    auto *layoutEsp = new QHBoxLayout();
    auto *btnInfoEsp = new QPushButton("ℹ");
    btnInfoEsp->setFixedWidth(20);
    auto *lblEsp = new QLabel("Space subelements");
    auto *chkEspaciar = new QCheckBox();
    layoutEsp->addWidget(btnInfoEsp);
    layoutEsp->addWidget(lblEsp);
    layoutEsp->addStretch();
    layoutEsp->addWidget(chkEspaciar);

    QObject::connect(btnInfoEsp, &QPushButton::clicked, [=]() {
        mostrarModal(ventana, "Information",
            "When active, a space is added after each separator between the subelements of a "
            "declaration, except after the last one. This applies to function parameters and "
            "grouped assignment elements.");
    });

    // --- Process assignments ---
    auto *layoutAsig = new QHBoxLayout();
    auto *btnInfoAsig = new QPushButton("ℹ");
    btnInfoAsig->setFixedWidth(20);
    auto *lblAsig = new QLabel("Process assignments");
    auto *chkAsignaciones = new QCheckBox();
    layoutAsig->addWidget(btnInfoAsig);
    layoutAsig->addWidget(lblAsig);
    layoutAsig->addStretch();
    layoutAsig->addWidget(chkAsignaciones);

    QObject::connect(btnInfoAsig, &QPushButton::clicked, [=]() {
        mostrarModal(ventana, "Information",
            "When active, global assignments from the source file are processed and included in "
            "the output. When inactive, all assignments are ignored completely and will not appear "
            "in the result.");
    });

    // --- Process functions ---
    auto *layoutFunc = new QHBoxLayout();
    auto *btnInfoFunc = new QPushButton("ℹ");
    btnInfoFunc->setFixedWidth(20);
    auto *lblFunc = new QLabel("Process functions");
    auto *chkFunciones = new QCheckBox();
    layoutFunc->addWidget(btnInfoFunc);
    layoutFunc->addWidget(lblFunc);
    layoutFunc->addStretch();
    layoutFunc->addWidget(chkFunciones);

    // Suboption: suppress variables
    auto *layoutSubSuprimir = new QHBoxLayout();
    auto *lblArrowSuprimir = new QLabel("  └");
    lblArrowSuprimir->setFixedWidth(22);
    auto *lblSuprimir = new QLabel("Suppress variables");
    auto *chkSuprimir = new QCheckBox();
    layoutSubSuprimir->addWidget(lblArrowSuprimir);
    layoutSubSuprimir->addWidget(lblSuprimir);
    layoutSubSuprimir->addStretch();
    layoutSubSuprimir->addWidget(chkSuprimir);
    lblArrowSuprimir->setVisible(false);
    lblSuprimir->setVisible(false);
    chkSuprimir->setVisible(false);

    // Suboption: delimiter
    auto *layoutSubDelim = new QHBoxLayout();
    auto *lblArrowDelim = new QLabel("  └");
    lblArrowDelim->setFixedWidth(22);
    auto *lblDelimitador = new QLabel("Delimiter");
    auto *comboDelimitador = new QComboBox();
    comboDelimitador->addItems({";", "{"});
    comboDelimitador->setMaximumWidth(70);
    layoutSubDelim->addWidget(lblArrowDelim);
    layoutSubDelim->addWidget(lblDelimitador);
    layoutSubDelim->addStretch();
    layoutSubDelim->addWidget(comboDelimitador);
    lblArrowDelim->setVisible(false);
    lblDelimitador->setVisible(false);
    comboDelimitador->setVisible(false);

    QObject::connect(chkFunciones, &QCheckBox::toggled, [=](bool checked) {
        lblArrowSuprimir->setVisible(checked);
        lblSuprimir->setVisible(checked);
        chkSuprimir->setVisible(checked);
        lblArrowDelim->setVisible(checked);
        lblDelimitador->setVisible(checked);
        comboDelimitador->setVisible(checked);
    });

    QObject::connect(btnInfoFunc, &QPushButton::clicked, [=]() {
        mostrarModal(ventana, "Information",
            "When active, functions from the source file are processed and included in the output. "
            "When inactive, all functions are ignored completely.\n\n"
            "> Suppress variables: when active, the parameter identifiers (variable names) are "
            "omitted from the output, keeping only their types.\n\n"
            "> Delimiter: symbol used to close each function in the output. "
            "Use ';' for header files and '{' for source files.");
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

    // --- Input ---
    auto *grupoInput = new QGroupBox("Input");
    auto *layoutInput = new QVBoxLayout();
    auto *textoEntrada = new QTextEdit();
    auto *botonConvertir = new QPushButton("Convert");
    layoutInput->addWidget(textoEntrada);
    layoutInput->addWidget(botonConvertir);
    grupoInput->setLayout(layoutInput);

    {
        ifstream src = abrirArchivo_IFS("../resources/Source.txt");
        string contenido((istreambuf_iterator<char>(src)), istreambuf_iterator<char>());
        textoEntrada->setText(QString::fromStdString(contenido));
        src.close();
    }

    // --- Barra separadora ---
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

    // --- Output ---
    auto *grupoOutput = new QGroupBox("Output");
    auto *layoutOutput = new QVBoxLayout();
    auto *textoSalida = new QTextEdit();
    textoSalida->setReadOnly(true);
    auto *btnCopiar = new QPushButton("Copy");
    layoutOutput->addWidget(textoSalida);
    layoutOutput->addWidget(btnCopiar);
    grupoOutput->setLayout(layoutOutput);

    {
        ifstream res = abrirArchivo_IFS("../resources/Conversion.txt");
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
    lblArrowMargen->setVisible(pf.ajustarPorMargen);
    lblMargenLimit->setVisible(pf.ajustarPorMargen);
    inputMargen->setVisible(pf.ajustarPorMargen);

    chkOrdenar->setChecked(pf.ordenarDeclaraciones);
    inputCriteriosValor->setText(pf.criteriosDeOrdenamiento);
    lblArrowSort->setVisible(pf.ordenarDeclaraciones);
    lblCriterios->setVisible(pf.ordenarDeclaraciones);
    inputCriteriosValor->setVisible(pf.ordenarDeclaraciones);
    btnEditCriterios->setVisible(pf.ordenarDeclaraciones);

    chkEspaciar->setChecked(pf.espaciarSubelementos);
    chkAsignaciones->setChecked(pf.procesarAsignaciones);

    chkFunciones->setChecked(pf.procesarFunciones);
    chkSuprimir->setChecked(pf.suprimirVariables);
    comboDelimitador->setCurrentText(QString(pf.simboloDelimitador));

    lblArrowSuprimir->setVisible(pf.procesarFunciones);
    lblSuprimir->setVisible(pf.procesarFunciones);
    chkSuprimir->setVisible(pf.procesarFunciones);
    lblArrowDelim->setVisible(pf.procesarFunciones);
    lblDelimitador->setVisible(pf.procesarFunciones);
    comboDelimitador->setVisible(pf.procesarFunciones);

    // =========================
    // 🔹 BOTÓN CONVERTIR
    // =========================
    QObject::connect(botonConvertir, &QPushButton::clicked, [=]() {

        ofstream src = abrirArchivo_OFS("../resources/Source.txt");
        src << textoEntrada->toPlainText().toStdString();
        src.close();

        ProcessingFormat nuevoPF;
        nuevoPF.ajustarPorMargen      = chkMargen->isChecked();
        nuevoPF.limitePorMargen       = inputMargen->text().toInt();
        nuevoPF.ordenarDeclaraciones  = chkOrdenar->isChecked();
        strcpy(nuevoPF.criteriosDeOrdenamiento, inputCriteriosValor->text().toStdString().c_str());
        nuevoPF.espaciarSubelementos  = chkEspaciar->isChecked();
        nuevoPF.procesarAsignaciones  = chkAsignaciones->isChecked();
        nuevoPF.procesarFunciones     = chkFunciones->isChecked();
        nuevoPF.suprimirVariables     = chkSuprimir->isChecked();
        nuevoPF.simboloDelimitador    = comboDelimitador->currentText().toStdString()[0];

        actualizarArchivoDeFormatoDeProcesamiento(nuevoPF);
        cargarFormatoDeProcesamiento();
        cargarListaDePalabrasClave();
        cargarListaDeOperadores();

        headerConversion();

        textoSalida->clear();

        ifstream res = abrirArchivo_IFS("../resources/Conversion.txt");
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

    auto *widgetIzquierdo = new QWidget();
    widgetIzquierdo->setLayout(layoutIzquierdo);
    widgetIzquierdo->setMaximumWidth(320);

    layoutPrincipal->addWidget(widgetIzquierdo);
    layoutPrincipal->addLayout(layoutDerecho, 1);

    ventana->setLayout(layoutPrincipal);
    ventana->show();
}