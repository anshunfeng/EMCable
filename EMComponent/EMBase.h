#pragma once

namespace FEM {

	class EMBase {
	public:
		EMBase() { }
		virtual ~EMBase() { }

		EMBase(const EMBase&) = delete;
		EMBase& operator=(const EMBase&) = delete;
	};

} // namespace mom