#pragma once
#include "Custom.h"

namespace component {
	struct ThreePhaseSource {
		ThreePhaseSource(value_t _freq = 0, value_t _amplitude = 0, value_t _phase = 0 )
			: m_freq(_freq),  m_amplitude(_amplitude), m_phase(_phase)
		{
		}
		value_t     m_freq;
		value_t     m_amplitude;
		value_t     m_phase;
	
	};
	struct DCSource {
		DCSource( value_t _amplitude = 0)
			: m_amplitude(_amplitude)
		{
		}
		value_t     m_amplitude;
	};

} // namespace component