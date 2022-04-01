#pragma once

template <class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();

	T x_;
	T y_;
	
	//Vector2オペレーターオーバーロード
	 
	//代入演算子
	Vector2Template& operator =(const Vector2Template& vec);
	
	//添え字演算子
	int& operator [](int no);

	//単項演算子
	Vector2Template operator +=(const Vector2Template& vec);
	Vector2Template operator -=(const Vector2Template& vec);
	Vector2Template operator *=(const Vector2Template& vec);
	Vector2Template operator /=(const Vector2Template& vec);
	Vector2Template operator %=(const Vector2Template& vec);
	Vector2Template operator +() const;
	Vector2Template operator -() const;

	//二項演算子
	Vector2Template operator +(const Vector2Template vec);
	Vector2Template operator -(const Vector2Template vec);
	Vector2Template operator *(const Vector2Template vec);
	Vector2Template operator /(const Vector2Template vec);
	Vector2Template operator %(const Vector2Template vec);

	//比較演算子
	bool operator ==(const Vector2Template& vec) const;
	bool operator !=(const Vector2Template& vec) const;
	bool operator <(const Vector2Template& vec) const;
	bool operator >(const Vector2Template& vec) const;
	bool operator <=(const Vector2Template& vec) const;
	bool operator >=(const Vector2Template& vec) const;

	//数学関係
	///ベクトルの大きさ
	float Magnitude(void)const;

	//大きさを二乗して足し合わせたもの
	float SQMagnitude(void)const;

	///正規化(大きさを１に)
	void Normalize(void);

	///正規化ベクトル
	Vector2Template Normalized(void);
};

//　Tというクラスがあることを宣言しなければいけない
template<class T>
Vector2Template<T> operator *(const Vector2Template<T>& vec, const T inte);

template<class T>
Vector2Template<T> operator *(const T inte, const Vector2Template<T>& vec);

template<class T>
Vector2Template<T> operator /(const Vector2Template<T>& vec, const T inte);

using Vector2 = Vector2Template<int>;
using Vector2Flt = Vector2Template<float>;
using Vector2Dbl = Vector2Template<double>;

#include"Deteals/Vector2.h"