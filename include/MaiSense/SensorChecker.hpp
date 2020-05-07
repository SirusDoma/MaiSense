#ifndef MAISENSE_SENSOR_CHECKER_HPP
#define MAISENSE_SENSOR_CHECKER_HPP

#include <map>

#include <MaiSense/Sensor.hpp>
#include <MaiSense/TouchController.hpp>
#include <MaiSense/SensorRegion.hpp>

namespace MaiSense
{
	class SensorChecker
	{
	private:
		const SensorRegion C_REGION = SensorRegion({ { 500, 380 }, { 585, 415 }, { 615, 500 }, { 585, 585 }, { 500, 620 }, { 415, 585 }, { 380, 500 }, { 415, 415 } });
		const SensorRegion A1_REGION = SensorRegion({ { 500, 0 }, { 999, 0 }, { 710, 290 }, { 500, 200 } });
		const SensorRegion A2_REGION = SensorRegion({ { 999, 0 }, { 999, 500 }, { 800, 500 }, { 710, 290 } });
		const SensorRegion A3_REGION = SensorRegion({ { 800, 500 }, { 999, 500 }, { 999, 999 }, { 710, 710 } });
		const SensorRegion A4_REGION = SensorRegion({ { 710, 710 }, { 999, 999 }, { 500, 999 }, { 500, 800 } });
		const SensorRegion A5_REGION = SensorRegion({ { 500, 800 }, { 500, 999 }, { 0, 999 }, { 290, 710 } });
		const SensorRegion A6_REGION = SensorRegion({ { 290, 710 }, { 0, 999 }, { 0, 500 }, { 200, 500 } });
		const SensorRegion A7_REGION = SensorRegion({ { 200, 500 }, { 0, 500 }, { 0, 0 }, { 290, 290 } });
		const SensorRegion A8_REGION = SensorRegion({ { 0, 0 }, { 500, 0 }, { 500, 200 }, { 290, 290 } });
		const SensorRegion B1_REGION = SensorRegion({ { 500, 200 }, { 790, 290 }, { 585, 415 }, { 500, 380 } });
		const SensorRegion B2_REGION = SensorRegion({ { 710, 290 }, { 800, 500 }, { 620, 500 }, { 585, 415 } });
		const SensorRegion B3_REGION = SensorRegion({ { 620, 500 }, { 800, 500 }, { 710, 710 }, { 585, 585 } });
		const SensorRegion B4_REGION = SensorRegion({ { 585, 585 }, { 710, 710 }, { 500, 800 }, { 500, 620 } });
		const SensorRegion B5_REGION = SensorRegion({ { 500, 620 }, { 500, 800 }, { 290, 710 }, { 415, 585 } });
		const SensorRegion B6_REGION = SensorRegion({ { 415, 585 }, { 380, 500 }, { 200, 500 }, { 290, 710 } });
		const SensorRegion B7_REGION = SensorRegion({ { 200, 500 }, { 380, 500 }, { 415, 415 }, { 290, 290 } });
		const SensorRegion B8_REGION = SensorRegion({ { 290, 290 }, { 500, 200 }, { 500, 380 }, { 415, 415 } });
		const int REF_SCREEN_WIDTH = 1000;
		const int REF_SCREEN_HEIGHT = 1000;

		std::map<SensorId, SensorRegion> regionMap;
		int screenWidth;
		int screenHeight;

		void InitRegions();
	public:
		SensorChecker();
		SensorChecker(int screenWidth, int screenHeight);
		SensorChecker(int screenWidth, int screenHeight, const std::map<SensorId, SensorRegion>& regionMap);
		~SensorChecker();

		void AddRegion(SensorId sensorId, const SensorRegion& region);
		void RemoveRegion(SensorId sensorId);
		bool Check(Point position, SensorId sensorId);

        int GetScreenWidth();
        int GetScreenHeight();
        void SetScreenSize(int width, int height);
	};
}

#endif