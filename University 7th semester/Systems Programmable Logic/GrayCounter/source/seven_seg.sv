module sevenseg_decoder
    #(
        parameter WIDTH
    )(
        input  logic [WIDTH - 1 : 0 ] VALUE,
        output logic [6         : 0 ] SEG_OUT
    );
    
    always_comb 
    begin
        case(VALUE)
            3'd0: SEG_OUT    = 7'b1000000; // 0
            3'd1: SEG_OUT    = 7'b1111001; // 1
            3'd2: SEG_OUT    = 7'b0100100; // 2
            3'd3: SEG_OUT    = 7'b0110000; // 3
            3'd4: SEG_OUT    = 7'b0011001; // 4
            3'd5: SEG_OUT    = 7'b0010010; // 5
            3'd6: SEG_OUT    = 7'b0000010; // 6
            3'd7: SEG_OUT    = 7'b1111000; // 7
            default: SEG_OUT = 7'b0000000; // All ON
         endcase
    end

endmodule : sevenseg_decoder