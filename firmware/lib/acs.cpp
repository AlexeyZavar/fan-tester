/**************************************************************************/
/*!
    @file     ACS7xx_Allegro.h
    @author   SOSAndroid.fr (E. Ha.)

    @section  HISTORY

	v1.0 - First release
    v1.1 - Moving exponential average + robustness
	v1.2 - Modify the constructor call to avoid some errors

    Set of methods to use measurements from ACS7xx sensors from Allegro Micro
	All those methods are made to be called periodically

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, SOSAndroid.fr (E. Ha.)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _ACS7XX_ALLEGRO_H_
#define _ACS7XX_ALLEGRO_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// Enabling debug - comment to disable / normal operations
//#define SERIAL_DEBUG

// Default values - default is ACS712-20A
#define ACS7XX_BIDIR_DEFAULT true //Is the sensor bidirectional ?
#define ACS7XX_PIN_DEFAULT	0 //Pin used  for measurements - default A0 from Mega 2650
#define ACS7XX_SENSITIVITY_DEFAULT  0.1 //Sensitivity of the sensor in V/A (divide per 1000 the datasheet value)
#define ACS7XX_FACTOR_VALUE 1000 // adjust the unit read. For instance, a sensitivity in V/A and a 1000 factor provides a mA reading. 1 => A, 1000 => mA
// Default Arduino values
#define BOARD_MEASURED_OFFSET 1 //ADC may shift its measurement. For instance a shitfted value of 1 out of 1024 steps represents 0.05A for ACS712. In perfect world, BOARD_MEASURED_OFFSET should be 0
#define BOARD_VOLTAGE_DEFAULT 5.0 //Power supply voltage
#define BOARD_ADC_DEPTH 1024.0 // 10bits: 1024.0, 12bits: 4096.0, 14bits: 16384.0
// Moving Exponential Average init values
#define EXP_MOVAVG_N 30	//history length impact on moving average impact - keep in mind the moving average will be impacted by the measurement frequency too
#define EXP_MOVAVG_LOOP 1 //number of measurements before starting mobile Average - starting with a simple average - 1 allows a quick start. Value must be 1 minimum


class ACS7XX_ALLEGRO {
public:
    ACS7XX_ALLEGRO(void);
    ACS7XX_ALLEGRO(boolean bidir, int pintoread, double voltage, double sensitivity);

    void	begin(void);
    void	instantCurrent(double *current); //calculates in mA
    void	ampereHourCount(double *mamperehc); // mAh
    void	resetCounters(void); // reset all counters
    void 	updateMillis(void); // when starting a new cycle, we need to init Lastmillis
    void	updateCounters(void);
    void	getAHCount(double *mahcount);
    void	getCoulombCount(double *ccount);
    void	printDebugDeviceInit(void);
    double	getMovingAvgExp(void);
    void	resetMovingAvgExp(void);

private:
    unsigned long	_lastMillis;
    double	_lastCurrent;
    double	_lastAmperehour;
    double	_lastCoulomb;
    double	_AHCounter;
    double	_CoulombCounter;

    boolean	_bidir;
    int		_pintoread;
    double	_voltage;
    double	_voltage_offset;
    double	_factor_value;
    double	_sensitivity;
    double	_resolution;
    int		_adc_offset;
    double	_movavgexp_val;
    double	_movavgexp_alpha;
    int		_movavgexp_loop;

    void	movingAvgExp(double current);
};

#endif


/**************************************************************************/
/*!
    @file     CurrentIC_ACS7xx_Allegro.cpp
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

    Set of methods to use measurements from ACS7xx sensors from Allegro Micro
	All those methods are made to be called periodically

    @section  HISTORY

    v1.0 - First release
	V1.1 - Moving exponential average + robustness
	v1.2 - Modify the constructor call to avoid some errors
	v1.2.1 - Fix issue #1
*/
/**************************************************************************/

#include <stdlib.h>

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
ACS7XX_ALLEGRO::ACS7XX_ALLEGRO(void)
{
    _bidir = ACS7XX_BIDIR_DEFAULT;
    _pintoread = ACS7XX_PIN_DEFAULT;
    _sensitivity = ACS7XX_SENSITIVITY_DEFAULT;
    _voltage = BOARD_VOLTAGE_DEFAULT;
}

ACS7XX_ALLEGRO::ACS7XX_ALLEGRO(boolean bidir, int pintoread, double voltage, double sensitivity)
{
    _bidir = bidir;
    _pintoread = pintoread;
    _sensitivity = sensitivity;
    _voltage = voltage;
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

void ACS7XX_ALLEGRO::begin(void) {


    if (_bidir) {
        _voltage_offset = _voltage / 2.0;
    }
    else {
        _voltage_offset = 0.0;
    }
    _resolution = (double) BOARD_ADC_DEPTH;
    _adc_offset = (int) BOARD_MEASURED_OFFSET;
    _factor_value = (double) ACS7XX_FACTOR_VALUE;

    _lastMillis = 0; //need to add a function to start the counting by initializing lastmillis when starting a discharge / charge cycle
    _lastCurrent = 0.0;
    _AHCounter = 0.0;
    _CoulombCounter = 0.0;

    _movavgexp_alpha = 2.0 / (EXP_MOVAVG_N + 1.0);
    _movavgexp_loop = 0;
    _movavgexp_val = 0.0;

#ifdef SERIAL_DEBUG
    if(!Serial) {
			Serial.begin(9600);
		}
#endif
    return;
}

void ACS7XX_ALLEGRO::instantCurrent(double *current)
{
    int readvalue = analogRead(_pintoread) + _adc_offset;
    double readvolt = (((double) readvalue / _resolution) * _voltage) - _voltage_offset;
    double readcur = readvolt * _factor_value / _sensitivity;
    *current = readcur;
    _lastCurrent = readcur;
    ACS7XX_ALLEGRO::movingAvgExp(readcur);

#ifdef SERIAL_DEBUG
    if (Serial){
			Serial.print("Read value on pin including offset: ");
			Serial.println(readvalue, DEC);
			Serial.print("Current: ");
			Serial.print(readcur, DEC);
			Serial.println(" mA");
			Serial.print("Moving average: ");
			Serial.print(ACS7XX_ALLEGRO::getMovingAvgExp(), DEC);
			Serial.println(" mA");
		}
#endif

    return;
}

void ACS7XX_ALLEGRO::ampereHourCount(double *mamperehc)
{
    unsigned long currentmillis = millis();
    double timeframehour = (double)(currentmillis - _lastMillis) / 3600000.0;

    double readcurrent;
    ACS7XX_ALLEGRO::instantCurrent(&readcurrent);
    *mamperehc = readcurrent * timeframehour;

    _lastMillis = currentmillis;

#ifdef SERIAL_DEBUG
    if (Serial){
			Serial.print("AmpHour: ");
			Serial.print(*mamperehc, DEC);
			Serial.println(" mAh");
			Serial.print("timeframe ");
			Serial.print(timeframehour, DEC);
			Serial.println(" hour");
		}
#endif

    return;
}

void ACS7XX_ALLEGRO::updateCounters(void)
{
    double amperehcTemp;
    ACS7XX_ALLEGRO::ampereHourCount(&amperehcTemp);
    _lastAmperehour = amperehcTemp;
    _AHCounter += amperehcTemp;

    _lastCoulomb = amperehcTemp * 3.6;
    _CoulombCounter += _lastCoulomb;

#ifdef SERIAL_DEBUG
    if (Serial){
			Serial.print("mAH counter ");
			Serial.print(_AHCounter, DEC);
			Serial.println(" mAH");
			Serial.print("Coulomb counter ");
			Serial.print(_CoulombCounter, DEC);
			Serial.println(" C");
			Serial.println("Counters updated");
		}
#endif

    return;
}

void ACS7XX_ALLEGRO::resetCounters(void)
{
    _lastAmperehour = 0.0;
    _AHCounter = 0.0;
    _lastCoulomb = 0.0;
    _CoulombCounter = 0.0;

#ifdef SERIAL_DEBUG
    if (Serial){
			Serial.println("Counters reseted");
		}
#endif

    return;
}

void ACS7XX_ALLEGRO::updateMillis(void)
{
    _lastMillis = millis();

#ifdef SERIAL_DEBUG
    if (Serial){
			Serial.println("Millis updated");
		}
#endif

    return;
}

void ACS7XX_ALLEGRO::getAHCount(double *ahcount)
{
    *ahcount = _AHCounter;
    return;
}

void ACS7XX_ALLEGRO::getCoulombCount(double *ccount)
{
    *ccount = _CoulombCounter;
    return;
}

void ACS7XX_ALLEGRO::printDebugDeviceInit(void)
{

#ifdef SERIAL_DEBUG
    if (Serial){
			Serial.println("ACS7XX sensor object initialized");
			Serial.print("PIN: ");
			Serial.println(_pintoread, DEC);
			Serial.print("Sensitivity: ");
			Serial.print(_sensitivity, DEC);
			Serial.println(" V/A");
			Serial.print("device is ");
			if(!_bidir) Serial.print("not ");
			Serial.println("bidirectional");
			Serial.println("...... ...... ......");
		}
#endif

    return;
}

void ACS7XX_ALLEGRO::movingAvgExp(double current) {

    //init moving average exponetial with simple average of

    if (_movavgexp_loop < EXP_MOVAVG_LOOP) {
        _movavgexp_val += current;
        if (_movavgexp_loop == (EXP_MOVAVG_LOOP - 1)) _movavgexp_val = _movavgexp_val / (double) EXP_MOVAVG_LOOP;
        _movavgexp_loop ++;
    }
    else {
        double movavgexp = _movavgexp_val + _movavgexp_alpha * (current - _movavgexp_val);
        _movavgexp_val = movavgexp;
    }

    return;
}

double ACS7XX_ALLEGRO::getMovingAvgExp(void) {
    return _movavgexp_val;
}

void ACS7XX_ALLEGRO::resetMovingAvgExp(void) {
    _movavgexp_val = 0;
    _movavgexp_loop = 0;
    return;
}