# weak_scaling.ps1

# -------------------------
# Configuration
# -------------------------
$SRC = "ex1.cpp"
$EXE_STD = "ex3_std.exe"
$BASE_SIZE = 1000000   # Adjust to get ~0.5-1s runtime for 1 thread
$THREADS = @(1, 2, 4, 8)
$REPEATS = 3

# -------------------------
# Compile
# -------------------------
g++ -std=c++17 -O2 -o $EXE_STD $SRC
if ($LASTEXITCODE -ne 0) { Write-Host "Compilation failed!"; exit }

Write-Host "Weak Scaling Experiment (Base size $BASE_SIZE)`n"

# -------------------------
# Run for 1 thread to get T(1)
# -------------------------
$OUTPUT_1 = & .\$EXE_STD $BASE_SIZE 1
$TIME_LINE_1 = $OUTPUT_1 | Select-String "STD::THREAD RESULTS" -Context 2,5 | ForEach-Object {
    $_.Context.PostContext | Where-Object { $_ -match '^\s*\d+' } | Select-Object -First 1
}
$T1 = [double](($TIME_LINE_1 -split '\|')[1].Trim())
Write-Host "T(1) = $T1 ms`n"

# -------------------------
# Table header
# -------------------------
Write-Host "Threads | Array Size       | Avg Time (ms) | Weak Scaling Efficiency (%)"
Write-Host "---------------------------------------------------------------"

# -------------------------
# Loop over threads for weak scaling
# -------------------------
foreach ($t in $THREADS) {
    $ARRAY_SIZE = $BASE_SIZE * $t
    $TOTAL = 0
    for ($i = 1; $i -le $REPEATS; $i++) {
        $OUTPUT = & .\$EXE_STD $ARRAY_SIZE $t
        # Extract first numeric row under STD::THREAD RESULTS
        $TIME_LINE = $OUTPUT | Select-String -Pattern "STD::THREAD RESULTS" -Context 2,5 | ForEach-Object {
            $_.Context.PostContext | Where-Object { $_ -match '^\s*\d+' } | Select-Object -First 1
        }
        $TIME = ($TIME_LINE -split '\|')[1].Trim()
        $TOTAL += [double]$TIME
    }
    $AVG = [math]::Round($TOTAL / $REPEATS, 2)
    $EFF_WEAK = [math]::Round($T1 / $AVG * 100, 2)

    Write-Host ("{0,7} | {1,15} | {2,13} | {3,25}" -f $t, $ARRAY_SIZE, $AVG, $EFF_WEAK)
}
