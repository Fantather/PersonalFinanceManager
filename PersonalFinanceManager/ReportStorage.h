#pragma once

#include "Report.h"
#include <string>
#include <vector>

class ReportStorage {
public:
    // Сохраняет сразу несколько отчётов в один файл (дописыванием)
    static bool save_reports(
        const std::vector<std::unique_ptr<Report>>& reports,
        const std::string& filename);
};