#pragma once
#include "Custom.h"
#include  <set>

namespace component {
	struct ThreePhaseSource {
		ThreePhaseSource(value_t _freq = 0, value_t _amplitude = 0, value_t _phase = 0, value_t _sigma = 5.8e7)
			: m_freq(_freq),  m_amplitude(_amplitude), m_phase(_phase), m_mentalSigma(_sigma)
		{

		}
		/*std::set<int>   m_currentDomains;
		std::set<int>	m_dielectricDomains;*/
		value_t			m_freq;
		value_t			m_amplitude;
		value_t			m_phase;
		value_t			m_mentalSigma;
	};
	struct DCSource {
		DCSource( value_t _amplitude = 0, value_t _sigma = 5.8e7)
			: m_amplitude(_amplitude), m_mentalSigma(_sigma)
		{
		}
		value_t			m_amplitude;
		/*td::set<int>   m_currentDomains;
		std::set<int>	m_dielectricDomains;*/
		value_t			m_mentalSigma;
	};

} // namespace component