`timescale 1ns / 1ps

`include "top.sv"
import top_params_pkg::*;

module top_tb;

logic             i_clk;
logic             i_rst;
logic             i_man_clk;
logic             i_man_clk_en;
logic             i_dir;
logic [6:0]       SEG_OUT;
logic [WIDTH-1:0] o_gray;
logic [3:0]       SEG_EN;

top #(
    .NEW_FREQ_HZ     (1),
    .DEFAULT_FREQ_HZ (2),
    .WIDTH           (top_params_pkg::WIDTH)
) dut ( // device under test
    .i_clk        (i_clk),
    .i_rst        (i_rst),
    .i_man_clk    (i_man_clk),
    .i_man_clk_en (i_man_clk_en),
    .i_dir        (i_dir),
    .SEG_OUT      (SEG_OUT),
    .o_gray       (o_gray),
    .SEG_EN       (SEG_EN)
);

initial begin
    i_clk        = 0;
    i_rst        = 0;
    i_man_clk    = 0;
    i_man_clk_en = 0;
    i_dir        = 0;
end

always #5ns i_clk = ~i_clk;

initial begin
    #50ns;
    i_rst = 1;

    #50ns;
    i_rst = 0;

    i_man_clk    = 0;
    i_man_clk_en = 0;

    #220ns;
    i_dir = 1;

    #220ns;
    i_dir = 0;
    i_man_clk_en = 1;

    repeat(20) begin
        i_man_clk = ~i_man_clk;
        #10ns;
    end

    #10ns;
    i_dir = 1;

    repeat(20) begin
        i_man_clk = ~i_man_clk;
        #10ns;
    end
end

endmodule
