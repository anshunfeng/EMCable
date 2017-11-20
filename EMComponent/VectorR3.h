#pragma once
#include "Custom.h"

namespace component {

	class VectorR3 {
//���Ա
	public:
		value_t x, y, w; 
//����
	public:
		VectorR3() :x(0.0), y(0.0), w(0.0) {}
		VectorR3(const VectorR3&) = default;
		VectorR3(VectorR3&&) = default;
		~VectorR3() = default;
	public:
		VectorR3& operator=(const VectorR3&) = default;
		VectorR3& operator=(VectorR3&&) = default;

	};

} 