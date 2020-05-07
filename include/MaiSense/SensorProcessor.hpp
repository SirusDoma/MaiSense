#ifndef MAISENSE_SENSOR_PROCESSOR_HPP
#define MAISENSE_SENSOR_PROCESSOR_HPP

#include <unordered_map>
#include <vector>

#include <MaiSense/Sensor.hpp>
#include <MaiSense/SensorChecker.hpp>
#include <MaiSense/Point.hpp>
#include <MaiSense/TouchEvent.hpp>
#include <MaiSense/MouseEvent.hpp>

namespace MaiSense
{
	class SensorProcessor
	{
	private:
        SensorChecker *checker;
        Sensor *sensor;

	public:
        SensorProcessor();
        virtual ~SensorProcessor();

        SensorChecker *GetChecker();
        Sensor *GetSensor();

        void SetChecker(SensorChecker *checker);
        void SetSensor(Sensor *sensor);

        bool Handle(TouchEvent ev);
        bool Handle(MouseEvent ev);
        bool Handle(const Point& pointer, bool active);
	};
}

#endif