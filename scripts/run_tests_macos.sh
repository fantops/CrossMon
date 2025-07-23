#!/bin/bash
# CrossMon - Test Launcher for macOS
# Simple menu to run different tests

show_menu() {
    clear
    echo "========================================"
    echo "CrossMon - Test Launcher"
    echo "========================================"
    echo
    echo "Choose a test to run:"
    echo
    echo "1. Build and Test Everything"
    echo "2. Quick System Monitor Test"
    echo "3. GPU Detection Test"
    echo "4. Application Monitoring Test (TextEdit)"
    echo "5. Show Help"
    echo "6. Exit"
    echo
}

# Get script directory and go to project root
cd "$(dirname "$0")/.."

while true; do
    show_menu
    read -p "Enter your choice (1-6): " choice
    
    case $choice in
        1)
            bash scripts/build_and_test_macos.sh
            read -p "Press any key to continue..."
            ;;
        2)
            bash scripts/test_quick_macos.sh
            ;;
        3)
            bash scripts/test_gpu_detection_macos.sh
            ;;
        4)
            bash scripts/test_app_monitoring_macos.sh
            ;;
        5)
            if [ -f "build/crossmon" ]; then
                ./build/crossmon --help
            else
                echo "CrossMon not built yet. Please run option 1 first."
            fi
            echo
            read -p "Press any key to continue..."
            ;;
        6)
            echo "Goodbye!"
            exit 0
            ;;
        *)
            echo "Invalid choice. Please try again."
            read -p "Press any key to continue..."
            ;;
    esac
done
