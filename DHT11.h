#ifndef DHT11_H
#define DHT11_H

#include <wiringPi.h>

class DHT11
{
public:
    DHT11(const int pin);
    float get_temperature();
    float get_humidite();
    bool wiringpi();
    
private:
    int dht_pin;
    float temperature;
    float humidite = 0;
    int time = millis();
    bool wiringpiOK = true;
    void read_dht_data();
};

#endif // DHT11_H
