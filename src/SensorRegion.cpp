#include <MaiSense/SensorRegion.hpp>
#include <cmath>

namespace MaiSense
{
	SensorRegion::SensorRegion()
	{
	}

	SensorRegion::SensorRegion(const std::vector<Point>& points)
	{
		this->points.reserve(points.size());
		this->points.insert(this->points.end(), points.begin(), points.end());
	}

	SensorRegion::~SensorRegion()
	{
	}

	void SensorRegion::AddPoint(const Point& point)
	{
		this->points.push_back(point);
	}

	bool SensorRegion::Test(const Point& testPoint, float scaleX, float scaleY)
	{
        if (this->points.size() < 3)
            return false;

        std::vector<Point> scaledPoints;

        for (unsigned int i = 0; i < this->points.size(); i++)
        {
            int scaledX = (int)ceil(this->points[i].X * scaleX);
            int scaledY = (int)ceil(this->points[i].Y * scaleY);
            scaledPoints.push_back({ scaledX, scaledY });
        }

        Point extreme = { 9999, testPoint.Y };

        int count = 0, i = 0, n = scaledPoints.size();
        do
        {
            int next = (i + 1) % n;

            if (DoesSegmentIntersect(scaledPoints[i], scaledPoints[next], testPoint, extreme))
            {
                if (FindOrientation(scaledPoints[i], testPoint, scaledPoints[next]) == COLINEAR)
                    return IsOnSegment(scaledPoints[i], testPoint, scaledPoints[next]);

                count++;
            }

            i = next;
        } while (i != 0);

        return count & 1;
	}

    bool SensorRegion::IsOnSegment(const Point& p, const Point& q, const Point& r)
    {
        if (q.X <= max(p.X, r.X) && q.X >= min(p.X, r.X) &&
            q.Y <= max(p.Y, r.Y) && q.Y >= min(p.Y, r.Y))
            return true;
        return false;
    }

    int SensorRegion::FindOrientation(const Point& p, const Point& q, const Point& r)
    {
        int val = (q.Y - p.Y) * (r.X - q.X) -
            (q.X - p.X) * (r.Y - q.Y);

        if (val == 0) 
            return COLINEAR;

        return (val > 0) ? CLOCKWISE : COUNTER_CLOCKWISE;
    }

    bool SensorRegion::DoesSegmentIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2)
    {
        int o1 = FindOrientation(p1, q1, p2);
        int o2 = FindOrientation(p1, q1, q2);
        int o3 = FindOrientation(p2, q2, p1);
        int o4 = FindOrientation(p2, q2, q1);

        if (o1 != o2 && o3 != o4)
            return true;

        if (o1 == COLINEAR && IsOnSegment(p1, p2, q1)) 
            return true;

        if (o2 == COLINEAR && IsOnSegment(p1, q2, q1))
            return true;

        if (o3 == COLINEAR && IsOnSegment(p2, p1, q2))
            return true;

        if (o4 == COLINEAR && IsOnSegment(p2, q1, q2))
            return true;

        return false;
    }
}