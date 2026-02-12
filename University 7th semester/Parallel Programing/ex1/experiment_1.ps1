# experiment_analyze.ps1

# -------------------------
# Configuration
# -------------------------
$SRC = "ex1.cpp"
$EXE_STD = "ex3_std.exe"

$ARRAY_SIZE = 10000000
$THREADS = @(1, 2, 4, 8)
$REPEATS = 8

# -------------------------
# Compile
# -------------------------
g++ -std=c++17 -O2 -o $EXE_STD $SRC
if ($LASTEXITCODE -ne 0) { Write-Host "Compilation failed!"; exit }

Write-Host "Running tests on array size $ARRAY_SIZE`n"

# -------------------------
# Sequential baseline
# -------------------------
$SEQ_OUTPUT = & .\$EXE_STD $ARRAY_SIZE 1
$SEQ_TIME_LINE = $SEQ_OUTPUT | Select-String "Sequential time"
$SEQ_TIME = [double]($SEQ_TIME_LINE -split " ")[2]
Write-Host "Sequential time: $SEQ_TIME ms`n"

# -------------------------
# Table header
# -------------------------
Write-Host "Threads | Avg Time (ms) | Speedup | Efficiency (%)"
Write-Host "-----------------------------------------------"

# -------------------------
# Loop over thread counts
# -------------------------
foreach ($t in $THREADS) {
    $TOTAL = 0
    for ($i = 1; $i -le $REPEATS; $i++) {
        $OUTPUT = & .\$EXE_STD $ARRAY_SIZE $t
        # Find the first line after STD::THREAD RESULTS header that starts with a number
        $TIME_LINE = $OUTPUT | Select-String -Pattern "STD::THREAD RESULTS" -Context 2,5 | ForEach-Object {
            $_.Context.PostContext | Where-Object { $_ -match '^\s*\d+' } | Select-Object -First 1
        }
        # Extract the Time column (2nd column)
        $TIME = ($TIME_LINE -split '\|')[1].Trim()
        $TOTAL += [double]$TIME
    }

    # Compute average, speedup, efficiency
    $AVG = [math]::Round($TOTAL / $REPEATS, 2)
    $SPEEDUP = [math]::Round($SEQ_TIME / $AVG, 2)
    $EFF = [math]::Round($SPEEDUP / $t * 100, 2)

    Write-Host ("{0,7} | {1,13} | {2,7} | {3,14}" -f $t, $AVG, $SPEEDUP, $EFF)
}
