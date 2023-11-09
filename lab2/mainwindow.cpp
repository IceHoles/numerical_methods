#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(1200, 800);

    topLeftWidget = new QWidget(this);
    topRightWidget = new QWidget(this);
    bottomLeftWidget = new QWidget(this);
    bottomRightWidget = new QWidget(this);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(topLeftWidget, 0, 0);
    gridLayout->addWidget(topRightWidget, 0, 1);
    gridLayout->addWidget(bottomLeftWidget, 1, 0);
    gridLayout->addWidget(bottomRightWidget, 1, 1);
    topLeftWidget->setFixedSize(310, 270);
    bottomLeftWidget->setFixedSize(310, 530);
    centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    QLabel* label1 = new QLabel("Число разбиений сетки:", topLeftWidget);

    grid = new QLineEdit(topLeftWidget);
    grid->setText("10");
    grid->setFixedWidth(150);

    QPushButton* taskButton1 = new QPushButton("Тестовая задача", this);
    QPushButton* taskButton2 = new QPushButton("Основная задача", this);
    QPushButton* clearPlotButton = new QPushButton("Очистить график", this);

    summary = new QTextEdit();
    summary->setPlainText("Справка");
    summary->setReadOnly(true);

    QGridLayout* topLeftLayout = new QGridLayout;

    topLeftLayout->addWidget(taskButton1, 0, 0);
    topLeftLayout->addWidget(taskButton2, 0, 1);
    topLeftLayout->addWidget(label1, 1, 0);
    topLeftLayout->addWidget(grid, 1, 1);
    topLeftLayout->addWidget(clearPlotButton, 2, 0);
    topLeftLayout->addWidget(summary, 3, 0, 1, 2);
    topLeftWidget->setLayout(topLeftLayout);

    tabWidget = new QTabWidget(bottomLeftWidget);
    QVBoxLayout* bottomLeftLayout = new QVBoxLayout(bottomLeftWidget);
    bottomLeftLayout->addWidget(tabWidget);
    bottomLeftWidget->setLayout(bottomLeftLayout);

    QVBoxLayout* topRightLayout = new QVBoxLayout(topRightWidget);
    pchart = new QChart();
    pchart->createDefaultAxes();
    pchart->setAnimationOptions(QChart::AllAnimations);
    paxisX = new QValueAxis;
    paxisX->setTitleText("Точность");
    pchart->addAxis(paxisX, Qt::AlignBottom);
    paxisX->setGridLineVisible(true);

    paxisY = new QValueAxis;
    paxisY->setTitleText("X");
    pchart->addAxis(paxisY, Qt::AlignLeft);
    paxisY->setGridLineVisible(true);

    paxisX->setGridLineVisible(true);
    paxisY->setGridLineVisible(true);

    pchart->setMargins(QMargins(0, 0, 0, 0));

    pchartView = new QChartView(pchart);
    pchartView->setRenderHint(QPainter::Antialiasing, true);

    topRightLayout->addWidget(pchartView);
    topRightWidget->setLayout(topRightLayout);

    chart = new QChart();
    chart->createDefaultAxes();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    axisX = new QValueAxis;
    axisX->setTitleText("X");
    chart->addAxis(axisX, Qt::AlignBottom);
    axisX->setGridLineVisible(true);

    axisY = new QValueAxis;
    axisY->setTitleText("V");
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setGridLineVisible(true);

    chart->setMargins(QMargins(0, 0, 0, 0));

    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(true);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->setInteractive(true);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setDragMode(QGraphicsView::ScrollHandDrag);
    chartView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    QVBoxLayout* bottomRightLayout = new QVBoxLayout(bottomRightWidget);
    bottomRightLayout->addWidget(chartView);
    bottomRightWidget->setLayout(bottomRightLayout);

    connect(taskButton1, &QPushButton::clicked, this, &MainWindow::onTask1Clicked);
    connect(taskButton2, &QPushButton::clicked, this, &MainWindow::onTask2Clicked);
    connect(clearPlotButton, &QPushButton::clicked, this, &MainWindow::onClearPlotButtonClicked);
}

MainWindow::~MainWindow()
{
}



void MainWindow::onTask1Clicked() {
    n = grid->text().toInt();
    QLineSeries* series = new QLineSeries();
    QLineSeries* seriesve = new QLineSeries();
    QLineSeries* pseries = new QLineSeries();
    tableWidget = new QTableWidget(tabWidget);
    QHeaderView* header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(false);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("v"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("u"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("v-u"));

    Functions::set_task("test");

    double ae1 = 0;
    double mu1 = 0;
    double ae2 = 0;
    double mu2 = 1;
    double x_0 = 0;
    double x_n = 1;
    double eps = 10e-6;
    std::vector<double> epss;
    std::vector<double> y(n);
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> phi;

    buildLES(n, x_0, x_n, a, b, c, phi);
    run(ae1, mu1, ae2, mu2, x_0, x_n, a, b, c, phi, y);

    a.clear();
    b.clear();
    c.clear();
    phi.clear();
    double h = (1.0 / (double)(n - 1));
    for (int i = 0; i < n; i++) {
        double x_i = x_0 + i * h;
        double u = Functions::tr_f(x_i);
        epss.push_back(std::abs(y[i] - u));
    }
    for (int i = 0; i < y.size(); i++) {
        double x_i = x_0 + i * h;
        double u = Functions::tr_f(x_i);
        series->append(x_i, y[i]);
        seriesve->append(x_i, u);
        pseries->append(x_i, std::abs(y[i] - u));
    }
    for(int i = 1; i < y.size(); i++){
        double x_i = x_0 + i * (1.0 / (double)(n - 1));
        double u = Functions::tr_f(x_i);
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(x_i)));
        tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(y[i])));
        tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(u)));
        tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(std::abs(y[i] - u))));
    }
    auto max = std::max_element(epss.begin(), epss.end());
    epsilon1 = *max;
    xerr = x_0 + std::distance(epss.begin(), max) * (1.0 / (double)n);
    pseries->setName("Погрешность " + QString::number(tabCounter));
    pchart->addSeries(pseries);
    pchart->createDefaultAxes();
    pchart->zoomReset();
    series->setName("Тестовая" + QString::number(tabCounter));
    seriesve->setName("Тестовая истинная" + QString::number(tabCounter));
    chart->addSeries(seriesve);
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->zoomReset();
    tabWidget->addTab(tableWidget, "Тестовая задача " + QString::number(tabCounter));
    tabCounter++;

    QString message = QString("Справка:  \n"
        "Для решения задачи использована равномерная сетка с числом разбиений n = %1 \n"
        "Задача должна быть решена с погрешностью не более ε = 10e-6 \n"
        "Задача решена с погрешностью ε1 = %2 \n"
        "Максимальное отклонение аналитического и численного решений наблюдается в точке x = %3 \n")
        .arg(n)
        .arg(epsilon1)
        .arg(xerr);
    summary->setPlainText(message);
    summary->setReadOnly(true);
}

void  MainWindow::onTask2Clicked() {
    n = grid->text().toInt();
    QLineSeries* series = new QLineSeries();
    QLineSeries* seriesve = new QLineSeries();
    QLineSeries* pseries = new QLineSeries();
    tableWidget = new QTableWidget(tabWidget);
    QHeaderView* header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->setSortingEnabled(false);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("x"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("v"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("v2"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("v-v2"));

    Functions::set_task("main");

    double ae1 = 0;
    double mu1 = 0;
    double ae2 = 0;
    double mu2 = 1;
    double x_0 = 0;
    double x_n = 1;
    double eps = 10e-6;
    std::vector<double> epss;
    std::vector<double> y(n);
    std::vector<double> y2(n * 2 - 1);
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> phi;

    buildLES(n, x_0, x_n, a, b, c, phi);
    run(ae1, mu1, ae2, mu2, x_0, x_n, a, b, c, phi, y);

    a.clear();
    b.clear();
    c.clear();
    phi.clear();
    
    buildLES(n * 2 - 1, x_0, x_n, a, b, c, phi);
    run(ae1, mu1, ae2, mu2, x_0, x_n, a, b, c, phi, y2);

    double h1 = 1.0 / (double)(n - 1);
    for (int i = 0; i < y.size(); i++) {
        epss.push_back(std::abs(y[i] - y2[2 * i]));
    }
    for (int i = 0; i < y.size(); i++) {
        double x_i = x_0 + i * h1;
        series->append(x_i, y[i]);
        seriesve->append(x_i, y2[2 * i]);
        pseries->append(x_i, std::abs(y[i] - y2[2 * i]));
    }
    for(int i = 1; i < y.size(); i++){
        double x_i = x_0 + i * h1;
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setItem(tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(x_i)));
        tableWidget->setItem(tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(y[i])));
        tableWidget->setItem(tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(y2[2 * i])));
        tableWidget->setItem(tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(std::abs(y[i] - y2[2 * i]))));
    }
    auto max = std::max_element(epss.begin(), epss.end());
    epsilon1 = *max;
    xerr = x_0 + std::distance(epss.begin(), max) * h1;
    pseries->setName("Погрешность " + QString::number(tabCounter));
    pchart->addSeries(pseries);
    pchart->createDefaultAxes();
    pchart->zoomReset();
    series->setName("Численная" + QString::number(tabCounter));
    seriesve->setName("Численная двойная" + QString::number(tabCounter));
    chart->addSeries(seriesve);
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->zoomReset();
    tabWidget->addTab(tableWidget, "Основная задача " + QString::number(tabCounter));
    tabCounter++;

    QString message = QString("Справка:  \n"
        "Для решения задачи использована равномерная сетка с числом разбиений n = %1 \n"
        "Задача должна быть решена с заданной точностью ε = 10e-6 \n"
        "Задача решена с точностью ε2 = %2 \n"
        "Максимальная разность численных решений в общих узлах сетки наблюдается в точке x = %3 \n")
        .arg(n)
        .arg(epsilon1)
        .arg(xerr);
    summary->setPlainText(message);
    summary->setReadOnly(true);
}

void  MainWindow::onClearPlotButtonClicked() {
    chart->removeAllSeries();
    chart->createDefaultAxes();
    pchart->removeAllSeries();
    pchart->createDefaultAxes();
    summary->setPlainText("Справка");
    tabWidget->clear();
}
