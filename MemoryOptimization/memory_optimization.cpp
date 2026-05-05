#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct LargeRecord {
    std::uint64_t value;
    std::uint64_t cold1;
    std::uint64_t cold2;
    std::uint64_t cold3;
    std::uint64_t cold4;
    std::uint64_t cold5;
    std::uint64_t cold6;
    std::uint64_t cold7;
};

static volatile std::uint64_t anti_opt_sink = 0;

std::uint64_t parse_size_arg(char *arg, std::uint64_t default_value) {
    if (arg == nullptr) {
        return default_value;
    }

    char *end = nullptr;
    std::uint64_t value = std::strtoull(arg, &end, 10);

    if (end == arg || *end != '\0') {
        throw std::invalid_argument("Invalid numeric argument");
    }

    return value;
}

void trash_cache(std::vector<std::uint64_t> &cache_trash) {
    std::uint64_t sum = 0;

    for (std::size_t i = 0; i < cache_trash.size(); i += 16) {
        sum += cache_trash[i];
    }

    anti_opt_sink += sum;
}

template <typename Function>
std::uint64_t time_in_microseconds(Function function, int repeats) {
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < repeats; i++) {
        anti_opt_sink += function();
    }

    auto stop = std::chrono::steady_clock::now();

    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
    );
}

int main(int argc, char *argv[]) {
    const std::uint64_t default_records = 2000000;
    const int default_repeats = 40;

    std::uint64_t record_count = default_records;
    int repeats = default_repeats;

    try {
        if (argc >= 2) {
            record_count = parse_size_arg(argv[1], default_records);
        }

        if (argc >= 3) {
            repeats = static_cast<int>(parse_size_arg(argv[2], default_repeats));
        }
    } catch (const std::exception &error) {
        std::cerr << "Error: " << error.what() << "\n";
        std::cerr << "Usage: ./memory_optimization [record_count] [repeats]\n";
        return 1;
    }

    std::vector<LargeRecord> original_layout(record_count);
    std::vector<std::uint64_t> optimized_layout(record_count);

    for (std::uint64_t i = 0; i < record_count; i++) {
        std::uint64_t value = i ^ 0x9e3779b97f4a7c15ULL;

        original_layout[i].value = value;
        original_layout[i].cold1 = i + 1;
        original_layout[i].cold2 = i + 2;
        original_layout[i].cold3 = i + 3;
        original_layout[i].cold4 = i + 4;
        original_layout[i].cold5 = i + 5;
        original_layout[i].cold6 = i + 6;
        original_layout[i].cold7 = i + 7;

        optimized_layout[i] = value;
    }

    std::vector<std::uint64_t> cache_trash(16000000, 1);

    auto sum_original_layout = [&original_layout]() {
        std::uint64_t sum = 0;

        for (const LargeRecord &record : original_layout) {
            sum += record.value;
        }

        return sum;
    };

    auto sum_optimized_layout = [&optimized_layout]() {
        std::uint64_t sum = 0;

        for (std::uint64_t value : optimized_layout) {
            sum += value;
        }

        return sum;
    };

    std::uint64_t original_bytes =
        record_count * static_cast<std::uint64_t>(sizeof(LargeRecord));

    std::uint64_t optimized_bytes =
        record_count * static_cast<std::uint64_t>(sizeof(std::uint64_t));

    std::cout << "Low-level memory optimization demo\n\n";
    std::cout << "Record count: " << record_count << "\n";
    std::cout << "Repeats: " << repeats << "\n";
    std::cout << "LargeRecord size: " << sizeof(LargeRecord) << " bytes\n";
    std::cout << "Optimized value size: " << sizeof(std::uint64_t) << " bytes\n\n";

    std::cout << "Original layout bytes scanned per pass:  "
              << original_bytes << "\n";

    std::cout << "Optimized layout bytes scanned per pass: "
              << optimized_bytes << "\n\n";

    trash_cache(cache_trash);
    std::uint64_t original_time =
        time_in_microseconds(sum_original_layout, repeats);

    trash_cache(cache_trash);
    std::uint64_t optimized_time =
        time_in_microseconds(sum_optimized_layout, repeats);

    double speedup = static_cast<double>(original_time) /
                     static_cast<double>(optimized_time);

    std::cout << "Results\n";
    std::cout << "Original struct layout time:  "
              << original_time << " us\n";

    std::cout << "Optimized array layout time:  "
              << optimized_time << " us\n";

    std::cout << "Speedup from memory layout:   "
              << std::fixed << std::setprecision(2) << speedup << "x\n\n";

    std::cout << "Explanation\n";
    std::cout << "The original layout stores one hot value beside seven cold values.\n";
    std::cout << "When the program only needs the hot value, it still pulls the larger record through memory.\n";
    std::cout << "The optimized layout stores only the hot values in a contiguous array.\n";
    std::cout << "This improves cache locality and reduces the amount of memory scanned.\n\n";

    std::cout << "Anti-optimization sink: " << anti_opt_sink << "\n";

    return 0;
}