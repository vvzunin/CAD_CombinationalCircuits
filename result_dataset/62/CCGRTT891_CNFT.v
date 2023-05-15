//f0 = (not x0 and not x1 and not x2) or (not x0 and x1 and not x2) or (not x0 and x1 and x2) or (x0 and not x1 and not x2) or (x0 and not x1 and x2)
//f1 = (not x0 and not x1 and x2) or (not x0 and x1 and not x2) or (x0 and not x1 and x2) or (x0 and x1 and not x2)
//f2 = (not x0 and x1 and not x2) or (not x0 and x1 and x2) or (x0 and not x1 and not x2) or (x0 and x1 and x2)
module CCGRTT891_CNFT(
	input x0, x1, x2,
	output f0, f1, f2
);

	wire d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17, d18, d19, d20, d21, d22, d23, d24, d25;

	assign f0 = d1;
	or (d1, d2, d7);
	and (d2, d3, d4);
	not (d3, x0);
	and (d4, d5, d6);
	not (d5, x1);
	not (d6, x2);
	or (d7, d8, d10);
	and (d8, d3, d9);
	and (d9, x1, d6);
	or (d10, d11, d13);
	and (d11, d3, d12);
	and (d12, x1, x2);
	or (d13, d14, d15);
	and (d14, x0, d4);
	and (d15, x0, d16);
	and (d16, d5, x2);
	assign f1 = d17;
	or (d17, d18, d19);
	and (d18, d3, d16);
	or (d19, d8, d20);
	or (d20, d15, d21);
	and (d21, x0, d9);
	assign f2 = d22;
	or (d22, d8, d23);
	or (d23, d11, d24);
	or (d24, d14, d25);
	and (d25, x0, d12);
endmodule
