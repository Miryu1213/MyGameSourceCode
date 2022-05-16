#pragma once

template <class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();

	T x_;
	T y_;
	
	//Vector2�I�y���[�^�[�I�[�o�[���[�h
	 
	//������Z�q
	Vector2Template& operator =(const Vector2Template& vec);
	
	//�Y�������Z�q
	int& operator [](int no);

	//�P�����Z�q
	Vector2Template operator +=(const Vector2Template& vec);
	Vector2Template operator -=(const Vector2Template& vec);
	Vector2Template operator *=(const Vector2Template& vec);
	Vector2Template operator /=(const Vector2Template& vec);
	Vector2Template operator %=(const Vector2Template& vec);
	Vector2Template operator +() const;
	Vector2Template operator -() const;

	//�񍀉��Z�q
	Vector2Template operator +(const Vector2Template vec);
	Vector2Template operator -(const Vector2Template vec);
	Vector2Template operator *(const Vector2Template vec);
	Vector2Template operator /(const Vector2Template vec);
	Vector2Template operator %(const Vector2Template vec);

	//��r���Z�q
	bool operator ==(const Vector2Template& vec) const;
	bool operator !=(const Vector2Template& vec) const;
	bool operator <(const Vector2Template& vec) const;
	bool operator >(const Vector2Template& vec) const;
	bool operator <=(const Vector2Template& vec) const;
	bool operator >=(const Vector2Template& vec) const;

	//���w�֌W
	///�x�N�g���̑傫��
	float Magnitude(void)const;

	//�傫�����悵�đ������킹������
	float SQMagnitude(void)const;

	///���K��(�傫�����P��)
	void Normalize(void);

	///���K���x�N�g��
	Vector2Template Normalized(void);
};

//�@T�Ƃ����N���X�����邱�Ƃ�錾���Ȃ���΂����Ȃ�
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