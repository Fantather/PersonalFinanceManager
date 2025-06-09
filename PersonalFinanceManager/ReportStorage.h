#pragma once
#include "Report.h"

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <chrono>

class ReportStorage {
public:
    enum PeriodType { Daily = 0, Weekly = 1, Monthly = 2 };

    static bool save(const std::vector<std::unique_ptr<Report>>& reports,
        const std::vector<PeriodType>& types,
        const std::string& filename);

    static bool load(std::vector<std::unique_ptr<Report>>& reports,
        std::vector<PeriodType>& types,
        const std::string& filename);
};