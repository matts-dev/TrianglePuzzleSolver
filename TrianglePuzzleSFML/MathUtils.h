#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace MathUtils
{
	inline float distanceBetweenPoints(glm::vec2& pnt_a, glm::vec2& pnt_b)
	{
		glm::vec2 b_to_a = pnt_b - pnt_a;
		return glm::length(b_to_a);
	}

	template<typename T>
	inline bool almostEqual(T a, T b, T epsilon)
	{
		return abs(a - b) < epsilon;
	}
}