#include <MaiSense/SensorChecker.hpp>
#include <MaiSense/InputManager.hpp>
#include <math.h>

namespace MaiSense
{
	SensorChecker::SensorChecker()
	{
		this->screenWidth = 0;
		this->screenHeight = 0;
		this->InitRegions();
	}

	SensorChecker::SensorChecker(int screenWidth, int screenHeight)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->InitRegions();
	}

	SensorChecker::SensorChecker(int screenWidth, int screenHeight, const std::map<SensorId, SensorRegion>& regionMap)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->InitRegions();
		this->regionMap.insert(regionMap.begin(), regionMap.end());
	}

	SensorChecker::~SensorChecker()
	{
	}

	void SensorChecker::InitRegions()
	{
		this->AddRegion(Sensor::C, C_REGION);
		this->AddRegion(Sensor::A1, A1_REGION);
		this->AddRegion(Sensor::A2, A2_REGION);
		this->AddRegion(Sensor::A3, A3_REGION);
		this->AddRegion(Sensor::A4, A4_REGION);
		this->AddRegion(Sensor::A5, A5_REGION);
		this->AddRegion(Sensor::A6, A6_REGION);
		this->AddRegion(Sensor::A7, A7_REGION);
		this->AddRegion(Sensor::A8, A8_REGION);
		this->AddRegion(Sensor::B1, B1_REGION);
		this->AddRegion(Sensor::B2, B2_REGION);
		this->AddRegion(Sensor::B3, B3_REGION);
		this->AddRegion(Sensor::B4, B4_REGION);
		this->AddRegion(Sensor::B5, B5_REGION);
		this->AddRegion(Sensor::B6, B6_REGION);
		this->AddRegion(Sensor::B7, B7_REGION);
		this->AddRegion(Sensor::B8, B8_REGION);
	}

	void SensorChecker::AddRegion(SensorId sensorId, const SensorRegion& region)
	{
		this->regionMap.insert(std::pair<SensorId, SensorRegion>(sensorId, region));
	}

	void SensorChecker::RemoveRegion(SensorId sensorId)
	{
		this->regionMap.erase(sensorId);
	}

	bool SensorChecker::Check(Point position, SensorId sensorId)
	{
		auto  sensor = InputManager::GetSensor();
		float scaleX = this->screenWidth / (float)REF_SCREEN_WIDTH;
		float scaleY = this->screenHeight / (float)REF_SCREEN_HEIGHT;

		if (this->regionMap.count(sensorId) <= 0)
			return false;

		return this->regionMap[sensorId].Test(position, scaleX, scaleY);
	}

    int SensorChecker::GetScreenWidth()
    {
        return this->screenWidth;
    }

    int SensorChecker::GetScreenHeight()
    {
        return this->screenHeight;
    }

	void SensorChecker::SetScreenSize(int width, int height)
	{
		this->screenWidth = width;
		this->screenHeight = height;
	}
}