MATRIX MULTIPLICATION OPTIMIZATION
==================================

1. File → New → Project
2. Изберете "Visual C++" → "Empty Project" 
3. Име: "MatrixMultiplication"
4. Location: Изберете папка по ваш избор

Настройки на проекта (Project Properties):

1. Задайте Configuration: "All Configurations"
2. Задайте Platform: "x64"

Основни настройки (C/C++ → General):
- Debug Information Format: Program Database (/Zi)

Оптимизации (C/C++ → Optimization):
- Optimization: Maximize Speed (/O2)
- Inline Function Expansion: Any Suitable (/Ob2)
- Intrinsic Functions: Yes (/Oi)

Инструкции (C/C++ → Code Generation):
- Enable Enhanced Instruction Set: Advanced Vector Extensions 2 (/arch:AVX2)
- Runtime Library: Multi-threaded DLL (/MD)

OpenMP (C/C++ → Language):
- Open MP Support: Yes (/openmp)

Linker настройки (Linker → System):
- SubSystem: Console (/SUBSYSTEM:CONSOLE)

==================================
КОМПИЛАЦИЯ ОТ КОНЗОЛА:
==================================
1. Стартирайте "Developer Command Prompt"
2. Навигирайте до папката с проекта
3. Компилирайте:
cl /O2 /arch:AVX2 /openmp /Fe:matrix_mult.exe main.cpp mm_mult.cpp