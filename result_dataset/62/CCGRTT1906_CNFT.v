//f0 = (not x0 and not x1 and x2 and x3) or (not x0 and x1 and not x2 and x3) or (x0 and not x1 and not x2 and not x3) or (x0 and not x1 and x2 and not x3) or (x0 and x1 and not x2 and not x3) or (x0 and x1 and x2 and not x3) or (x0 and x1 and x2 and x3)
//f1 = (not x0 and not x1 and not x2 and not x3) or (not x0 and not x1 and x2 and not x3) or (not x0 and x1 and not x2 and not x3) or (x0 and not x1 and not x2 and x3) or (x0 and not x1 and x2 and not x3) or (x0 and not x1 and x2 and x3) or (x0 and x1 and not x2 and x3) or (x0 and x1 and x2 and x3)
//f2 = (not x0 and not x1 and not x2 and not x3) or (not x0 and x1 and not x2 and x3) or (not x0 and x1 and x2 and not x3) or (not x0 and x1 and x2 and x3) or (x0 and not x1 and not x2 and x3) or (x0 and not x1 and x2 and not x3) or (x0 and not x1 and x2 and x3) or (x0 and x1 and not x2 and not x3)
//f3 = (not x0 and not x1 and not x2 and x3) or (not x0 and x1 and not x2 and x3) or (x0 and not x1 and not x2 and not x3) or (x0 and not x1 and x2 and not x3) or (x0 and x1 and not x2 and not x3) or (x0 and x1 and not x2 and x3) or (x0 and x1 and x2 and not x3)
//f4 = (not x0 and x1 and not x2 and not x3) or (not x0 and x1 and not x2 and x3) or (x0 and x1 and not x2 and not x3) or (x0 and x1 and x2 and not x3) or (x0 and x1 and x2 and x3)
//f5 = (not x0 and not x1 and not x2 and not x3) or (not x0 and not x1 and x2 and not x3) or (not x0 and not x1 and x2 and x3) or (not x0 and x1 and x2 and not x3) or (x0 and not x1 and x2 and x3) or (x0 and x1 and not x2 and x3) or (x0 and x1 and x2 and x3)
//f6 = (not x0 and x1 and not x2 and not x3) or (not x0 and x1 and not x2 and x3) or (not x0 and x1 and x2 and x3) or (x0 and not x1 and not x2 and x3) or (x0 and not x1 and x2 and x3) or (x0 and x1 and x2 and not x3) or (x0 and x1 and x2 and x3)
module CCGRTT1906_CNFT(
	input x0, x1, x2, x3,
	output f0, f1, f2, f3, f4, f5, f6
);

	wire d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31, d32, d33, d34, d35, d36, d37, d38, d39, d40, d41, d42, d43, d44, d45, d46, d47, d48, d49, d50, d51, d52, d53, d54, d55, d56, d57, d58, d59, d60, d61, d62, d63, d64, d65, d66, d67, d68, d69;

	assign f0 = d1;
	or (d1, d2, d7);
	and (d2, d3, d4);
	not (d3, x0);
	and (d4, d5, d6);
	not (d5, x1);
	and (d6, x2, x3);
	or (d7, d8, d12);
	and (d8, d3, d9);
	and (d9, x1, d10);
	and (d10, x3, d11);
	not (d11, x2);
	or (d12, d13, d17);
	and (d13, x0, d14);
	and (d14, d5, d15);
	and (d15, d11, d16);
	not (d16, x3);
	or (d17, d18, d21);
	and (d18, x0, d19);
	and (d19, d5, d20);
	and (d20, x2, d16);
	or (d21, d22, d24);
	and (d22, x0, d23);
	and (d23, x1, d15);
	or (d24, d25, d27);
	and (d25, x0, d26);
	and (d26, x1, d20);
	and (d27, x0, d28);
	and (d28, x1, d6);
	assign f1 = d29;
	or (d29, d30, d31);
	and (d30, d3, d14);
	or (d31, d32, d33);
	and (d32, d3, d19);
	or (d33, d34, d35);
	and (d34, d3, d23);
	or (d35, d36, d38);
	and (d36, x0, d37);
	and (d37, d5, d10);
	or (d38, d18, d39);
	or (d39, d40, d41);
	and (d40, x0, d4);
	or (d41, d27, d42);
	and (d42, x0, d9);
	assign f2 = d43;
	or (d43, d30, d44);
	or (d44, d8, d45);
	or (d45, d46, d47);
	and (d46, d3, d26);
	or (d47, d48, d49);
	and (d48, d3, d28);
	or (d49, d36, d50);
	or (d50, d18, d51);
	or (d51, d22, d40);
	assign f3 = d52;
	or (d52, d53, d54);
	and (d53, d3, d37);
	or (d54, d8, d55);
	or (d55, d13, d56);
	or (d56, d18, d57);
	or (d57, d22, d58);
	or (d58, d25, d42);
	assign f4 = d59;
	or (d59, d34, d60);
	or (d60, d8, d21);
	assign f5 = d61;
	or (d61, d30, d62);
	or (d62, d32, d63);
	or (d63, d2, d64);
	or (d64, d39, d46);
	assign f6 = d65;
	or (d65, d34, d66);
	or (d66, d8, d67);
	or (d67, d48, d68);
	or (d68, d36, d69);
	or (d69, d24, d40);
endmodule
