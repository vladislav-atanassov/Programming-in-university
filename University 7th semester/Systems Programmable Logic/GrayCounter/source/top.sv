module top
    import top_params_pkg::*;
    #(
        parameter NEW_FREQ_HZ     = top_params_pkg::NEW_FREQ_HZ,
        parameter DEFAULT_FREQ_HZ = top_params_pkg::DEFAULT_FREQ_HZ,
        parameter WIDTH           = top_params_pkg::WIDTH
    )(
        input logic i_clk,
        input logic i_rst,
        input logic i_man_clk,
        input logic i_man_clk_en,
        input logic i_dir,

        output logic [6:0]       SEG_OUT,
        output logic [WIDTH-1:0] o_gray ,
        output logic [3:0]       SEG_EN

    );

    logic clk;
    logic [WIDTH-1:0] gray;

    assign o_gray = gray;
    assign SEG_EN = 4'b1110;

    clock_divider #(
        .NEW_FREQ_HZ(NEW_FREQ_HZ),
        .DEFAULT_FREQ_HZ(DEFAULT_FREQ_HZ)
    ) div (
        .i_clk(i_clk),
        .i_rst(i_rst),
        .o_clk(clk)
    );

    gray_counter #(
        .WIDTH(WIDTH)
    ) gray_cnt (
        .i_clk       (clk),
        .i_rst       (i_rst),
        .i_man_clk   (i_man_clk),
        .i_man_clk_en(i_man_clk_en),
        .i_dir       (i_dir),
        .o_gray      (gray)
    );

    sevenseg_decoder #(
        .WIDTH(WIDTH)
    ) seg (
        .VALUE  (gray),
        .SEG_OUT(SEG_OUT)
    );

endmodule : top
