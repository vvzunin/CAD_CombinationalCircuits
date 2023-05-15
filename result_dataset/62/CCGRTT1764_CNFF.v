//f0 = (x0 or x1 or x2 or x3) and (x0 or x1 or x2 or not x3) and (x0 or not x1 or x2 or x3) and (x0 or not x1 or x2 or not x3) and (x0 or not x1 or not x2 or x3) and (not x0 or x1 or x2 or x3) and (not x0 or x1 or not x2 or x3) and (not x0 or not x1 or x2 or not x3) and (not x0 or not x1 or not x2 or x3)
//f1 = (x0 or x1 or x2 or x3) and (x0 or x1 or not x2 or x3) and (x0 or not x1 or x2 or not x3) and (not x0 or x1 or x2 or x3) and (not x0 or x1 or x2 or not x3) and (not x0 or x1 or not x2 or not x3) and (not x0 or not x1 or x2 or x3) and (not x0 or not x1 or x2 or not x3) and (not x0 or not x1 or not x2 or x3)
//f2 = (x0 or x1 or not x2 or x3) and (x0 or x1 or not x2 or not x3) and (x0 or not x1 or not x2 or x3) and (not x0 or x1 or x2 or x3) and (not x0 or x1 or not x2 or not x3) and (not x0 or not x1 or not x2 or not x3)
//f3 = (x0 or x1 or not x2 or not x3) and (x0 or not x1 or x2 or not x3) and (x0 or not x1 or not x2 or not x3) and (not x0 or x1 or x2 or not x3) and (not x0 or x1 or not x2 or x3) and (not x0 or not x1 or x2 or not x3) and (not x0 or not x1 or not x2 or not x3)
//f4 = (x0 or x1 or x2 or x3) and (x0 or x1 or not x2 or x3) and (x0 or x1 or not x2 or not x3) and (x0 or not x1 or x2 or not x3) and (x0 or not x1 or not x2 or x3) and (not x0 or x1 or x2 or x3) and (not x0 or x1 or not x2 or x3) and (not x0 or x1 or not x2 or not x3) and (not x0 or not x1 or x2 or x3) and (not x0 or not x1 or not x2 or not x3)
module CCGRTT1764_CNFF(
	input x0, x1, x2, x3,
	output f0, f1, f2, f3, f4
);

	wire d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31, d32, d33, d34, d35, d36, d37, d38, d39, d40, d41, d42, d43, d44, d45, d46, d47, d48, d49, d50, d51, d52, d53, d54, d55, d56, d57, d58, d59, d60, d61, d62, d63, d64, d65, d66, d67;

	assign f0 = d1;
	and (d1, d2, d5);
	or (d2, x0, d3);
	or (d3, x1, d4);
	or (d4, x2, x3);
	and (d5, d6, d10);
	or (d6, x0, d7);
	or (d7, x1, d8);
	or (d8, x2, d9);
	not (d9, x3);
	and (d10, d11, d14);
	or (d11, x0, d12);
	or (d12, d4, d13);
	not (d13, x1);
	and (d14, d15, d17);
	or (d15, x0, d16);
	or (d16, d8, d13);
	and (d17, d18, d22);
	or (d18, x0, d19);
	or (d19, d13, d20);
	or (d20, x3, d21);
	not (d21, x2);
	and (d22, d23, d25);
	or (d23, d3, d24);
	not (d24, x0);
	and (d25, d26, d28);
	or (d26, d24, d27);
	or (d27, x1, d20);
	and (d28, d29, d30);
	or (d29, d16, d24);
	or (d30, d19, d24);
	assign f1 = d31;
	and (d31, d2, d32);
	and (d32, d33, d34);
	or (d33, x0, d27);
	and (d34, d15, d35);
	and (d35, d23, d36);
	and (d36, d37, d38);
	or (d37, d7, d24);
	and (d38, d39, d42);
	or (d39, d24, d40);
	or (d40, x1, d41);
	or (d41, d9, d21);
	and (d42, d28, d43);
	or (d43, d12, d24);
	assign f2 = d44;
	and (d44, d33, d45);
	and (d45, d46, d47);
	or (d46, x0, d40);
	and (d47, d18, d48);
	and (d48, d23, d49);
	and (d49, d39, d50);
	or (d50, d24, d51);
	or (d51, d13, d41);
	assign f3 = d52;
	and (d52, d46, d53);
	and (d53, d15, d54);
	and (d54, d55, d56);
	or (d55, x0, d51);
	and (d56, d37, d57);
	and (d57, d26, d58);
	and (d58, d29, d50);
	assign f4 = d59;
	and (d59, d2, d60);
	and (d60, d33, d61);
	and (d61, d46, d62);
	and (d62, d15, d63);
	and (d63, d18, d64);
	and (d64, d23, d65);
	and (d65, d26, d66);
	and (d66, d39, d67);
	and (d67, d43, d50);
endmodule
