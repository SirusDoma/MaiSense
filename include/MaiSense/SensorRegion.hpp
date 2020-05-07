#ifndef MAISENSE_SENSOR_REGION_HPP
#define MAISENSE_SENSOR_REGION_HPP

#include <vector>

#include <MaiSense/Sensor.hpp>
#include <MaiSense/Point.hpp>

namespace MaiSense
{
	class SensorRegion
	{
	private:
		const int COLINEAR = 0;
		const int CLOCKWISE = 1;
		const int COUNTER_CLOCKWISE = 2;

		std::vector<Point> points;

		bool IsOnSegment(const Point& p, const Point& q, const Point& r);
		int FindOrientation(const Point& p, const Point& q, const Point& r);
		bool DoesSegmentIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2);

	public:
		SensorRegion();
		SensorRegion(const std::vector<Point>& points);
		~SensorRegion();

		void AddPoint(const Point& point);
		bool Test(const Point& testPoint, float scaleX = 1.0, float scaleY = 1.0f);
	};
}

#endif
