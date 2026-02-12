`timescale 1ns/1ps

//  Module: gray_counter
module gray_counter
    #(
        parameter WIDTH
    )(
        input logic i_clk,
        input logic i_rst,
        input logic i_man_clk,
        input logic i_man_clk_en,
        input logic i_dir,

        output logic [WIDTH - 1 : 0] o_gray
    );

    localparam COUNTER_MAX  = (1 << WIDTH) - 1;

    logic [WIDTH - 1 : 0] gray_next;
    logic [WIDTH - 1 : 0] gray_curr;
    logic [WIDTH - 1 : 0] counter;
    logic clk;

    assign clk    = i_man_clk_en ? i_man_clk : i_clk;
    assign o_gray = gray_curr;

    always_comb 
    begin
        for(int i = 0; i < WIDTH - 1; ++i)
        begin
            gray_next[i] = counter[i] ^ counter[i+1];
        end
        gray_next[WIDTH - 1] = counter[WIDTH - 1]; 
    end

    always_ff @(posedge clk or posedge i_rst) begin
        if (i_rst)
        begin
            gray_curr   <= '0;
            counter <= '0;
        end
        else if (1'b1 == i_dir)
        begin
            counter <= (COUNTER_MAX == counter) ? '0 : (counter + 1);
            gray_curr    <= gray_next;
        end
        else
        begin
            counter <= ('0 == counter) ? COUNTER_MAX : (counter - 1);
            gray_curr    <= gray_next;
        end
    end

endmodule: gray_counter
