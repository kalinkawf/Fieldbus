#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <limits>
#include <string>
#include <chrono>

unsigned int getCRC(const std::vector<unsigned char>& data) {
    // Algorytm
    return 0x123478;
}

bool isValidHexChar(char c) {
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

int main() {
    std::vector<unsigned char> byteSequence;
    std::string input;

    std::cout << "Podaj sekwencje bajtow w notacji hex: ";
    std::getline(std::cin, input);

    if (input.size() % 2 != 0) {
        input.insert(input.size(), 1, '0');
        std::cout << "Parzysta liczba znakow w notacji hex, Dodalem zero na koncu" << std::endl;
    }

    for (size_t i = 0; i < input.size(); i += 2) {
        std::string hexByte = input.substr(i, 2);

        for (char c : hexByte) {
            if (!isValidHexChar(c)) {
                std::cerr << "Nieprawidlowa notacja heksadecymalna: " << hexByte << std::endl;
                return 1;
            }
        }

        std::stringstream hexStream(hexByte);
        unsigned int byteValue;

        hexStream >> std::hex >> byteValue;

        if (hexStream.fail()) {
            std::cerr << "Nieprawidlowa notacja heksadecymalna: " << hexByte << std::endl;
            return 1;
        }

        byteSequence.push_back(static_cast<unsigned char>(byteValue));
    }

    if (byteSequence.size() > 256) {
        std::cerr << "Dlugosc sekwencji przekracza 256 bajtow." << std::endl;
        return 1;
    }

    unsigned int n;
    std::cout << "Podaj liczbe n powtorzen algorytmu CRC (1 do 1000000000): ";
    std::cin >> n;

    if (n < 1 || n > 1000000000) {
        std::cerr << "Wartosc poza zakresem." << std::endl;
        return 1;
    }

    auto startTime = std::chrono::high_resolution_clock::now(); // Algorytm start

    unsigned int totalCRC = 0;

    for (unsigned int i = 0; i < n; ++i) {
        unsigned int crcResult = getCRC(byteSequence);
        totalCRC += crcResult;
    }

    auto endTime = std::chrono::high_resolution_clock::now(); // Algorytm koniec
    std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

    std::cout << "Suma CRC po " << n << " powtorzeniach: 0x" << std::hex << std::setw(8) << std::setfill('0') << totalCRC << std::dec << std::endl;
    std::cout << "Czas: " << duration.count() << "s" << std::endl;

    return 0;
}
