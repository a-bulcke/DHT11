#include "DHT11.h"


#define MAX_TIMINGS   85

DHT11::DHT11(int pin) : dht_pin(1)
{
	dht_pin = pin;
    wiringPiSetup();

	if ( wiringPiSetup() == -1 )
		wiringpiOK = false;
		
	read_dht_data();
	delay(250);
	
}
bool DHT11::wiringpi()
{
	return wiringpiOK;
}
float DHT11::get_temperature()
{
	if (millis() - time < 1000)
	{
		read_dht_data();
		delay(250);
	}
	return this->temperature;
}

float DHT11::get_humidite()
{
	if (millis() - time > 1000)
	{
		read_dht_data();
		delay(250);

	}
	return this->humidite;
}

void DHT11::read_dht_data()
{
	int data[5] = { 0, 0, 0, 0, 0 }; 
	int laststate	= HIGH;
	int counter		= 0;
	int j			= 0,i;
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;
	
	/* pin etat bas pendant 18 ms */
	pinMode( dht_pin, OUTPUT );
	digitalWrite( dht_pin, LOW );
	delay( 18 );
	
	/* pin en entree */
	pinMode( dht_pin, INPUT );
	
	/* detecter un changement et lecture des donnees */
	for (i=0; i<MAX_TIMINGS; i++)
	{
		counter = 0;
		while ( digitalRead( dht_pin ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( dht_pin );
		if ( counter == 255 )
			break;
		/* ignorer les 3 premiers transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* chaque bit est sauvegarde */
			data[j / 8] <<= 1;
			if ( counter > 16 )
				data[j / 8] |= 1;
			j++;
		}
	}
	/*
    * On controle si on a bien lu 40 bits (8 bit x 5 ) + verification du checksum (dernier octet)
    * si c'est OK on sauvegarde dans h et c
    */
	if ( (j >= 40) &&
		(data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
	{
		float h = (float)((data[0] << 8) + data[1]) / 10;
		if ( h > 100 )
		{
			h = data[0];	// pour le DHT11
		}
		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if ( c > 125 )
		{
			c = data[2];	// pour le DHT11
		}
		if ( data[2] & 0x80 )
		{
			c = -c;
		}
		//float f = c * 1.8f + 32; //Fahrenheit
		
		this-> humidite = h;
		this->temperature = c;
		this->time = millis();
	}
	else
	{
		this->time = 0; //pour recommencer la lecture la prochaine fois
	}
}
