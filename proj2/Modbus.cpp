#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>


uint16_t getCRC(const std::vector<uint8_t>& data) {
    uint16_t crc = 0xFFFF; // Początkowa wartość CRC w protokole MODBUS

    for (uint8_t byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001; // XOR z wielomianem generującym
            } else {
                crc >>= 1;
            }
        }
    }
    uint8_t reg8 = crc >> 8;
    return (crc << 8 | reg8);
    // return crc;
}

bool isValidHexChar(char c) {
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

int main() {
    std::vector<uint8_t> byteSequence;
    std::string input;

    std::cout << "Podaj sekwencje bajtow w notacji hex: ";
    std::getline(std::cin, input);

    if (input.size() % 2 != 0) {
        input.insert(input.size(), 1, '0');
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
        uint8_t byteValue;

        hexStream >> std::hex >> byteValue;

        if (hexStream.fail()) {
            std::cerr << "Nieprawidlowa notacja heksadecymalna: " << hexByte << std::endl;
            return 1;
        }

        byteSequence.push_back(byteValue);
    }

    if (byteSequence.size() > 256) {
        std::cerr << "Dlugosc sekwencji przekracza 256 bajtow." << std::endl;
        return 1;
    }

    // pobieranie liczby powtórzeń algorytmu
    uint32_t n;
    std::cout << "Podaj liczbe n powtorzen algorytmu CRC (1 do 1000000000): ";
    std::cin >> n;

    if (n < 1 || n > 1000000000) {
        std::cerr << "Wartosc poza zakresem." << std::endl;
        return 1;
    }

    auto startTime = std::chrono::high_resolution_clock::now(); // Algorytm start

    uint16_t crcResult = 0;
    

    for (unsigned int i = 0; i< n; ++i) {
        crcResult = getCRC(byteSequence);
    }

    auto endTime = std::chrono::high_resolution_clock::now(); // Algorytm koniec
    std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

    std::cout << "Suma CRC po " << n << " powtorzeniach: 0x" << std::hex << crcResult << std::dec << std::endl;
    std::cout << "Czas: " << duration.count() << "s" << std::endl;

    std::cin.get(); // Czekaj na wciśnięcie klawisza Enter
    return 0;
}
