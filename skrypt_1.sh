#!/bin/bash

PROCESS_NAME="nginx"
LOG_FILE="/var/log/process_monitor.log"
CPU_THRESHOLD=80
MEM_THRESHOLD=500 # w MB
EMAIL="admin@example.com"

function monitor_resources() {
    local cpu_usage=$(ps -C "$PROCESS_NAME" -o %cpu= | awk '{s+=$1} END {print s}')
    local mem_usage=$(ps -C "$PROCESS_NAME" -o rss= | awk '{s+=$1} END {print s / 1024}')

    echo "$(date): CPU=${cpu_usage}% MEM=${mem_usage}MB" >> "$LOG_FILE"

    if (( ${cpu_usage%%.*} > CPU_THRESHOLD )); then
        echo "Proces $PROCESS_NAME przekroczył CPU: ${cpu_usage}%" | mail -s "CPU Alert" "$EMAIL"
    fi

    if (( ${mem_usage%%.*} > MEM_THRESHOLD )); then
        echo "Proces $PROCESS_NAME przekroczył pamięć: ${mem_usage}MB" | mail -s "Memory Alert" "$EMAIL"
    fi
}