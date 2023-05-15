//f0 = (not x0 and not x1 and not x2 and x3) or (not x0 and not x1 and x2 and x3) or (not x0 and x1 and not x2 and not x3) or (not x0 and x1 and not x2 and x3) or (not x0 and x1 and x2 and not x3) or (x0 and not x1 and not x2 and x3) or (x0 and x1 and not x2 and not x3) or (x0 and x1 and not x2 and x3) or (x0 and x1 and x2 and not x3) or (x0 and x1 and x2 and x3)
//f1 = (not x0 and not x1 and not x2 and not x3) or (not x0 and not x1 and x2 and x3) or (not x0 and x1 and not x2 and not x3) or (not x0 and x1 and x2 and not x3) or (x0 and not x1 and not x2 and not x3) or (x0 and not x1 and not x2 and x3) or (x0 and not x1 and x2 and not x3) or (x0 and not x1 and x2 and x3) or (x0 and x1 and not x2 and x3) or (x0 and x1 and x2 and x3)
//f2 = (not x0 and not x1 and not x2 and x3) or (not x0 and not x1 and x2 and not x3) or (not x0 and x1 and not x2 and not x3) or (not x0 and x1 and not x2 and x3) or (x0 and not x1 and not x2 and not x3) or (x0 and not x1 and not x2 and x3) or (x0 and not x1 and x2 and x3) or (x0 and x1 and not x2 and not x3) or (x0 and x1 and x2 and x3)
//f3 = (not x0 and not x1 and not x2 and not x3) or (not x0 and not x1 and x2 and x3) or (not x0 and x1 and not x2 and x3) or (x0 and not x1 and x2 and not x3) or (x0 and x1 and not x2 and not x3) or (x0 and x1 and not x2 and x3) or (x0 and x1 and x2 and not x3)
//f4 = (not x0 and not x1 and not x2 and x3) or (not x0 and not x1 and x2 and not x3) or (not x0 and x1 and x2 and not x3) or (not x0 and x1 and x2 and x3) or (x0 and not x1 and x2 and x3) or (x0 and x1 and x2 and not x3)
//f5 = (not x0 and not x1 and not x2 and not x3) or (not x0 and not x1 and not x2 and x3) or (not x0 and x1 and x2 and x3) or (x0 and not x1 and not x2 and x3) or (x0 and not x1 and x2 and not x3) or (x0 and not x1 and x2 and x3) or (x0 and x1 and not x2 and not x3) or (x0 and x1 and not x2 and x3) or (x0 and x1 and x2 and x3)
module CCGRTT1814_CNFT(
	input x0, x1, x2, x3,
	output f0, f1, f2, f3, f4, f5
);

	wire d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31, d32, d33, d34, d35, d36, d37, d38, d39, d40, d41, d42, d43, d44, d45, d46, d47, d48, d49, d50, d51, d52, d53, d54, d55, d56, d57, d58, d59, d60, d61, d62, d63, d64, d65, d66, d67, d68, d69, d70, d71, d72, d73, d74, d75, d76;

	assign f0 = d1;
	or (d1, d2, d8);
	and (d2, d3, d4);
	not (d3, x0);
	and (d4, d5, d6);
	not (d5, x1);
	and (d6, d7, x3);
	not (d7, x2);
	or (d8, d9, d12);
	and (d9, d3, d10);
	and (d10, d5, d11);
	and (d11, x2, x3);
	or (d12, d13, d17);
	and (d13, d3, d14);
	and (d14, x1, d15);
	and (d15, d7, d16);
	not (d16, x3);
	or (d17, d18, d20);
	and (d18, d3, d19);
	and (d19, x1, d6);
	or (d20, d21, d24);
	and (d21, d3, d22);
	and (d22, x1, d23);
	and (d23, x2, d16);
	or (d24, d25, d26);
	and (d25, x0, d4);
	or (d26, d27, d28);
	and (d27, x0, d14);
	or (d28, d29, d30);
	and (d29, x0, d19);
	or (d30, d31, d32);
	and (d31, x0, d22);
	and (d32, x0, d33);
	and (d33, x1, d11);
	assign f1 = d34;
	or (d34, d35, d37);
	and (d35, d3, d36);
	and (d36, d5, d15);
	or (d37, d9, d38);
	or (d38, d13, d39);
	or (d39, d21, d40);
	or (d40, d41, d42);
	and (d41, x0, d36);
	or (d42, d25, d43);
	or (d43, d44, d46);
	and (d44, x0, d45);
	and (d45, d5, d23);
	or (d46, d47, d48);
	and (d47, x0, d10);
	or (d48, d29, d32);
	assign f2 = d49;
	or (d49, d2, d50);
	or (d50, d51, d52);
	and (d51, d3, d45);
	or (d52, d13, d53);
	or (d53, d18, d54);
	or (d54, d41, d55);
	or (d55, d25, d56);
	or (d56, d47, d57);
	or (d57, d27, d32);
	assign f3 = d58;
	or (d58, d35, d59);
	or (d59, d9, d60);
	or (d60, d18, d61);
	or (d61, d44, d62);
	or (d62, d27, d63);
	or (d63, d29, d31);
	assign f4 = d64;
	or (d64, d2, d65);
	or (d65, d51, d66);
	or (d66, d21, d67);
	or (d67, d68, d69);
	and (d68, d3, d33);
	or (d69, d31, d47);
	assign f5 = d70;
	or (d70, d35, d71);
	or (d71, d2, d72);
	or (d72, d68, d73);
	or (d73, d25, d74);
	or (d74, d44, d75);
	or (d75, d47, d76);
	or (d76, d27, d48);
endmodule
