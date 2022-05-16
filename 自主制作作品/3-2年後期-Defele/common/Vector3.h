#pragma once

class Vector3
{
public:
	int x;
	int y;
	int z;

	Vector3 operator =(const Vector3& vec) const;

	bool operator <(const Vector3& vec) const;
	bool operator <=(const Vector3& vec) const;
	bool operator >(const Vector3& vec) const;
	bool operator >=(const Vector3& vec) const;
	bool operator ==(const Vector3& vec) const;
	bool operator !=(const Vector3& vec) const;
};

