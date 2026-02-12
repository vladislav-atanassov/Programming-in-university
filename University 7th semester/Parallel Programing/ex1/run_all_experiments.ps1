# run_all_experiments.ps1
# Runs all experiment scripts (1–3) and writes output to console

# Define experiments
$experiments = @(
    @{ file = "experiment_1.ps1"; title = "Experiment 1: Strong Scaling (Amdahl's Law)" },
    @{ file = "experiment_2.ps1"; title = "Experiment 2: Weak Scaling (Gustafson's Law)" },
    @{ file = "experiment_3.ps1"; title = "Experiment 3: Comparison of std::thread vs OpenMP" }
)

foreach ($exp in $experiments) {
    $file = $exp.file
    $title = $exp.title

    Write-Host "============================================================"
    Write-Host ">>> $title"
    Write-Host "============================================================"
    Write-Host ""

    # Run the experiment and capture its output
    & powershell -ExecutionPolicy Bypass -File $file
    Write-Host ""
}

Write-Host "============================================================"
Write-Host "All experiments completed successfully."
Write-Host "============================================================"
