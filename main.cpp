#include "includes.h"


int main() {
    while (true) {
        std::string inputPath;
        std::cout << "Pfad zur Datei: ";
        std::getline(std::cin, inputPath);

        if (inputPath.empty()) {
            std::cerr << "Kein Pfad eingegeben!\n";
            continue;
        }

        std::ifstream test(inputPath, std::ios::binary);
        if (!test) {
            std::cerr << "Fehler: Datei nicht gefunden!\n";
            continue;
        }
        test.close();

        std::string baseName = inputPath;
        size_t pos = baseName.find_last_of("/\\");
        if (pos != std::string::npos) baseName = baseName.substr(pos + 1);
        std::string outputPath = baseName + ".h";

        fileToHeader(inputPath, outputPath);
        break;
    }

    return 0;
}