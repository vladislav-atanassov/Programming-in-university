`timescale 1ns/1ps

//  Module: clock_divider
//
module clock_divider
    #(
        parameter NEW_FREQ_HZ,
        parameter DEFAULT_FREQ_HZ
    )(
        input logic i_clk,
        input logic i_rst,

        output logic o_clk
    );

    localparam COUNTER_SIZE = $clog2(DEFAULT_FREQ_HZ / NEW_FREQ_HZ);
    localparam COUNTER_MAX  = (1 << COUNTER_SIZE) - 1;
    
    logic [COUNTER_SIZE - 1 : 0] counter;

    always_ff @(posedge i_clk)
    begin
        if (1'b1 == i_rst)
        begin
            o_clk <= 1'b0;
        end
        else
        begin
            if (COUNTER_MAX > counter)
            begin
                counter <= counter + 1;
                o_clk   <= 1'b0;
            end
            else
            begin
                counter <= '0;
                o_clk   <= 1'b1;
            end
        end    
    end
    
endmodule: clock_divider
