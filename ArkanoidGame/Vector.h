#pragma once
#include <cmath>

namespace ArkanoidGame
{
	// вектор с иницилизацией полей 
	template<typename T>
	struct Vector2D
	{
		T x = (T)0;//иницилизация при обьявление
		T y = (T)0;
	};

	using Vector2Df = Vector2D<float>;
	using Vector2Di = Vector2D<int>;
	using Position = Vector2Df;
	// перегрузка оп.сложения

	template<typename T>
	Vector2D<T> operator+(const Vector2D<T>& lhs, const Vector2D<T>& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y };
	}
	// перегрузка оп.вычитания

	template<typename T>
	Vector2D<T> operator-(const Vector2D<T>& lhs, const Vector2D<T>& rhs)
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y };
	}
	// перегрузка оп.сравнения
	template<typename T>
	bool operator==(const Vector2D<T>& lhs, const Vector2D<T>& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	// длина вектора
	template<typename T>
	float GetVectorLength(const Vector2D<T>& vector)
	{
		return sqrtf(vector.x * vector.x + vector.y * vector.y);
	}

	template<typename U, typename V>
	U Convert(const V& v)
	{
		return { static_cast<decltype(U::x)>(v.x), static_cast<decltype(U::y)>(v.y) };
	}
}