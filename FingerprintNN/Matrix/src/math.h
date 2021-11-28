#pragma once

#include "Vector.h"
#include "Matrix.h"


template<typename T>
static double dot(const Vector<T>& a, const Vector<T>& b)
{
	if (a.m_size != b.m_size)
		throw std::runtime_error("vectors sizes don\'t match");
	double out = 0;

	for (int i = 0; i < a.m_size; ++i)
		out += a.m_elements[i] * b.m_elements[i];
	return out;
}

template<typename T>
static Matrix<T> applyFunction(const Matrix<T>& mat, float(*func)(float))
{
	matf out(mat.width(), mat.height());
	for (int y = 0; y < mat.height(); ++y)
		for (int x = 0; x < mat.width(); ++x)
			out(x, y) = func(mat(x, y));
	return out;
}



//template<typename T>
//static double dot(const Vector<T>& a, const Vector<T>& b)
//{
//	if (a.m_size != b.m_size)
//		throw std::runtime_error("vectors sizes don\'t match");
//	double out = 0;
//
//	for (int i = 0; i < a.m_size; ++i)
//		out += a.m_elements[i] * b.m_elements[i];
//	return out;
//}