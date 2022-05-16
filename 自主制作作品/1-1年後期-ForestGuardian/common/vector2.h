#pragma once
template<class T> class vector2template
{
public:
	vector2template();
	vector2template(T x, T y);
	~vector2template();
	T x;
	T y;

	//������Z�q
	vector2template& operator = (const vector2template& vec);

	//�Y�������Z�q
	int& operator [] (int no);

	//�P�����Z�q
	vector2template& operator +=(const vector2template& vec);
	vector2template& operator -=(const vector2template& vec);
	vector2template& operator *=(const vector2template& vec);
	vector2template& operator /=(const vector2template& vec);

	vector2template& operator +=(T vec);
	vector2template& operator -=(T vec);
	vector2template& operator *=(T vec);
	vector2template& operator /=(T vec);

	vector2template operator +()const;
	vector2template operator -()const;

	//��r���Z�q
	bool operator ==(const vector2template& vec)const;
	bool operator !=(const vector2template& vec)const;
	/*bool operator <(const vector2& vec)const;
	bool operator <=(const vector2& vec)const;*/

};

// vector2template + T(�^����)
template<class T>
vector2template<T> operator +(const vector2template<T>& u, T v);

// vector2template - T(�^����)
template<class T>
vector2template<T> operator -(const vector2template<T>& u, T v);

// vector2template * T(�^����)
template<class T>
vector2template<T> operator *(const vector2template<T>& u, T v);

// vector2template / T(�^����)
template<class T>
vector2template<T> operator /(const vector2template<T>& u, T v);

// vector2template % int
template<class T>
vector2template<T> operator %(const vector2template<T>& u, int v);

// vector2 + vector2
template<class T>
vector2template<T> operator +(const vector2template<T>& u, const vector2template<T>& u2);

// vector2 - vector2
template<class T>
vector2template<T> operator -(const vector2template<T>& u, const vector2template<T>& u2);

// vector2 * vector2
template<class T>
vector2template<T> operator *(const vector2template<T>& u, const vector2template<T>& u2);

// vector2 / vector2
template<class T>
vector2template<T> operator /(const vector2template<T>& u, const vector2template<T>& u2);

// vector2 / vector2
template<class T>
vector2template<T> operator %(const vector2template<T>& u, const vector2template<T>& u2);


using Vector2 = vector2template<int>;
using Vector2double = vector2template<double>;
using Vector2float = vector2template<float>;

#include"deteals/vector2.h"
