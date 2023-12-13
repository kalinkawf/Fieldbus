// modbus_hacker_Resistance.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>

extern "C" {
#include <modbus.h>
}

int main()
{
    std::cout << "Hello World!\n";
    //fprintf("Gowno \n");

    modbus_t* ctx;

    // Utwórz kontekst Modbus RTU na porcie COM (zmień nazwę portu zgodnie z Twoimi ustawieniami)
    ctx = modbus_new_rtu("COM5", 9600, 'N', 0, 0);

    // Ustawienia połączenia (zgodne z ustawieniami Twojego urządzenia)
    modbus_set_slave(ctx, 1); // Adres urządzenia Modbus
    // modbus_set_speed(ctx, 9600); // Szybkość transmisji
    modbus_set_byte_timeout(ctx, 0, 100000); // Czas oczekiwania na bajt w mikrosekundach
    modbus_set_response_timeout(ctx, 0, 100000); // Czas oczekiwania na odpowiedź w mikrosekundach

    // Połączenie z urządzeniem
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Nie udalo sie polaczyc: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // Tutaj możesz użyć odpowiednich funkcji Modbus do komunikacji z urządzeniem, np. odczytu rejestrów

    std::cout << "Gowno!\n";

    // Zakończ połączenie i zwolnij kontekst
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;

}

