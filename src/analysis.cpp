#include "MaxSizeDeque.h"
#include "VariableSizeDeque.h"
#include "CLI11.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
struct SAnalysisStats
{
    // Use Welfords's Algorithm
    std::size_t DCount;
    double DMean;
    double DMeanDifferenceSquared;
    SAnalysisStats();
    SAnalysisStats(const SAnalysisStats &stats) = default;
    void AddSample(double sample);
    std::size_t Count() const;
    double Mean() const;
    double Variance() const;
    double StandardDeviation() const;
    static SAnalysisStats PushFrontTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations);
    static SAnalysisStats PushBackTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations);
    static SAnalysisStats PopFrontTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations);
    static SAnalysisStats PopBackTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations);
    static SAnalysisStats ForwardTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations);
    static SAnalysisStats ReverseTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations);
    static void OutputTotal(std::ostream &out, const std::vector<SAnalysisStats> &stats);
    static void OutputMean(std::ostream &out, const std::vector<SAnalysisStats> &stats);
    static void OutputStandardDeviation(std::ostream &out, const std::vector<SAnalysisStats> &stats);
};

constexpr std::string_view PushFrontLabel = "PushFront";
constexpr std::string_view PushBackLabel = "PushBack";
constexpr std::string_view PopFrontLabel = "PopFront";
constexpr std::string_view PopBackLabel = "PopBack";
constexpr std::string_view ForwardLabel = "Forward";
constexpr std::string_view ReverseLabel = "Reverse";
constexpr std::string_view MaxSizeLabel = "Max";
constexpr std::string_view VariableSizeLabel = "Var";

int main(int argc, char *argv[])
{
    CLI::App CLIApp{"Deque Implementation Analysis"};
    std::size_t MaxSize;
    std::vector<std::size_t> Iterations;
    argv = CLIApp.ensure_utf8(argv);
    CLIApp.add_option("-m,--max", MaxSize, "Max queue size.")->required();
    CLIApp.add_option("-i,--iter", Iterations, "Iterations to execute each.")->required()->expected(1, 8);
    CLI11_PARSE(CLIApp, argc, argv);
    std::vector<SAnalysisStats> PushFrontMax, PushFrontVariable;
    std::vector<SAnalysisStats> PushBackMax, PushBackVariable;
    std::vector<SAnalysisStats> PopFrontMax, PopFrontVariable;
    std::vector<SAnalysisStats> PopBackMax, PopBackVariable;
    std::vector<SAnalysisStats> ForwardMax, ForwardVariable;
    std::vector<SAnalysisStats> ReversedMax, ReverseVariable;
    for (auto Iteration : Iterations)
    {
        auto MaxSizeDeque = std::make_shared<CMaxSizeDeque>(MaxSize);
        auto VariableSizeDeque = std::make_shared<CVariableSizeDeque>();
        PushFrontMax.push_back(SAnalysisStats::PushFrontTest(MaxSizeDeque, MaxSize, Iteration));
        PushFrontVariable.push_back(SAnalysisStats::PushFrontTest(VariableSizeDeque, MaxSize, Iteration));
        PushBackMax.push_back(SAnalysisStats::PushBackTest(MaxSizeDeque, MaxSize, Iteration));
        PushBackVariable.push_back(SAnalysisStats::PushBackTest(VariableSizeDeque, MaxSize, Iteration));
        PopFrontMax.push_back(SAnalysisStats::PopFrontTest(MaxSizeDeque, MaxSize, Iteration));
        PopFrontVariable.push_back(SAnalysisStats::PopFrontTest(VariableSizeDeque, MaxSize, Iteration));
        PopBackMax.push_back(SAnalysisStats::PopBackTest(MaxSizeDeque, MaxSize, Iteration));
        PopBackVariable.push_back(SAnalysisStats::PopBackTest(VariableSizeDeque, MaxSize, Iteration));
        ForwardMax.push_back(SAnalysisStats::ForwardTest(MaxSizeDeque, MaxSize, Iteration));
        ForwardVariable.push_back(SAnalysisStats::ForwardTest(VariableSizeDeque, MaxSize, Iteration));
        ReversedMax.push_back(SAnalysisStats::ReverseTest(MaxSizeDeque, MaxSize, Iteration));
        ReverseVariable.push_back(SAnalysisStats::ReverseTest(VariableSizeDeque, MaxSize, Iteration));
    }
    std::ofstream OutFile("results.csv");
    OutFile << "Total," << MaxSizeLabel << std::string(Iterations.size(), ',') << VariableSizeLabel << std::endl;
    for (std::size_t Index = 0; Index < 2; Index++)
    {
        for (auto Iteration : Iterations)
        {
            OutFile << "," << Iteration;
        }
    }
    OutFile << std::endl;
    OutFile << PushFrontLabel;
    SAnalysisStats::OutputTotal(OutFile, PushFrontMax);
    SAnalysisStats::OutputTotal(OutFile, PushFrontVariable);
    OutFile << std::endl;
    OutFile << PushBackLabel;
    SAnalysisStats::OutputTotal(OutFile, PushBackMax);
    SAnalysisStats::OutputTotal(OutFile, PushBackVariable);
    OutFile << std::endl;
    OutFile << PopFrontLabel;
    SAnalysisStats::OutputTotal(OutFile, PopFrontMax);
    SAnalysisStats::OutputTotal(OutFile, PopFrontVariable);
    OutFile << std::endl;
    OutFile << PopBackLabel;
    SAnalysisStats::OutputTotal(OutFile, PopBackMax);
    SAnalysisStats::OutputTotal(OutFile, PopBackVariable);
    OutFile << std::endl;
    OutFile << ForwardLabel;
    SAnalysisStats::OutputTotal(OutFile, ForwardMax);
    SAnalysisStats::OutputTotal(OutFile, ForwardVariable);
    OutFile << std::endl;
    OutFile << ReverseLabel;
    SAnalysisStats::OutputTotal(OutFile, ReversedMax);
    SAnalysisStats::OutputTotal(OutFile, ReverseVariable);
    OutFile << std::endl
            << std::endl;
    OutFile << "Mean," << MaxSizeLabel << std::string(Iterations.size(), ',') << VariableSizeLabel << std::endl;
    for (std::size_t Index = 0; Index < 2; Index++)
    {
        for (auto Iteration : Iterations)
        {
            OutFile << "," << Iteration;
        }
    }
    OutFile << std::endl;
    OutFile << PushFrontLabel;
    SAnalysisStats::OutputMean(OutFile, PushFrontMax);
    SAnalysisStats::OutputMean(OutFile, PushFrontVariable);
    OutFile << std::endl;
    OutFile << PushBackLabel;
    SAnalysisStats::OutputMean(OutFile, PushBackMax);
    SAnalysisStats::OutputMean(OutFile, PushBackVariable);
    OutFile << std::endl;
    OutFile << PopFrontLabel;
    SAnalysisStats::OutputMean(OutFile, PopFrontMax);
    SAnalysisStats::OutputMean(OutFile, PopFrontVariable);
    OutFile << std::endl;
    OutFile << PopBackLabel;
    SAnalysisStats::OutputMean(OutFile, PopBackMax);
    SAnalysisStats::OutputMean(OutFile, PopBackVariable);
    OutFile << std::endl;
    OutFile << ForwardLabel;
    SAnalysisStats::OutputMean(OutFile, ForwardMax);
    SAnalysisStats::OutputMean(OutFile, ForwardVariable);
    OutFile << std::endl;
    OutFile << ReverseLabel;
    SAnalysisStats::OutputMean(OutFile, ReversedMax);
    SAnalysisStats::OutputMean(OutFile, ReverseVariable);
    OutFile << std::endl
            << std::endl;
    OutFile << "StdDev," << MaxSizeLabel << std::string(Iterations.size(), ',') << VariableSizeLabel << std::endl;
    for (std::size_t Index = 0; Index < 2; Index++)
    {
        for (auto Iteration : Iterations)
        {
            OutFile << "," << Iteration;
        }
    }
    OutFile << std::endl;
    OutFile << PushFrontLabel;
    SAnalysisStats::OutputStandardDeviation(OutFile, PushFrontMax);
    SAnalysisStats::OutputStandardDeviation(OutFile, PushFrontVariable);
    OutFile << std::endl;
    OutFile << PushBackLabel;
    SAnalysisStats::OutputStandardDeviation(OutFile, PushBackMax);
    SAnalysisStats::OutputStandardDeviation(OutFile, PushBackVariable);
    OutFile << std::endl;
    OutFile << PopFrontLabel;
    SAnalysisStats::OutputStandardDeviation(OutFile, PopFrontMax);
    SAnalysisStats::OutputStandardDeviation(OutFile, PopFrontVariable);
    OutFile << std::endl;
    OutFile << PopBackLabel;
    SAnalysisStats::OutputStandardDeviation(OutFile, PopBackMax);
    SAnalysisStats::OutputStandardDeviation(OutFile, PopBackVariable);
    OutFile << std::endl;
    OutFile << ForwardLabel;
    SAnalysisStats::OutputStandardDeviation(OutFile, ForwardMax);
    SAnalysisStats::OutputStandardDeviation(OutFile, ForwardVariable);
    OutFile << std::endl;
    OutFile << ReverseLabel;
    SAnalysisStats::OutputStandardDeviation(OutFile, ReversedMax);
    SAnalysisStats::OutputStandardDeviation(OutFile, ReverseVariable);
    OutFile << std::endl;

    return 0;
}

SAnalysisStats::SAnalysisStats() : DCount(0), DMean(0.0), DMeanDifferenceSquared(0.0)
{
}

void SAnalysisStats::AddSample(double sample)
{
    double Delta1 = sample - DMean;
    DMean += Delta1 / (Count() ? Count() : 1);
    double Delta2 = sample - DMean;
    DMeanDifferenceSquared += Delta1 * Delta2;
    DCount++;
}

std::size_t SAnalysisStats::Count() const
{
    return DCount;
}

double SAnalysisStats::Mean() const
{
    return DMean;
}

double SAnalysisStats::Variance() const
{
    return Count() ? DMeanDifferenceSquared / (Count() - 1) : 0.0;
}

double SAnalysisStats::StandardDeviation() const
{
    return std::sqrt(Variance());
}

SAnalysisStats SAnalysisStats::PushFrontTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations)
{
    SAnalysisStats Stats;
    std::size_t CurrentIteration = 0;
    while (deque->Size())
    {
        deque->PopBack();
    }
    while (CurrentIteration < iterations)
    {
        for (std::size_t Index = 0; Index < maxsize; Index++)
        {
            auto Start = std::chrono::steady_clock::now();
            deque->PushFront(Index);
            auto End = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> Duration = (End - Start);
            Stats.AddSample(Duration.count());
        }
        while (deque->Size())
        {
            deque->PopFront();
        }
        CurrentIteration += maxsize;
    }
    return Stats;
}

SAnalysisStats SAnalysisStats::PushBackTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations)
{
    SAnalysisStats Stats;
    std::size_t CurrentIteration = 0;
    while (deque->Size())
    {
        deque->PopBack();
    }
    while (CurrentIteration < iterations)
    {
        for (std::size_t Index = 0; Index < maxsize; Index++)
        {
            auto Start = std::chrono::steady_clock::now();
            deque->PushBack(Index);
            auto End = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> Duration = (End - Start);
            Stats.AddSample(Duration.count());
        }
        while (deque->Size())
        {
            deque->PopBack();
        }
        CurrentIteration += maxsize;
    }
    return Stats;
}

SAnalysisStats SAnalysisStats::PopFrontTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations)
{
    SAnalysisStats Stats;
    std::size_t CurrentIteration = 0;
    while (deque->Size())
    {
        deque->PopBack();
    }
    while (CurrentIteration < iterations)
    {
        for (std::size_t Index = 0; Index < maxsize; Index++)
        {
            deque->PushFront(Index);
        }
        while (deque->Size())
        {
            auto Start = std::chrono::steady_clock::now();
            deque->PopFront();
            auto End = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> Duration = (End - Start);
            Stats.AddSample(Duration.count());
        }
        CurrentIteration += maxsize;
    }
    return Stats;
}

SAnalysisStats SAnalysisStats::PopBackTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations)
{
    SAnalysisStats Stats;
    std::size_t CurrentIteration = 0;
    while (deque->Size())
    {
        deque->PopBack();
    }
    while (CurrentIteration < iterations)
    {
        for (std::size_t Index = 0; Index < maxsize; Index++)
        {
            deque->PushBack(Index);
        }
        while (deque->Size())
        {
            auto Start = std::chrono::steady_clock::now();
            deque->PopBack();
            auto End = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> Duration = (End - Start);
            Stats.AddSample(Duration.count());
        }
        CurrentIteration += maxsize;
    }
    return Stats;
}

SAnalysisStats SAnalysisStats::ForwardTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations)
{
    SAnalysisStats Stats;
    std::size_t CurrentIteration = 0;
    while (deque->Size())
    {
        deque->PopBack();
    }
    for (std::size_t Index = 0; Index < maxsize; Index++)
    {
        deque->PushBack(Index);
    }
    while (CurrentIteration < iterations)
    {
        auto Start = std::chrono::steady_clock::now();
        deque->PopFront();
        deque->PushBack(maxsize + CurrentIteration);
        auto End = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> Duration = (End - Start);
        Stats.AddSample(Duration.count());
        CurrentIteration++;
    }
    return Stats;
}

SAnalysisStats SAnalysisStats::ReverseTest(std::shared_ptr<CDeque> deque, std::size_t maxsize, std::size_t iterations)
{
    SAnalysisStats Stats;
    std::size_t CurrentIteration = 0;
    while (deque->Size())
    {
        deque->PopBack();
    }
    for (std::size_t Index = 0; Index < maxsize; Index++)
    {
        deque->PushFront(Index);
    }
    while (CurrentIteration < iterations)
    {
        auto Start = std::chrono::steady_clock::now();
        deque->PopBack();
        deque->PushFront(CurrentIteration);
        auto End = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> Duration = (End - Start);
        Stats.AddSample(Duration.count());
        CurrentIteration++;
    }
    return Stats;
}

void SAnalysisStats::OutputTotal(std::ostream &out, const std::vector<SAnalysisStats> &stats)
{
    for (auto Stat : stats)
    {
        out << "," << Stat.Mean() * Stat.Count();
    }
}

void SAnalysisStats::OutputMean(std::ostream &out, const std::vector<SAnalysisStats> &stats)
{
    for (auto Stat : stats)
    {
        out << "," << Stat.Mean();
    }
}

void SAnalysisStats::OutputStandardDeviation(std::ostream &out, const std::vector<SAnalysisStats> &stats)
{
    for (auto Stat : stats)
    {
        out << "," << Stat.StandardDeviation();
    }
}
