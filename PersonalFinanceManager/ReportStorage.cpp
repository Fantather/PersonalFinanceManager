#include "ReportStorage.h"
#include <cstdint>

using namespace std;

static void write_time(ofstream& ofs, const chrono::system_clock::time_point& tp) {
    auto sec = chrono::duration_cast<chrono::seconds>(tp.time_since_epoch()).count();
    ofs.write(reinterpret_cast<const char*>(&sec), sizeof(sec));
}

static chrono::system_clock::time_point read_time(ifstream& ifs) {
    int64_t sec;
    ifs.read(reinterpret_cast<char*>(&sec), sizeof(sec));
    return chrono::system_clock::time_point(chrono::seconds(sec));
}

bool ReportStorage::save(const vector<unique_ptr<Report>>& reports,
    const vector<PeriodType>& types,
    const string& filename)
{
    if (reports.size() != types.size()) return false;

    ofstream ofs(filename, ios::binary);
    if (!ofs) return false;

    // Write number of reports
    uint32_t count = static_cast<uint32_t>(reports.size());
    ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (size_t i = 0; i < reports.size(); ++i) {
        const Report& r = *reports[i];
        const PeriodType type = types[i];

        // Write period type
        ofs.write(reinterpret_cast<const char*>(&type), sizeof(type));

        // Write timestamps
        write_time(ofs, r.period_start);
        write_time(ofs, r.period_end);

        // Write totals
        ofs.write(reinterpret_cast<const char*>(&r.total_income), sizeof(double));
        ofs.write(reinterpret_cast<const char*>(&r.total_expenses), sizeof(double));

        // Write expenses by category
        uint32_t cat_count = static_cast<uint32_t>(r.expenses_by_category.size());
        ofs.write(reinterpret_cast<const char*>(&cat_count), sizeof(cat_count));
        for (const auto& [cat, sum] : r.expenses_by_category) {
            ofs.write(reinterpret_cast<const char*>(&cat), sizeof(cat));
            ofs.write(reinterpret_cast<const char*>(&sum), sizeof(sum));
        }

        // Write top3 transactions
        uint32_t tx_count = static_cast<uint32_t>(r.top3_transactions.size());
        ofs.write(reinterpret_cast<const char*>(&tx_count), sizeof(tx_count));
        for (const auto& [id, amount] : r.top3_transactions) {
            ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));
            ofs.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
        }

        // Write top3 categories
        uint32_t top_cat_count = static_cast<uint32_t>(r.top3_categories.size());
        ofs.write(reinterpret_cast<const char*>(&top_cat_count), sizeof(top_cat_count));
        for (const auto& [cat, amount] : r.top3_categories) {
            ofs.write(reinterpret_cast<const char*>(&cat), sizeof(cat));
            ofs.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
        }
    }

    return true;
}

bool ReportStorage::load(vector<unique_ptr<Report>>& reports,
    vector<PeriodType>& types,
    const string& filename)
{
    ifstream ifs(filename, ios::binary);
    if (!ifs) return false;

    // Read number of reports
    uint32_t count;
    ifs.read(reinterpret_cast<char*>(&count), sizeof(count));

    reports.clear();
    types.resize(count);

    for (uint32_t i = 0; i < count; ++i) {
        auto rep = make_unique<Report>();

        // Read period type
        PeriodType type;
        ifs.read(reinterpret_cast<char*>(&type), sizeof(type));
        types[i] = type;

        // Read timestamps
        rep->period_start = read_time(ifs);
        rep->period_end = read_time(ifs);

        // Read totals
        ifs.read(reinterpret_cast<char*>(&rep->total_income), sizeof(double));
        ifs.read(reinterpret_cast<char*>(&rep->total_expenses), sizeof(double));

        // Read expenses by category
        uint32_t cat_count;
        ifs.read(reinterpret_cast<char*>(&cat_count), sizeof(cat_count));
        for (uint32_t j = 0; j < cat_count; ++j) {
            int cat;
            double sum;
            ifs.read(reinterpret_cast<char*>(&cat), sizeof(cat));
            ifs.read(reinterpret_cast<char*>(&sum), sizeof(sum));
            rep->expenses_by_category[cat] = sum;
        }

        // Read top3 transactions
        uint32_t tx_count;
        ifs.read(reinterpret_cast<char*>(&tx_count), sizeof(tx_count));
        rep->top3_transactions.resize(tx_count);
        for (auto& [id, amount] : rep->top3_transactions) {
            ifs.read(reinterpret_cast<char*>(&id), sizeof(id));
            ifs.read(reinterpret_cast<char*>(&amount), sizeof(amount));
        }

        // Read top3 categories
        uint32_t top_cat_count;
        ifs.read(reinterpret_cast<char*>(&top_cat_count), sizeof(top_cat_count));
        rep->top3_categories.resize(top_cat_count);
        for (auto& [cat, amount] : rep->top3_categories) {
            ifs.read(reinterpret_cast<char*>(&cat), sizeof(cat));
            ifs.read(reinterpret_cast<char*>(&amount), sizeof(amount));
        }

        reports.push_back(move(rep));
    }

    return true;
}