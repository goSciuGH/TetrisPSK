#pragma once

/// Typ wyliczeniowy scoring
///
/// Enum scoring posiada zawiera wartoœci punktacyjne ró¿nych ruchów w grze - czyszczenia linii, T-Spinów, Combo, Dropów i Perfect Cleara.
enum scoring
{
	SNGL = 100,		// Single
	DUBL = 300,		// Double
	TRPL = 500,		// Triple
	QUAD = 800,		// Quad
	TSMN = 100,		// T-Spin Mini (bez czyszczenia)
	TSPN = 400,		// T-Spin (bez czyszczenia)
	TSMS = 200,		// T-Spin Mini Single
	TSSN = 800,		// T-Spin Single
	TSMD = 1200,	// T-Spin Mini Double
	TSDB = 1200,	// T-Spin Double
	TSTR = 1600,	// T-Spin Triple
	CMBO = 50,		// Combo (mno¿one przez d³ugoœæ combo)
	SDRP = 1,		// Soft Drop (1 pole)
	HDRP = 2,		// Hard Drop (2 pole)
	PCLR = 3500		// Perfect Clear
};