@echo off
start "" cmd /k "D:\programas\JuegoRPG\reino.exe"
{
    "label": "run",
    "type": "shell",
    "command": "run.bat",
    "dependsOn": "build",
    "presentation": {
        "echo": true,
        "reveal": "never",
        "focus": false,
        "panel": "shared"
    }
}