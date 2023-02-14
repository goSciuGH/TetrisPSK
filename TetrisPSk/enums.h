#pragma once

/// Ten enum scoring posiada zawiera wartoœci punktacyjne ró¿nych ruchów w grze - czyszczenia linii, T-Spinów, Combo, Dropów i Perfect Cleara.
enum scoring
{
	SNGL = 100,		///< Wartoœæ punktowa Single
	DUBL = 300,		///< Wartoœæ punktowa Double
	TRPL = 500,		///< Wartoœæ punktowa Triple
	QUAD = 800,		///< Wartoœæ punktowa Quad
	TSMN = 100,		///< Wartoœæ punktowa T-Spin Mini (bez czyszczenia)
	TSPN = 400,		///< Wartoœæ punktowa T-Spin (bez czyszczenia)
	TSMS = 200,		///< Wartoœæ punktowa T-Spin Mini Single
	TSSN = 800,		///< Wartoœæ punktowa T-Spin Single
	TSMD = 1200,	///< Wartoœæ punktowa T-Spin Mini Double
	TSDB = 1200,	///< Wartoœæ punktowa T-Spin Double
	TSTR = 1600,	///< Wartoœæ punktowa T-Spin Triple
	CMBO = 50,		///< Wartoœæ punktowa Combo (mno¿one przez d³ugoœæ combo)
	SDRP = 1,		///< Wartoœæ punktowa Soft Drop (1 pole)
	HDRP = 2,		///< Wartoœæ punktowa Hard Drop (2 pole)
	PCLR = 3500		///< Wartoœæ punktowa Perfect Clear
};