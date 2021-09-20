
`timescale 1ns/1ns

module fp_adder__tb2();

  reg [31:0] s_true, a, b;

  shortreal ar,br;
  int errors, no_of_tests;

  initial begin
  
    errors= 0;
    no_of_tests= 2000000;
    for (int i=0;i<no_of_tests;i++) begin
      #10;
      a = $random();
      b = $random();
      ar = $bitstoshortreal(a);
      br = $bitstoshortreal(b);
      s_true = $shortrealtobits(ar+ br);
      #10;
      if (uut.s != s_true && s_true[30:23] != 255 && a[30:23] != 255 && b[30:23] != 255) begin
        errors++;
        $stop;  
      end
    end

    $display("%d (%%%0d) errors are found.\n", errors, (errors*100.0+no_of_tests)/(no_of_tests));
    $stop;  
  end

  fp_adder uut( .a(a), .b(b), .s());
  
endmodule
