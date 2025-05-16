#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <memory>

#ifndef TRACKH
#define TRACKH
// Базовый класс контрольного пункта
class ControlPoint
{
public:
    ControlPoint(const std::string& name, double latitude, double longitude)
        : name(name), latitude(latitude), longitude(longitude) {}

    virtual ~ControlPoint() = default;

    std::string getName() const { return name; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    virtual bool isOptional() const = 0;
    virtual double getPenalty() const = 0;

private:
    std::string name;
    double latitude;
    double longitude;
};

// Обязательный контрольный пункт
class MandatoryControlPoint : public ControlPoint
{
public:
    MandatoryControlPoint(const std::string& name, double latitude, double longitude)
        : ControlPoint(name, latitude, longitude) {}

    bool isOptional() const override { return false; }
    double getPenalty() const override { return 0.0; }
};

// Необязательный контрольный пункт
class OptionalControlPoint : public ControlPoint
{
public:
    OptionalControlPoint(const std::string& name, double latitude, double longitude, double penalty)
        : ControlPoint(name, latitude, longitude), penalty(penalty) {}

    bool isOptional() const override { return true; }
    double getPenalty() const override { return penalty; }

private:
    double penalty;
};

// Базовый класс Builder
class ControlPointBuilder
{
public:
    virtual ~ControlPointBuilder() = default;
    virtual void addControlPoint(const ControlPoint &cp) = 0;
    virtual void reset() = 0;
};

// Builder для текстового вывода
class TextReportBuilder : public ControlPointBuilder
{
public:
    void addControlPoint(const ControlPoint &cp) override
    {
        std::ostringstream ss;
        ss << ++counter << ". " << cp.getName()
           << " (" << std::fixed << std::setprecision(6)
           << cp.getLatitude() << ", " << cp.getLongitude() << ") - ";

        if (cp.isOptional())
        {
            ss << "Fine: " << cp.getPenalty() << " h";
        }
        else
        {
            ss << "failure of SU";
        }
        report += ss.str() + "\n";
    }

    void reset() override
    {
        counter = 0;
        report.clear();
    }

    std::string getReport() const { return report; }

private:
    int counter = 0;
    std::string report;
};

// Builder для подсчета штрафов
class PenaltyCalculatorBuilder : public ControlPointBuilder
{
public:
    void addControlPoint(const ControlPoint &cp) override
    {
        if (cp.isOptional())
        {
            totalPenalty += cp.getPenalty();
        }
    }

    void reset() override
    {
        totalPenalty = 0.0;
    }

    double getTotalPenalty() const { return totalPenalty; }

private:
    double totalPenalty = 0.0;
};

// Класс-распорядитель
class TrackDirector
{
public:
    void buildReport(const std::vector<std::unique_ptr<ControlPoint>> &points,
                     ControlPointBuilder &builder)
    {
        builder.reset();
        for (auto &cp : points)
        {
            builder.addControlPoint(*cp);
        }
    }
};

#endif // TRACKH