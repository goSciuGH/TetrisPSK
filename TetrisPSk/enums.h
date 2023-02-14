#pragma once

/// Ten enum scoring posiada zawiera warto�ci punktacyjne r�nych ruch�w w grze - czyszczenia linii, T-Spin�w, Combo, Drop�w i Perfect Cleara.
enum scoring
{
	SNGL = 100,		///< Warto�� punktowa Single
	DUBL = 300,		///< Warto�� punktowa Double
	TRPL = 500,		///< Warto�� punktowa Triple
	QUAD = 800,		///< Warto�� punktowa Quad
	TSMN = 100,		///< Warto�� punktowa T-Spin Mini (bez czyszczenia)
	TSPN = 400,		///< Warto�� punktowa T-Spin (bez czyszczenia)
	TSMS = 200,		///< Warto�� punktowa T-Spin Mini Single
	TSSN = 800,		///< Warto�� punktowa T-Spin Single
	TSMD = 1200,	///< Warto�� punktowa T-Spin Mini Double
	TSDB = 1200,	///< Warto�� punktowa T-Spin Double
	TSTR = 1600,	///< Warto�� punktowa T-Spin Triple
	CMBO = 50,		///< Warto�� punktowa Combo (mno�one przez d�ugo�� combo)
	SDRP = 1,		///< Warto�� punktowa Soft Drop (1 pole)
	HDRP = 2,		///< Warto�� punktowa Hard Drop (2 pole)
	PCLR = 3500		///< Warto�� punktowa Perfect Clear
};