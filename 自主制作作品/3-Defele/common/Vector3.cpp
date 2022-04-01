#include "Vector3.h"

Vector3 Vector3::operator=(const Vector3& vec) const
{
	Vector3 tmp;
	tmp.x = vec.x;
	tmp.y = vec.y;
	tmp.z = vec.z;
	return tmp;
}

bool Vector3::operator<(const Vector3& vec) const
{
	if (x < vec.x&& y < vec.y&& z < vec.z)
	{
		return true;
	}
	return false;
}

bool Vector3::operator<=(const Vector3& vec) const
{
	if (x <= vec.x&& y <= vec.y&& z <= vec.z)
	{
		return true;
	}
	return false;
}

bool Vector3::operator>(const Vector3& vec) const
{
	if (x > vec.x&& y > vec.y&& z > vec.z)
	{
		return true;
	}
	return false;
}

bool Vector3::operator>=(const Vector3& vec) const
{
	if (x >= vec.x&& y >= vec.y&& z >= vec.z)
	{
		return true;
	}
	return false;
}

bool Vector3::operator==(const Vector3& vec) const
{
	if (x == vec.x&& y == vec.y&& z == vec.z)
	{
		return true;
	}
	return false;
}

bool Vector3::operator!=(const Vector3& vec) const
{
	if (x != vec.x || y != vec.y || z != vec.z)
	{
		return true;
	}
	return false;
}
