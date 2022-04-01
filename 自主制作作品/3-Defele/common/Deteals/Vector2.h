#include "..\Vector2.h"
#pragma once

template<class T>
Vector2Template<T>::Vector2Template()
{
	x_ = 0;
	y_ = 0;
}

template<class T>
Vector2Template<T>::Vector2Template(T x, T y)
{
	x_ = x;
	y_ = y;
}

template<class T>
Vector2Template<T>::~Vector2Template()
{
}

template<class T>
Vector2Template<T>& Vector2Template<T>::operator=(const Vector2Template<T>& vec)
{
	x_ = vec.x_;
	y_ = vec.y_;
	return (*this);		//éQè∆ìnÇµÇÃÇΩÇﬂ*ÇÇ¬ÇØÇÈ
}

template<class T>
int& Vector2Template<T>::operator[](int no)
{
	if (no == 0)
	{
		return x_;
	}
	else if (no == 1)
	{
		return y_;
	}
	else
	{
		return x_;
	}
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator+=(const Vector2Template<T>& vec)
{
	x_ += vec.x_;
	y_ += vec.y_;
	return (*this);
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator-=(const Vector2Template<T>& vec)
{
	x_ -= vec.x_;
	y_ -= vec.y_;
	return (*this);
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator*=(const Vector2Template<T>& vec)
{
	x_ *= vec.x_;
	y_ *= vec.y_;
	return (*this);
}

template<class T>
Vector2Template<T> Vector2Template <T>::operator/=(const Vector2Template<T>& vec)
{
	//0äÑÇËñhé~
	if (vec.x_ == 0)
	{
		x_ = 0;
	}
	else
	{
		x_ /= vec.x_;
	}
	if (vec.y_ == 0)
	{
		y_ = 0;
	}
	else
	{
		y_ /= vec.y_;
	}
	return (*this);
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator%=(const Vector2Template<T>& vec)
{
	//0äÑÇËñhé~
	if (vec.x_ == 0)
	{
		x_ = 0;
	}
	else
	{
		x_ %= vec.x_;
	}
	if (vec.y_ == 0)
	{
		y_ = 0;
	}
	else
	{
		y_ %= vec.y_;
	}
	return (*this);
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator+() const
{
	return *this;
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator-() const
{
	return { -x_, -y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator+(const Vector2Template<T> vec)
{
	return { x_ + vec.x_, y_ + vec.y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator-(const Vector2Template<T> vec)
{
	return { x_ - vec.x_, y_ - vec.y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator*(const Vector2Template<T> vec)
{
	return { x_ * vec.x_, y_ * vec.y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator/(const Vector2Template<T> vec)
{
	if (vec.x_ == 0 || vec.y_ == 0)
	{
		//0äÑÇËñhé~
		return { 0,0 };
	}
	return { x_ / vec.x_, y_ / vec.y_ };
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator%(const Vector2Template<T> vec)
{
	if (vec.x_ == 0 || vec.y_ == 0)
	{
		//0äÑÇËñhé~
		return { 0,0 };
	}
	return { x_ % vec.x_, y_ % vec.y_ };
}

template<class T>
bool Vector2Template<T>::operator==(const Vector2Template<T>& vec) const
{
	if (x_ == vec.x_ && y_ == vec.y_)
	{
		return true;
	}
	return false;
}

template<class T>
bool Vector2Template<T>::operator!=(const Vector2Template<T>& vec) const
{
	if (x_ != vec.x_ || y_ != vec.y_)
	{
		return true;
	}
	return false;
}

template<class T>
bool Vector2Template<T>::operator<(const Vector2Template<T>& vec) const
{
	if (x_ < vec.x_ && y_ < vec.y_)
	{
		return true;
	}
	return false;
}

template<class T>
bool Vector2Template<T>::operator>(const Vector2Template<T>& vec) const
{
	if (x_ > vec.x_ && y_ > vec.y_)
	{
		return true;
	}
	return false;
}

template<class T>
bool Vector2Template<T>::operator<=(const Vector2Template<T>& vec) const
{
	if (x_ <= vec.x_ && y_ <= vec.y_)
	{
		return true;
	}
	return false;
}

template<class T>
bool Vector2Template<T>::operator>=(const Vector2Template<T>& vec) const
{
	if (x_ >= vec.x_ && y_ >= vec.y_)
	{
		return true;
	}
	return false;
}

template<class T>
inline float Vector2Template<T>::Magnitude(void) const
{
	return hypotf(x_, y_);
}

template<class T>
inline float Vector2Template<T>::SQMagnitude(void) const
{
	return x_ * x_ + y_ * y_;
}

template<class T>
inline void Vector2Template<T>::Normalize(void)
{
	float mag = Magnitude();
	x_ /= mag;
	y_ /= mag;
}

template<class T>
inline Vector2Template<T> Vector2Template<T>::Normalized(void)
{
	float mag = Magnitude();
	return Vector2Template<T>(x_ / mag, y_ / mag);
}

template<class T>
Vector2Template<T> operator*(const Vector2Template<T>& vec, const T inte)
{
	return Vector2Template<T>(vec.x_ * inte, vec.y_ * inte);
}

template<class T>
Vector2Template<T> operator*(const T inte, const Vector2Template<T>& vec)
{
	return Vector2Template<T>(vec.x_ * inte, vec.y_ * inte);
}

template<class T>
inline Vector2Template<T> operator/(const Vector2Template<T>& vec, const T inte)
{
	if (inte == 0)
	{
		return vec;
	}
	return Vector2Template<T>(vec.x_ / inte, vec.y_ / inte);
}

