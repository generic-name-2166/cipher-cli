#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto encrypt_file(std::string filename) -> void {
    std::filesystem::path path = std::filesystem::path(filename);
    std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);

    if (!file.is_open()) {
        std::cerr << "Couldn't open the specified file: " << filename << '\n';
        return;
    }

    file.close();
}

auto encrypt(std::vector<std::string> files) -> void {
    for (const std::string file : files) {
        encrypt_file(file);
    }
}
