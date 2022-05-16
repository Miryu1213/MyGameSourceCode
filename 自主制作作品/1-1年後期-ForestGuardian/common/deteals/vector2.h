#pragma once

template<class T>
vector2template<T>::vector2template()
{
	x = 0;
	y = 0;
}

template<class T>
vector2template<T>::vector2template(T x, T y)
{
	vector2template<T>::x = x;
	vector2template<T>::y = y;
}

template<class T>
vector2template<T>::~vector2template()
{
}

template<class T>
vector2template<T>& vector2template<T>::operator=(const vector2template& vec)
{
	x = vec.x;
	y = vec.y;
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
int& vector2template<T>::operator[](int no)
{
	if (no == 0)
	{
		return x;
	}
	else if (no == 1)
	{
		return y;
	}
	else
	{
		return x;
	}
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T>& vector2template<T>::operator+=(const vector2template& vec)
{
	x += vec.x;
	y += vec.y;
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T>& vector2template<T>::operator-=(const vector2template<T>& vec)
{
	x -= vec.x;
	y -= vec.y;
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T>& vector2template<T>::operator*=(const vector2template<T>& vec)
{
	x *= vec.x;
	y *= vec.y;
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T>& vector2template<T>::operator/=(const vector2template<T>& vec)
{
	x /= vec.x;
	y /= vec.y;
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T>& vector2template<T>::operator+=(T vec)
{
	x += vec;
	y += vec;
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T>& vector2template<T>::operator-=(T vec)
{
	x -= vec;
	y -= vec;
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T>& vector2template<T>::operator*=(T vec)
{
	x *= vec;
	y *= vec;
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T>& vector2template<T>::operator/=(T vec)
{
	x /= vec;
	y /= vec;
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T> vector2template<T>::operator+() const
{
	return (*this);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
vector2template<T> vector2template<T>::operator-() const
{
	return Vector2(-this->x, -this->y);
	// TODO: return ステートメントをここに挿入します
}

template<class T>
bool vector2template<T>::operator==(const vector2template<T>& vec) const
{
	if (x == vec.x && y == vec.y)
	{
		return true;
	}
	return false;
}

template<class T>
bool vector2template<T>::operator!=(const vector2template<T>& vec) const
{
	if (x != vec.x || y != vec.y)
	{
		return true;
	}
	return false;
}

template<class T>
vector2template<T> operator+(const vector2template<T>& u, T v)
{
	return vector2template<T>(u.x + v, u.y + v);
}

template<class T>
vector2template<T> operator-(const vector2template<T>& u, T v)
{
	return vector2template<T>(u.x - v, u.y - v);
}

template<class T>
vector2template<T> operator*(const vector2template<T>& u, T v)
{
	return vector2template<T>(u.x * v, u.y * v);
}

template<class T>
vector2template<T> operator/(const vector2template<T>& u, T v)
{
	return vector2template<T>(u.x / v, u.y / v);
}

template<class T>
vector2template<T> operator%(const vector2template<T>& u, int v)
{
	return vector2template<T>(u.x % v, u.y % v);
}

template<class T>
vector2template<T> operator+(const vector2template<T>& u, const vector2template<T>& u2)
{
	return vector2template<T>(u.x + u2.x, u.y + u2.y);
}

template<class T>
vector2template<T> operator-(const vector2template<T>& u, const vector2template<T>& u2)
{
	return vector2template<T>(u.x - u2.x, u.y - u2.y);
}

template<class T>
vector2template<T> operator*(const vector2template<T>& u, const vector2template<T>& u2)
{
	return vector2template<T>(u.x * u2.x, u.y * u2.y);
}

template<class T>
vector2template<T> operator/(const vector2template<T>& u, const vector2template<T>& u2)
{
	return vector2template<T>(u.x / u2.x, u.y / u2.y);
}

template<class T>
vector2template<T> operator%(const vector2template<T>& u, const vector2template<T>& u2)
{
	return vector2template<T>(u.x % u2.x, u.y % u2.y);
}
