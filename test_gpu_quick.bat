@echo off
cd /d "c:\Users\osingh\Work_Docs\Repos\CrossMon"
echo Testing improved GPU monitoring...
echo Starting CrossMon with 3-second intervals...
timeout /t 3 >nul
build\Release\crossmon.exe -i 3000 -o test_gpu_results.txt &
echo CrossMon started in background
timeout /t 15 >nul
echo Test completed!
echo Check test_gpu_results.txt for results
