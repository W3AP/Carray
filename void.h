#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

std::string makeSafeName(const std::string& name) {
    std::string result;
    for (char c : name) {
        if (std::isalnum(c) || c == '_') result += c;
        else result += '_';
    }
    return result;
}

void fileToHeader(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream in(inputPath, std::ios::binary);
    if (!in) {
        std::cerr << "Error: Cannot open " << inputPath << "\n";
        return;
    }

    std::vector<unsigned char> data((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());
    in.close();

    std::string baseName = inputPath;
    size_t pos = baseName.find_last_of("/\\");
    if (pos != std::string::npos) baseName = baseName.substr(pos + 1);
    baseName = baseName.substr(0, baseName.find_last_of('.'));

    std::string varName = makeSafeName(baseName);
    std::string guardName = varName;
    for (char& c : guardName) c = std::toupper(c);

    std::ofstream out(outputPath);
    if (!out) {
        std::cerr << "Error: Cannot create " << outputPath << "\n";
        return;
    }

    out << "#ifndef " << guardName << "_H\n";
    out << "#define " << guardName << "_H\n\n";
    out << "unsigned char " << varName << "[] = {\n";

    size_t total = data.size();
    for (size_t i = 0; i < total; ++i) {
        out << "0x" << std::hex << (int)data[i] << std::dec;
        if (i < total - 1) out << ",";
        if ((i + 1) % 16 == 0) out << "\n";
    }

    out << "\n};\n\n";
    out << "unsigned int " << varName << "_len = " << total << ";\n\n";
    out << "#endif\n";

    out.close();
    std::cout << "Created: " << outputPath << "\n";
}