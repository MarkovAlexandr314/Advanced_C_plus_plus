#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "track.h"

int main()
{
    // Создание тестовых контрольных пунктов
    std::vector<std::unique_ptr<ControlPoint>> points;
    points.emplace_back(new MandatoryControlPoint("Start", 55.755833, 37.617222));
    points.emplace_back(new OptionalControlPoint("Mountain pass", 43.356667, 42.449444, 2.5));
    points.emplace_back(new MandatoryControlPoint("Finish", 59.934280, 30.335098));
    points.emplace_back(new OptionalControlPoint("River crossing", 52.371667, 39.823333, 1.2));

    TrackDirector director;

    // Генерация текстового отчета
    TextReportBuilder textBuilder;
    director.buildReport(points, textBuilder);
    std::cout << "Text report:\n"
              << textBuilder.getReport() << "\n";

    // Расчет суммарного штрафа
    PenaltyCalculatorBuilder penaltyBuilder;
    director.buildReport(points, penaltyBuilder);
    std::cout << "Total fine: " << penaltyBuilder.getTotalPenalty() << " h\n";

    return 0;
}