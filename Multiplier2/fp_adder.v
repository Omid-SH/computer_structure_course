module fp_adder(
	input [31:0] a,b,
	output [31:0] s
	);
//..................................
wire [22:0] fra_a , fra_b;                // for fraction part
wire [7:0] exp_a, exp_b, exp_a1 , exp_b1; // for exponent part
wire sign_a , sign_b;                     // for sign

assign fra_a = a[22:0];
assign fra_b = b[22:0];
assign exp_a = a[30:23];
assign exp_b = b[30:23];
assign sign_a = a[31];
assign sign_b = b[31];
//..................................
wire [25:0] unpacked_a, unpacked_b; // fraction with hidden bit and GR

assign unpacked_a = |exp_a ? {1'b1,fra_a,2'b00} : {1'b0,fra_a,2'b00};
assign unpacked_b = |exp_b ? {1'b1,fra_b,2'b00} : {1'b0,fra_b,2'b00};
assign exp_a1 = |exp_a ? exp_a : 8'b00000001;
assign exp_b1 = |exp_b ? exp_b : 8'b00000001;
//....................................
wire borrow;
wire [7:0] exp_diff; // abs value of exponents difference

assign exp_diff = (exp_a1 > exp_b1) ? exp_a1 - exp_b1 : exp_b1 - exp_a1;
assign borrow = (exp_b1 > exp_a1);
//.....................................
wire [25:0] X, Y;       // X is Smaller number and Y is Bigger one
wire sign_X , sign_Y;

assign X = borrow ? unpacked_a : unpacked_b;
assign sign_X = borrow ? sign_a : sign_b;
assign Y = borrow ? unpacked_b : unpacked_a;
assign sign_Y = borrow ? sign_b : sign_a;
//.......................................
wire [25:0] shifted_X1, shifted_X2; // tmp
wire [28:0] new_X, new_Y; 	    // X and Y with Sticky, one bit for overflow and finally one bit for sign ( 2's complement )
wire [28:0] adder_result;           // result of Big ALU
wire sticky;

assign shifted_X1 = X >> exp_diff;
assign shifted_X2 = shifted_X1 << exp_diff;
assign sticky = |(X - shifted_X2);
assign new_X = sign_X ? ~{2'b00,shifted_X1,sticky} + 1 : {2'b00,shifted_X1,sticky};
assign new_Y = sign_Y ? ~{2'b00 ,Y, 1'b0} + 1 : {2'b00 ,Y, 1'b0};
assign adder_result = new_X + new_Y;
//..........................................................................................
wire adder_result_sign ; // sign of result
wire [27:0] abs_val;     // abs of sum result

assign adder_result_sign = adder_result [28];
assign abs_val = adder_result_sign ? ~adder_result + 1 : adder_result[27:0];
//...........................................................................................
wire [7:0]nom; // first 1 position

assign nom =	 abs_val[27] ? 1
		: abs_val[26] ? 2
		: abs_val[25] ? 3
		: abs_val[24] ? 4
		: abs_val[23] ? 5
		: abs_val[22] ? 6
  		: abs_val[21] ? 7
		: abs_val[20] ? 8
		: abs_val[19] ? 9
		: abs_val[18] ? 10
		: abs_val[17] ? 11
		: abs_val[16] ? 12
		: abs_val[15] ? 13
		: abs_val[14] ? 14
		: abs_val[13] ? 15
		: abs_val[12] ? 16
		: abs_val[11] ? 17
		: abs_val[10] ? 18
		: abs_val[9] ? 19
		: abs_val[8] ? 20
		: abs_val[7] ? 21
		: abs_val[6] ? 22
		: abs_val[5] ? 23
		: abs_val[4] ? 24
		: abs_val[3] ? 25
		: abs_val[2] ? 26
		: abs_val[1] ? 27
		: abs_val[0] ? 28
		: 8'b11111111;

//.......................................................................
wire [27:0] shifted_abs_val; // shifted value based on bigger number exponent and first 1 in fraction
wire [7:0] exponent; 	     // exponent of bigger number ( we could even use Y_exponent )
wire [7:0] new_exponent;

assign exponent = borrow ? exp_b1: exp_a1;
assign shifted_abs_val = ( exponent > (nom-2) || nom==1) ? abs_val << nom : abs_val << exponent + 1;
assign new_exponent = ( exponent > (nom-2) || nom==1 ) ?  exponent - nom + 2 : 8'b00000000;
//.......................................................................
wire [23:0] rounded , tmp;  // tmp
wire [22:0] f_rounded;      // final fraction
wire [7:0] f_exp;	    // final exponent

assign tmp = {1'b0,shifted_abs_val[27:5]};
assign rounded = shifted_abs_val[4] ? (shifted_abs_val[3] | shifted_abs_val[2] | shifted_abs_val[1] | shifted_abs_val[0]) ? tmp+1 : shifted_abs_val[5] ? tmp+1 : tmp : tmp ;
assign f_rounded = rounded[22:0];
assign f_exp = rounded[23] ? new_exponent + 1 : new_exponent;
assign s = {adder_result_sign , f_exp , f_rounded};

endmodule


