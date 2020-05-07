#include <MaiSense/SensorProcessor.hpp>

namespace MaiSense
{
    SensorProcessor::SensorProcessor() :
        checker(0),
        sensor(0)
    {
    }

    SensorProcessor::~SensorProcessor()
    {
    }

    SensorChecker* SensorProcessor::GetChecker()
    {
        return this->checker;
    }

    Sensor* SensorProcessor::GetSensor()
    {
        return this->sensor;
    }

    void SensorProcessor::SetChecker(SensorChecker* checker)
    {
        this->checker = checker;
    }

    void SensorProcessor::SetSensor(Sensor* sensor)
    {
        this->sensor = sensor;
    }

	bool SensorProcessor::Handle(TouchEvent ev)
	{
        bool active = !(ev.Flag & POINTER_FLAG_UP || ev.Flag & POINTER_FLAG_CANCELED);
        return Handle({ev.X, ev.Y}, ev.Flag);
	}
    
    bool SensorProcessor::Handle(MouseEvent ev)
    {
        return Handle({ ev.X, ev.Y }, ev.LButton);
    }
    
    bool SensorProcessor::Handle(const Point& pointer, bool active)
    {
        if (this->checker == NULL)
            return false;

        if (this->sensor == NULL)
            return false;

        const auto sensors = {
          Sensor::A1,
          Sensor::A2,
          Sensor::A3,
          Sensor::A4,
          Sensor::A5,
          Sensor::A6,
          Sensor::A7,
          Sensor::A8,
          Sensor::B1,
          Sensor::B2,
          Sensor::B3,
          Sensor::B4,
          Sensor::B5,
          Sensor::B6,
          Sensor::B7,
          Sensor::B8,
          Sensor::C
        };

        for (auto sensorId : sensors)
        {
            if (checker->Check(pointer, sensorId))
            {
                sensor->Queue(sensorId, active);
                return true;
            }
        }

        return false;
    }
}