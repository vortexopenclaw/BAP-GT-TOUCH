#!/usr/bin/env bash
set -euo pipefail

repo_dir="$(cd "$(dirname "$0")/.." && pwd)"
parser_file="$repo_dir/main/bap_parser.c"
settings_file="$repo_dir/main/settings.c"

rg -Fq 'strcmp(msg->parameter, "auto_fan") == 0' "$parser_file"
rg -Fq 'strcmp(msg->parameter, "manual_fan_speed") == 0' "$parser_file"
rg -Fq 'settings_update_auto_fan_control(strcmp(value, "1") == 0)' "$parser_file"
rg -Fq 'speed_percent < 0 || speed_percent > 100' "$parser_file"
rg -Fq 'settings_update_fan_speed_percent((int)speed_percent)' "$parser_file"
rg -Fq 'current_settings.auto_fan_control = enabled' "$settings_file"
rg -Fq 'current_settings.fan_speed_percent = speed_percent' "$settings_file"

echo "fan settings sync contract tests passed"
