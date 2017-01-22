# wisp_ecc

Useful links:

ECC calculator
http://www.christelbach.com/eccalculator.aspx

Modulo calculator
http://www.miniwebtool.com/modulo-calculator/

Formules uit ECC in Matlab

voor y^2 = ax^3 + bx^2 + cx + d

P1 # P2 =	P1 : als P2 = O
		P2 : als P1 = O
		O  : als x1 = x2 en y1 != y2 
		O  : als x1 = x2 en y1 = y2 = 0
		P3 : overig
P3

x3 = -x1 -x2 - b * a^-1 + m^2 * a^-1
y3 = -y1 + m * (x1 - x3)

m =	(y2 -y1) * (x2 - x1) 				: als P1 != P2
	(3 * a * x1^2 + 2 * b * x1 + c) * 2^-1 * y1^-1	: als P1 = P2

source: https://ir.canterbury.ac.nz/bitstream/handle/10092/10163/silverwood_2007_report.pdf?sequence=1

