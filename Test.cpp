/*Dette er tests for Sensor*/
/*Unit tests med Mock kan ikke gennemf�res i FreeRTOS, Derfor kan denne fil ikke anvendes til afpr�vning af funktioner*/
/*Hvis Mock test kunne implementeres ville det g�res som vist i denne fil*/

#include "gtest/gtest.h"

extern "C"
{
#include "Sensor.h"
}
class SensorTest : public ::testing::Test
{
protected:
	void SetUp() override {

	}
	void TearDown() override {

	}
};

testCreate(SensorTest,CreateTest) {

	Sensor sensor = createSensor();
	sensor->measurement = 2;
	ASSERT_EQ(sensor->measurement, 2); //Asserter at structen er blevet oprettet ordenligt, ved at skrive et variabel til det og derefter afl�se det.
	detroySensor(sensor); 

}

testGetData(SensorTest, GetDataTest) {
	Sensor sensor = createSensor();
	sensor->measurement = 10;
	ASSERT_EQ(getData(sensor), 10); //Asserter at getData returnerer den rigtige indeholdte data.
	detroySensor(sensor);
}

testMeasure(SensorTest, MeasureTest) {
	Sensor sensor = createSensor();
	sensor->measurement = 10;
	Measure(sensor);
	ASSERT_NE(getData(sensor), 10); //Asserter at Measure virker korrekt, Measure inds�tter en v�rdi under 10 i feltet measurement og getData vil derefter ikke kunne returnerer 10.
	detroySensor(sensor);
}