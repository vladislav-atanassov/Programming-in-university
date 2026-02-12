# ==============================
# Experiment 3: Comparison of std::thread vs OpenMP
# ==============================
# Based on your C++ program structure and output.
# Requires: g++ compiler and your ex1.cpp file.

# -------- CONFIGURATION --------
$SRC = "ex1.cpp"
$EXE_STD = "ex3_std.exe"
$EXE_OMP = "ex3_omp.exe"
$THREADS = @(1, 2, 4, 8)
$REPEATS = 3
$ARRAY_SIZES = @(100000, 10000000, 100000000)

# -------- COMPILATION --------
g++ -std=c++17 -O2 -o $EXE_STD $SRC
g++ -std=c++17 -O2 -fopenmp -o $EXE_OMP $SRC

# -------- HELPER FUNCTION --------
function Get-TimeFromOutput {
    param(
        [string[]]$lines,
        [string]$sectionHeader
    )

    $found = $false
    foreach ($line in $lines) {
        if ($found -and $line -match '^\s*\d+\s*\|') {
            # extract the 2nd column (time)
            $parts = $line -split '\|'
            if ($parts.Count -ge 2) {
                $num = [regex]::Match($parts[1], '[0-9]+(\.[0-9]+)?')
                if ($num.Success) { return [double]$num.Value }
            }
        }
        if ($line -match $sectionHeader) { $found = $true }
    }
    return $null
}

function Get-SequentialTime {
    param([string[]]$lines)
    foreach ($line in $lines) {
        if ($line -match 'Sequential time:') {
            $num = [regex]::Match($line, '[0-9]+(\.[0-9]+)?')
            if ($num.Success) { return [double]$num.Value }
        }
    }
    return 0
}

# -------- MAIN EXPERIMENT LOOP --------
foreach ($size in $ARRAY_SIZES) {
    Write-Host "`n============================================"
    Write-Host "Array Size: $size" -ForegroundColor Yellow
    Write-Host "============================================"

    # Run the std::thread version (no OpenMP)
    Write-Host "`nSTD::THREAD IMPLEMENTATION" -ForegroundColor Cyan
    $seqOut = & .\${EXE_STD} $size 1
    $seqTime = Get-SequentialTime -lines $seqOut
    Write-Host "Sequential baseline: $seqTime ms`n"

    Write-Host ("{0,7} | {1,13} | {2,7} | {3,14}" -f "Threads", "Avg Time (ms)", "Speedup", "Efficiency (%)")
    Write-Host ("-" * 50)

    foreach ($t in $THREADS) {
        $total = 0.0
        for ($i = 1; $i -le $REPEATS; $i++) {
            $out = & .\${EXE_STD} $size $t
            $time = Get-TimeFromOutput -lines $out -sectionHeader "STD::THREAD RESULTS"
            $total += $time
        }
        $avg = [math]::Round($total / $REPEATS, 2)
        $speedup = [math]::Round($seqTime / $avg, 2)
        $eff = [math]::Round($speedup / $t * 100, 2)
        Write-Host ("{0,7} | {1,13} | {2,7} | {3,14}" -f $t, $avg, $speedup, $eff)
    }

    # Run the OpenMP version
    Write-Host "`nOPENMP IMPLEMENTATION" -ForegroundColor Cyan
    $seqOutOmp = & .\${EXE_OMP} $size 1
    $seqTimeOmp = Get-SequentialTime -lines $seqOutOmp
    Write-Host "Sequential baseline: $seqTimeOmp ms`n"

    Write-Host ("{0,7} | {1,13} | {2,7} | {3,14}" -f "Threads", "Avg Time (ms)", "Speedup", "Efficiency (%)")
    Write-Host ("-" * 50)

    foreach ($t in $THREADS) {
        $total = 0.0
        for ($i = 1; $i -le $REPEATS; $i++) {
            $out = & .\${EXE_OMP} $size $t
            $time = Get-TimeFromOutput -lines $out -sectionHeader "OPENMP RESULTS"
            $total += $time
        }
        $avg = [math]::Round($total / $REPEATS, 2)
        $speedup = [math]::Round($seqTimeOmp / $avg, 2)
        $eff = [math]::Round($speedup / $t * 100, 2)
        Write-Host ("{0,7} | {1,13} | {2,7} | {3,14}" -f $t, $avg, $speedup, $eff)
    }
}

Write-Host "`nExperiment 3 complete!" -ForegroundColor Green
