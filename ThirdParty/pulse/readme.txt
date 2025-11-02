# WoMA3Dengine - PulseAudio for WSL2
# ---------------------------------

## Current Working Setup

### Script Purposes
0_install_pulse_win.bat  
    ➜ Downloads and installs (or updates) only the PulseAudio Windows binaries.

1_start_wsl_with_audio.bat  
    ➜ Launches WSL and ensures it’s configured with the correct audio environment variables.

2_start_pulse_audio.bat  
    ➜ Starts the PulseAudio daemon on Windows (background service).

3_test_pulse_win.bat  
    ➜ Performs a connectivity test by playing `/usr/share/sounds/alsa/Front_Center.wav`.
